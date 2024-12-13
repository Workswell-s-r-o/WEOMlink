#include "wl/weom/deviceinterfaceweom.h"

#include "wl/communication/protocolinterfacetcsi.h"

#include <cmath>


namespace wl {


DeviceInterfaceWEOM::DeviceInterfaceWEOM(etl::unique_ptr<ProtocolInterfaceTCSI> protocolInterface, SleepFunction sleepFunction) :
    BaseClass(BaseClass::DeviceEndianity::LITTLE),
    m_protocolInterface(etl::move(protocolInterface)),
    m_memorySpace(MemorySpaceWEOM::getDeviceSpace()),
    m_sleepFunction(sleepFunction)
{

}

const MemorySpaceWEOM& DeviceInterfaceWEOM::getMemorySpace() const
{
    return m_memorySpace;
}

void DeviceInterfaceWEOM::setMemorySpace(const MemorySpaceWEOM& memorySpace)
{
    m_memorySpace = memorySpace;
}

etl::expected<void, Error> DeviceInterfaceWEOM::readData(etl::span<uint8_t> data, uint32_t address)
{
    const auto memoryDescriptor = getMemoryDescriptorWithChecks(address, data.size());
    if (!memoryDescriptor.has_value())
    {
        return etl::unexpected<Error>(memoryDescriptor.error());
    }

    return readDataImpl(data, address, getMaxDataSize(memoryDescriptor.value()));
}

etl::expected<void, Error> DeviceInterfaceWEOM::writeData(const etl::span<const uint8_t> data, uint32_t address)
{
    const auto memoryDescriptor = getMemoryDescriptorWithChecks(address, data.size());
    if (!memoryDescriptor.has_value())
    {
        return etl::unexpected<Error>(memoryDescriptor.error());
    }

    const uint32_t maxDataSize = getMaxDataSize(memoryDescriptor.value());
    Duration busyDelayTotal = std::chrono::milliseconds(0);
    ErrorWindow lastErrors;

    return writeDataImpl(data, address, TIMEOUT_DEFAULT, maxDataSize, busyDelayTotal, lastErrors);
}


etl::expected<void, Error> DeviceInterfaceWEOM::writeDataImpl(const etl::span<const uint8_t> data, uint32_t address, const Duration& expectedOperationDuration,
                                                const uint32_t maxDataSize, Duration& busyDelayTotal, ErrorWindow& lastErrors)
{
    etl::span<const uint8_t> restOfData = data;
    for (uint32_t currentAddress = address; !restOfData.empty(); )
    {
        const auto dataSize = std::min<uint32_t>(restOfData.size(), maxDataSize);

        const auto writeResult = m_protocolInterface->writeData(restOfData.first(dataSize), currentAddress, expectedOperationDuration);
        lastErrors <<= 1;
        if (writeResult.has_value())
        {
            currentAddress += dataSize;
            restOfData = restOfData.last(restOfData.size() - dataSize);
        }
        else
        {
            const auto result = handleErrorResponse(writeResult, lastErrors, busyDelayTotal);
            if (!result.has_value())
            {
                return result;
            }
        }
    }

    return {};
}

etl::expected<void, Error> DeviceInterfaceWEOM::readDataImpl(etl::span<uint8_t> data, uint32_t address, uint32_t maxDataSize)
{
    Duration busyDelayTotal = std::chrono::milliseconds(0);
    ErrorWindow lastErrors;

    etl::span<uint8_t> restOfData = data;
    for (uint32_t currentAddress = address; !restOfData.empty(); )
    {
        const auto addressRange = AddressRange::firstAndSize(currentAddress, std::min<uint32_t>(restOfData.size(), maxDataSize));

        const auto dataRange = restOfData.first(addressRange.getSize());
        const auto readResult = m_protocolInterface->readData(dataRange, addressRange.getFirstAddress(), TIMEOUT_DEFAULT);
        lastErrors <<= 1;
        if (readResult.has_value())
        {
            currentAddress += addressRange.getSize();
            restOfData = restOfData.last(restOfData.size() - addressRange.getSize());
        }
        else
        {
            const auto result = handleErrorResponse(readResult, lastErrors, busyDelayTotal);
            if (!result.has_value())
            {
                return result;
            }
        }
    }

    return {};
}

etl::expected<void, Error> DeviceInterfaceWEOM::handleErrorResponse(etl::expected<void, Error> operationResult, ErrorWindow& lastErrors, Duration& busyDelayTotal)
{
    if (!operationResult.has_value())
    {
        if (operationResult.error() == Error::DATALINK__TIMEOUT ||
            operationResult.error() == Error::TCSI__INVALID_SIZE ||
            operationResult.error() == Error::TCSI__INVALID_SYNCHRONIZATION_VALUE ||
            operationResult.error() == Error::TCSI__INVALID_STATUS_OR_COMMAND ||
            operationResult.error() == Error::TCSI__INVALID_CHECKSUM ||
            operationResult.error() == Error::TCSI__INVALID_RESPONSE_ADDRESS ||
            operationResult.error() == Error::TCSI__RESPONSE_STATUS_ERROR)
        {
            lastErrors.set(0, 1);
            if (lastErrors.count() <= MAX_ERRORS_IN_WINDOW)
            {
                return {};
            }
            else
            {
                return etl::unexpected<Error>(Error::DEVICE__DISCONNECTED);
            }
        }
        else if (operationResult.error() == Error::TCSI__RESPONSE_DEVICE_BUSY)
        {
            busyDelayTotal += BUSY_DEVICE_DELAY;
            if (busyDelayTotal < BUSY_DEVICE_TIMEOUT)
            {
                assert(m_sleepFunction);
                m_sleepFunction(BUSY_DEVICE_DELAY);
                return {};
            }
            else
            {
                return etl::unexpected<Error>(Error::DEVICE__BUSY);
            }
        }
    }
    return operationResult;

}

etl::expected<MemoryDescriptorWEOM, Error> DeviceInterfaceWEOM::getMemoryDescriptorWithChecks(uint32_t address, etl::optional<size_t> dataSize) const
{
    if (!m_protocolInterface || m_protocolInterface->getMaxDataSize() == 0)
    {
        return etl::unexpected<Error>(Error::DEVICE__NO_PROTOCOL);
    }

    if (dataSize && dataSize.value() == 0)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_DATA_SIZE);
    }

    if (dataSize && dataSize.value() - 1 > std::numeric_limits<uint32_t>::max() - address)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_ADDRESS);
    }

    const etl::expected<MemoryDescriptorWEOM, Error> memoryDescriptor = m_memorySpace.getMemoryDescriptor(AddressRange::firstAndSize(address, dataSize.value_or(1)));

    if (!memoryDescriptor.has_value())
    {
        return etl::unexpected<Error>(memoryDescriptor.error());
    }

    if (address % memoryDescriptor.value().minimumDataSize != 0)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_ADDRESS);
    }

    if (dataSize && dataSize.value() % memoryDescriptor.value().minimumDataSize != 0)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_DATA_SIZE);
    }

    return memoryDescriptor;
}

uint32_t DeviceInterfaceWEOM::getMaxDataSize(const MemoryDescriptorWEOM& memoryDescriptor) const
{
    const auto protocolMaxDataSize = (m_protocolInterface->getMaxDataSize() / memoryDescriptor.minimumDataSize) * memoryDescriptor.minimumDataSize;
    assert(protocolMaxDataSize > 0);
    return std::min(memoryDescriptor.maximumDataSize, protocolMaxDataSize);
}

} // namespace wl
