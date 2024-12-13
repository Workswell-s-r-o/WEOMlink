#include "wl/communication/protocolinterfacetcsi.h"

#include "wl/communication/addressrange.h"
#include "wl/communication/idatalinkinterface.h"
#include "wl/misc/elapsedtimer.h"

#include <algorithm>


namespace wl {


ProtocolInterfaceTCSI::ProtocolInterfaceTCSI(SleepFunction sleepFunction)
    : m_sleepFunction(sleepFunction)
{
}

void ProtocolInterfaceTCSI::setDataLinkInterface(etl::unique_ptr<IDataLinkInterface> dataLinkInterface)
{
    etl::lock_guard lock(m_mutex);

    m_dataLinkInterface = etl::move(dataLinkInterface);

    m_straightNoResponsesCount = 0;
    m_connectionLost = false;
}

uint32_t ProtocolInterfaceTCSI::getMaxDataSize() const
{
    if (!m_dataLinkInterface || m_dataLinkInterface->getMaxDataSize() < TCSIPacket::MINIMUM_PACKET_SIZE)
    {
        return 0;
    }

    const uint32_t maxDatalinkDataSize = m_dataLinkInterface->getMaxDataSize() - TCSIPacket::MINIMUM_PACKET_SIZE;
    const uint32_t maxTcsiDataSize = std::numeric_limits<uint8_t>::max();

    return std::min(maxDatalinkDataSize, maxTcsiDataSize);
}

etl::expected<void, Error> ProtocolInterfaceTCSI::readData(etl::span<uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout)
{
    if (!m_dataLinkInterface)
    {
        return etl::unexpected<Error>(Error::PROTOCOL__NO_DATALINK);
    }

    if (data.empty())
    {
        assert(false && "trying to read nothing? - weird");
        return {};
    }

    const auto responsePacket = readDataImpl(data.size(), address, timeout);
    if (!responsePacket.has_value())
    {
        return etl::unexpected<Error>(responsePacket.error());
    }

    assert(responsePacket.value().getPayloadData().size() == data.size());
    std::copy(responsePacket.value().getPayloadData().begin(), responsePacket.value().getPayloadData().end(), data.begin());

    return {};
}

etl::expected<void, Error> ProtocolInterfaceTCSI::writeData(const etl::span<const uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout)
{
    if (data.empty())
    {
        assert(false && "trying to write nothing? - weird");
        return {};
    }

    if (!m_dataLinkInterface)
    {
        return etl::unexpected<Error>(Error::PROTOCOL__NO_DATALINK);
    }

    etl::lock_guard lock(m_mutex);

    auto writeRequest = TCSIPacket::createWriteRequest(++m_lastPacketId, address, data);
    return writeDataImpl(writeRequest, address, timeout);
}

bool ProtocolInterfaceTCSI::isConnectionLost() const
{
    return m_connectionLost;
}

etl::expected<TCSIPacket, Error> ProtocolInterfaceTCSI::readDataImpl(uint32_t dataSize, uint32_t address, const std::chrono::steady_clock::duration& timeout)
{
    etl::lock_guard lock(m_mutex);

    auto readRequest = TCSIPacket::createReadRequest(++m_lastPacketId, address, dataSize);
    m_lastPacketId = readRequest.getPacketId();

    const ElapsedTimer timer(timeout);
    const auto readRequestResult = m_dataLinkInterface->write(readRequest.getPacketData(), timeout);
    if (!readRequestResult.has_value())
    {
        return etl::unexpected<Error>(readRequestResult.error());
    }

    return receiveResponse(m_lastPacketId, address, dataSize, timer.getRestOfTimeout());
}

etl::expected<void, Error> ProtocolInterfaceTCSI::writeDataImpl(TCSIPacket& writeRequest, uint32_t address, const std::chrono::steady_clock::duration& timeout)
{
    m_lastPacketId = writeRequest.getPacketId();

    const ElapsedTimer timer(timeout);
    const auto writeRequestResult = m_dataLinkInterface->write(writeRequest.getPacketData(), timeout);
    if (!writeRequestResult.has_value())
    {
        return etl::unexpected<Error>(writeRequestResult.error());
    }

    auto responseResult = receiveResponse(m_lastPacketId, address, 0, timer.getRestOfTimeout());
    if (!responseResult.has_value())
    {
        return etl::unexpected<Error>(responseResult.error());
    }
    return {};
}

etl::expected<TCSIPacket, Error> ProtocolInterfaceTCSI::receiveResponse(uint8_t packetId, uint32_t address, uint32_t dataSize, const std::chrono::steady_clock::duration& timeout)
{
    const ElapsedTimer timer(timeout);
    while (true)
    {
        const auto responsePacketResult = receiveResponsePacket(timer);
        if (!responsePacketResult.has_value())
        {
            return etl::unexpected<Error>(responsePacketResult.error());
        }

        const auto responseValidationResult = responsePacketResult.value().validateAsResponse(address);
        if (!responseValidationResult.has_value())
        {
            dropPendingData(timer.getRestOfTimeout());
            return etl::unexpected<Error>(responseValidationResult.error());
        }

        if (responsePacketResult.value().getPacketId() == packetId)
        {
            const auto okValidationResult = responsePacketResult.value().validateAsOkResponse(address, dataSize);
            if (okValidationResult.has_value())
            {
                return responsePacketResult.value();
            }
            else
            {
                return etl::unexpected<Error>(okValidationResult.error());
            }
        }
    }
}

etl::expected<TCSIPacket, Error> ProtocolInterfaceTCSI::receiveResponsePacket(const ElapsedTimer& timer)
{
    // try read empty response (ERROR / OK confirmation) or first part of non-empty response
    etl::vector<uint8_t, TCSIPacket::MAXIMUM_PACKET_SIZE> receivedData(TCSIPacket::MINIMUM_PACKET_SIZE, 0);
    const auto readResponseResult = m_dataLinkInterface->read(receivedData, timer.getRestOfTimeout());
    if (!readResponseResult.has_value())
    {
        if (readResponseResult.error() == Error::DATALINK__TIMEOUT)
        {
            ++m_straightNoResponsesCount;

            if (m_straightNoResponsesCount > MAX_STRAIGHT_NO_RESPONSES_COUNT)
            {
                m_connectionLost = true;
            }
        }

        dropPendingData(timer.getRestOfTimeout());
        return etl::unexpected<Error>(readResponseResult.error());
    }
    m_straightNoResponsesCount = 0;

    TCSIPacket responsePacket(receivedData);
    const auto expectedDataSize = responsePacket.getExpectedDataSize();
    if (!expectedDataSize.has_value())
    {
        dropPendingData(timer.getRestOfTimeout());
        return etl::unexpected<Error>(expectedDataSize.error());
    }
    else if (expectedDataSize.value() > 0)
    {
        // try read rest of response
        const auto packetSize = receivedData.size();
        receivedData.resize(packetSize + expectedDataSize.value(), 0);

        if (const auto readRestOfResponseResult = m_dataLinkInterface->read(etl::span<uint8_t>(receivedData).subspan(packetSize, expectedDataSize.value()), timer.getRestOfTimeout()); !readRestOfResponseResult.has_value())
        {
            dropPendingData(timer.getRestOfTimeout());
            return etl::unexpected<Error>(readRestOfResponseResult.error());
        }

        responsePacket = TCSIPacket(receivedData);
    }

    return responsePacket;
}

void ProtocolInterfaceTCSI::dropPendingData(const std::chrono::steady_clock::duration& restOfTimeout)
{
    assert(m_sleepFunction);
    m_sleepFunction(restOfTimeout);

    m_dataLinkInterface->dropPendingData();
}

} // namespace wl
