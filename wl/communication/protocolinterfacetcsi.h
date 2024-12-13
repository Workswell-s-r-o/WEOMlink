#ifndef WL_PROTOCOLINTERFACETCSI_H
#define WL_PROTOCOLINTERFACETCSI_H

#include "wl/communication/idatalinkinterface.h"
#include "wl/communication/iprotocolinterface.h"
#include "wl/communication/tcsipacket.h"
#include "wl/misc/elapsedtimer.h"
#include "wl/error.h"
#include "wl/time.h"

#include <etl/mutex.h>
#include <etl/expected.h>
#include <etl/memory.h>

#include <stddef.h>

namespace wl {

/**
 * @class ProtocolInterfaceTCSI
 * @headerfile protocolinterfacetcsi.h "wl/communication/protocolitnerfacetcsi.h"
 * @brief A class that implements the TCSI protocol interface, responsible for managing data communication.
 * 
 * The ProtocolInterfaceTCSI class provides an interface for reading and writing data over a TCSI protocol.
 * It utilizes a data link interface for low-level communication.
 */
class ProtocolInterfaceTCSI : public IProtocolInterface
{
    using BaseClass = IProtocolInterface;

public:
    /**
     * @brief Constructs a ProtocolInterfaceTCSI object.
     * @param sleepFunction User-defined function to handle delays, taking a duration as input.
     */
    ProtocolInterfaceTCSI(SleepFunction sleepFunction);

    /**
     * @brief Sets the data link interface used for communication.
     * @param dataLinkInterface A unique pointer to an IDataLinkInterface object.
     */
    void setDataLinkInterface(etl::unique_ptr<IDataLinkInterface> dataLinkInterface);

    /**
     * @brief Retrieves the maximum data size supported by this protocol interface.
     * @return The maximum data size in bytes.
     */
    virtual uint32_t getMaxDataSize() const override;

    /**
     * @brief Reads data from the specified address with a timeout.
     * @param data A span to store the read data.
     * @param address The address to read from.
     * @param timeout The maximum duration for the read operation.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> readData(etl::span<uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout) override;

    /**
     * @brief Writes data to the specified address with a timeout.
     * @param data A span of the data to write.
     * @param address The address to write to.
     * @param timeout The maximum duration for the write operation.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> writeData(const etl::span<const uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout) override;

    /**
     * @brief Checks if the connection has been lost.
     * @return true if the connection is lost, false otherwise.
     */
    bool isConnectionLost() const;

private:
    [[nodiscard]] etl::expected<TCSIPacket, Error> readDataImpl(uint32_t dataSize, uint32_t address, const std::chrono::steady_clock::duration& timeout);
    [[nodiscard]] etl::expected<void, Error> writeDataImpl(TCSIPacket& packet, uint32_t address, const std::chrono::steady_clock::duration& timeout);

    [[nodiscard]] etl::expected<TCSIPacket, Error> receiveResponse(uint8_t packetId, uint32_t address, uint32_t dataSize, const std::chrono::steady_clock::duration& timeout);
    [[nodiscard]] etl::expected<TCSIPacket, Error> receiveResponsePacket(const ElapsedTimer& timer);
    void dropPendingData(const std::chrono::steady_clock::duration& restOfTimeout);

    static constexpr size_t MAX_STRAIGHT_NO_RESPONSES_COUNT = 2;

    etl::unique_ptr<IDataLinkInterface> m_dataLinkInterface;
    uint8_t m_lastPacketId {0};

    size_t m_straightNoResponsesCount {0};
    bool m_connectionLost {false};

    mutable etl::mutex m_mutex;
    SleepFunction m_sleepFunction;
};

} // namespace wl

#endif // WL_PROTOCOLINTERFACETCSI_H
