#include "wl/communication/tcsipacket.h"

#include "wl/communication/addressrange.h"
#include "wl/misc/endian.h"

#include <etl/array.h>
#include <etl/algorithm.h>

#include <cassert>

namespace wl {

TCSIPacket::TCSIPacket(const etl::span<uint8_t>& packetData) :
    m_packetData(packetData.begin(), packetData.end())
{
}

TCSIPacket TCSIPacket::createReadRequest(uint8_t packetId, uint32_t address, uint8_t payloadDataSize)
{
    const auto request = createPacket(static_cast<uint8_t>(Command::READ), packetId, address, etl::array<uint8_t, 1>{payloadDataSize});
    assert(request.validateAsRequest().has_value());
    return request;
}

TCSIPacket TCSIPacket::createWriteRequest(uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData)
{
    const auto request = createPacket(static_cast<uint8_t>(Command::WRITE), packetId, address, payloadData);
    assert(request.validateAsRequest().has_value());
    return request;
}

TCSIPacket TCSIPacket::createBurstStartRequest(uint8_t packetId, uint32_t address)
{
    uint8_t payloadData[] = {0, 0, 0, 1};
    const auto request = createPacket(static_cast<uint8_t>(Command::FLASH_BURST_START), packetId, address, payloadData);
    assert(request.validateAsRequest().has_value());
    return request;
}

TCSIPacket TCSIPacket::createBurstEndRequest(uint8_t packetId, uint32_t address)
{
    const auto request = createPacket(static_cast<uint8_t>(Command::FLASH_BURST_END), packetId, address, etl::span<const uint8_t>());
    assert(request.validateAsRequest().has_value());
    return request;
}

TCSIPacket TCSIPacket::createOkResponse(uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData)
{
    const auto response = createPacket(static_cast<uint8_t>(Status::OK), packetId, address, payloadData);
    assert(response.validateAsOkResponse(address, payloadData.size()).has_value());
    return response;
}

TCSIPacket TCSIPacket::createErrorResponse(uint8_t packetId, uint32_t address, Status status)
{
    const auto response = createPacket(static_cast<uint8_t>(status), packetId, address, etl::span<const uint8_t>{});
    assert(response.validateAsOkResponse(address, 0).has_value() == (status == Status::OK));
    return response;
}

TCSIPacket TCSIPacket::createPacket(uint8_t statusOrCommand, uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData)
{
    etl::vector<uint8_t, MAXIMUM_PACKET_SIZE> packetData(MINIMUM_PACKET_SIZE + payloadData.size(), 0);

    packetData.at(SYNCHRONIZATION_AND_ID_POSITION) = (SYNCHRONIZATION_MASK & SYNCHRONIZATION_VALUE) | (PACKET_ID_MASK & packetId);
    packetData.at(STATUS_OR_COMMAND_POSITION) = statusOrCommand;

    *reinterpret_cast<uint32_t*>(packetData.data() + ADDRESS_POSITION) = wl::toLittleEndian(address);

    packetData.at(COUNT_POSITION) = payloadData.size();
    etl::copy(payloadData.begin(), payloadData.end(), packetData.begin() + DATA_POSITION);

    packetData.back() = calculateCheckSum(packetData);

    const TCSIPacket packet(packetData);
    assert(packet.validate());
    assert(packet.getStatusOrCommand() == statusOrCommand);
    assert(packet.getAddress() == address);
    assert(etl::equal(packet.getPayloadDataImpl().begin(), packet.getPayloadDataImpl().end(), payloadData.begin()));

    return packet;
}

uint8_t TCSIPacket::calculateCheckSum(const etl::span<const uint8_t> packetData)
{
    assert(!packetData.empty());

    return etl::accumulate(packetData.begin(), packetData.end() - 1, 0);
}

etl::expected<void, Error> TCSIPacket::validate() const
{
    if (m_packetData.size() < MINIMUM_PACKET_SIZE)
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
    }

    if ((m_packetData.at(SYNCHRONIZATION_AND_ID_POSITION) & SYNCHRONIZATION_MASK) != (SYNCHRONIZATION_VALUE & SYNCHRONIZATION_MASK))
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SYNCHRONIZATION_VALUE);
    }

    switch (getStatusOrCommand())
    {
    case static_cast<uint8_t>(Command::READ):
    case static_cast<uint8_t>(Command::WRITE):
    case static_cast<uint8_t>(Command::FLASH_BURST_START):
    case static_cast<uint8_t>(Command::FLASH_BURST_END):
        break;

    case static_cast<uint8_t>(Status::OK):
    case static_cast<uint8_t>(Status::CAMERA_NOT_READY):
    case static_cast<uint8_t>(Status::UNKNOWN_COMMAND):
    case static_cast<uint8_t>(Status::WRONG_CHECKSUM):
    case static_cast<uint8_t>(Status::WRONG_ADDRESS):
    case static_cast<uint8_t>(Status::WRONG_ARGUMENT_COUNT):
    case static_cast<uint8_t>(Status::FLASH_BURST_ERROR):
    case static_cast<uint8_t>(Status::INVALID_SETTINGS):
    case static_cast<uint8_t>(Status::INCORRECT_VALUE):
        break;
    default:
        return etl::unexpected<Error>(Error::TCSI__INVALID_STATUS_OR_COMMAND);;
    }

    if (m_packetData.at(COUNT_POSITION) != getPayloadDataImpl().size())
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
    }

    if (const auto calculatedChecksum = calculateCheckSum(m_packetData); m_packetData.back() != calculatedChecksum)
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_CHECKSUM);
    }

    return {};
}

etl::expected<void, Error> TCSIPacket::validateAsResponse(uint32_t address) const
{
    const auto validationResult = validate();
    if (!validationResult.has_value())
    {
        return validationResult;
    }

    if (getStatusOrCommand() != static_cast<uint8_t>(Status::OK) && getStatusOrCommand() != static_cast<uint8_t>(Status::CAMERA_NOT_READY) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::UNKNOWN_COMMAND) && getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_CHECKSUM) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_ADDRESS) && getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_ARGUMENT_COUNT) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::FLASH_BURST_ERROR) && getStatusOrCommand() != static_cast<uint8_t>(Status::INVALID_SETTINGS) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::INCORRECT_VALUE))
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_STATUS_OR_COMMAND);
    }

    if (getAddress() != address)
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_RESPONSE_ADDRESS);
    }

    return {};
}

etl::expected<void, Error> TCSIPacket::validateAsOkResponse(uint32_t address, uint8_t payloadDataSize) const
{
    const auto validationResult = validateAsResponse(address);
    if (!validationResult.has_value())
    {
        return validationResult;
    }

    if (static_cast<Status>(getStatusOrCommand()) != Status::OK)
    {
        if (static_cast<Status>(getStatusOrCommand()) == Status::CAMERA_NOT_READY)
        {
            return etl::unexpected<Error>(Error::TCSI__RESPONSE_DEVICE_BUSY);
        }
        else
        {
            return etl::unexpected<Error>(Error::TCSI__RESPONSE_STATUS_ERROR);
        }
    }

    if (getPayloadDataImpl().size() != payloadDataSize)
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
    }

    return {};
}

etl::expected<void, Error> TCSIPacket::validateAsRequest() const
{
    const auto validationResult = validate();
    if (!validationResult.has_value())
    {
        return validationResult;
    }

    switch (getStatusOrCommand())
    {
    case static_cast<uint8_t>(Command::READ):
        if (getPayloadDataImpl().size() != 1)
        {
            return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
        }
        break;
    case static_cast<uint8_t>(Command::WRITE):
        if (getPayloadDataImpl().size() == 0)
        {
            return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
        }
        break;
    case static_cast<uint8_t>(Command::FLASH_BURST_START):
        if (getPayloadDataImpl().size() != 4)
        {
            return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
        }
        break;
    case static_cast<uint8_t>(Command::FLASH_BURST_END):
        if (getPayloadDataImpl().size() != 0)
        {
            return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
        }
        break;

    default:
        return etl::unexpected<Error>(Error::TCSI__INVALID_STATUS_OR_COMMAND);;
    }
    return {};
}

etl::expected<uint8_t, Error> TCSIPacket::getExpectedDataSize() const
{
    if (m_packetData.size() < HEADER_SIZE)
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SIZE);
    }

    if ((m_packetData.at(SYNCHRONIZATION_AND_ID_POSITION) & SYNCHRONIZATION_MASK) != (SYNCHRONIZATION_VALUE & SYNCHRONIZATION_MASK))
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_SYNCHRONIZATION_VALUE);
    }

    if (getStatusOrCommand() != static_cast<uint8_t>(Status::OK) && getStatusOrCommand() != static_cast<uint8_t>(Status::CAMERA_NOT_READY) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::UNKNOWN_COMMAND) && getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_CHECKSUM) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_ADDRESS) && getStatusOrCommand() != static_cast<uint8_t>(Status::WRONG_ARGUMENT_COUNT) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::FLASH_BURST_ERROR) && getStatusOrCommand() != static_cast<uint8_t>(Status::INVALID_SETTINGS) &&
        getStatusOrCommand() != static_cast<uint8_t>(Status::INCORRECT_VALUE))
    {
        return etl::unexpected<Error>(Error::TCSI__INVALID_STATUS_OR_COMMAND);
    }

    return m_packetData.at(COUNT_POSITION);
}

uint8_t TCSIPacket::getPacketId() const
{
    assert(validate().has_value());

    return m_packetData.at(SYNCHRONIZATION_AND_ID_POSITION) & PACKET_ID_MASK;
}

etl::span<const uint8_t> TCSIPacket::getPayloadData() const
{
    assert(validate().has_value());

    return getPayloadDataImpl();
}

const etl::ivector<uint8_t>& TCSIPacket::getPacketData() const
{
    return m_packetData;
}

etl::ivector<uint8_t>& TCSIPacket::getPacketData()
{
    return m_packetData;
}

etl::span<const uint8_t> TCSIPacket::getPayloadDataImpl() const
{
    return etl::span<const uint8_t>(m_packetData).subspan(HEADER_SIZE, m_packetData.size() - MINIMUM_PACKET_SIZE);
}

uint8_t TCSIPacket::getStatusOrCommand() const
{
    return m_packetData.at(STATUS_OR_COMMAND_POSITION);
}

uint32_t TCSIPacket::getAddress() const
{
    return wl::fromLittleEndian(*reinterpret_cast<const uint32_t*>(m_packetData.data() + ADDRESS_POSITION));
}

} // namespace wl
