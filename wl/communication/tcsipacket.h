#ifndef WL_TCSIPACKET_H
#define WL_TCSIPACKET_H

#include "wl/error.h"

#include <etl/expected.h>
#include <etl/vector.h>
#include <etl/span.h>

#include <cstdint>

namespace wl {

/**
 * @class TCSIPacket
 * @headerfile tcsipacket.h "wl/communication/tcsipacket.h"
 * @brief A class for creating, validating, and parsing TCSI communication packets.
 *
 * @details
 * This class provides methods to create read and write requests, OK and error responses,
 * as well as methods to validate packets and extract payload and metadata.
 */
class TCSIPacket
{
public:
    /**
     * @brief Constructs a TCSIPacket from raw packet data.
     * @param packetData The raw packet data as a span of bytes.
     */
    explicit TCSIPacket(const etl::span<uint8_t>& packetData);

    /**
     * @enum Status
     * @brief Enumeration for possible status codes in TCSI responses.
     */
    enum class Status : uint8_t
    {
        OK                   = 0x00,
        CAMERA_NOT_READY     = 0x01,
        UNKNOWN_COMMAND      = 0x02,
        WRONG_CHECKSUM       = 0x03,
        WRONG_ADDRESS        = 0x04,
        WRONG_ARGUMENT_COUNT = 0x05,
        FLASH_BURST_ERROR    = 0x06,
        INVALID_SETTINGS     = 0x07,
        INCORRECT_VALUE      = 0x08,
    };

    /**
     * @brief Creates a read request packet.
     * @param packetId The ID of the packet.
     * @param address The memory address for the read operation.
     * @param payloadDataSize The size of the data payload.
     * @return A TCSIPacket object representing the read request.
     */
    [[nodiscard]] static TCSIPacket createReadRequest(uint8_t packetId, uint32_t address, uint8_t payloadDataSize);

    /**
     * @brief Creates a write request packet.
     * @param packetId The ID of the packet.
     * @param address The memory address for the write operation.
     * @param payloadData The data payload to be written.
     * @return A TCSIPacket object representing the write request.
     */
    [[nodiscard]] static TCSIPacket createWriteRequest(uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData);

    /**
     * @brief Creates an OK response packet.
     * @param packetId The ID of the packet.
     * @param address The memory address related to the response.
     * @param payloadData The data payload to include in the response.
     * @return A TCSIPacket object representing the OK response.
     */
    [[nodiscard]] static TCSIPacket createOkResponse(uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData);

    /**
     * @brief Creates an error response packet.
     * @param packetId The ID of the packet.
     * @param address The memory address related to the response.
     * @param status The error status to include in the response.
     * @return A TCSIPacket object representing the error response.
     */
    [[nodiscard]] static TCSIPacket createErrorResponse(uint8_t packetId, uint32_t address, Status status);

    /**
     * @brief Validates the packet's structure.
     * @return An `etl::expected<void, Error>` indicating success or error.
     * @retval Error::TCSI__INVALID_SIZE if packet data size is invalid
     * @retval Error::TCSI__INVALID_SYNCHRONIZATION_VALUE if packet syncrhonization value is invalid
     * @retval Error::TCSI__INVALID_STATUS_OR_COMMAND if packet status or command byte is invalid
     * @retval Error::TCSI__INVALID_CHECKSUM if checksum is incorrect
     */
    [[nodiscard]] etl::expected<void, Error> validate() const;

    /**
     * @brief Validates the packet as a response packet for a specified address.
     * @param address The expected address in the response.
     * @return An `etl::expected<void, Error>` indicating success or error.
     * @retval Error::TCSI__INVALID_SIZE if packet data size is invalid
     * @retval Error::TCSI__INVALID_SYNCHRONIZATION_VALUE if packet syncrhonization value is invalid
     * @retval Error::TCSI__INVALID_STATUS_OR_COMMAND if packet status or command byte is invalid
     * @retval Error::TCSI__INVALID_CHECKSUM if checksum is incorrect
     * @retval Error::TCSI__INVALID_RESPONSE_ADDRESS if address and packed address do not match
     */
    [[nodiscard]] etl::expected<void, Error> validateAsResponse(uint32_t address) const;

    /**
     * @brief Validates the packet as an OK response with a specified address and payload size.
     * @param address The expected address in the response.
     * @param payloadDataSize The expected size of the payload data.
     * @return An `etl::expected<void, Error>` indicating success or error.
     * @retval Error::TCSI__INVALID_SIZE if packet data size is invalid
     * @retval Error::TCSI__INVALID_SYNCHRONIZATION_VALUE if packet syncrhonization value is invalid
     * @retval Error::TCSI__INVALID_STATUS_OR_COMMAND if packet status or command byte is invalid
     * @retval Error::TCSI__INVALID_CHECKSUM if checksum is incorrect
     * @retval Error::TCSI__INVALID_RESPONSE_ADDRESS if address and packed address do not match
     * @retval Error::TCSI__RESPONSE_DEVICE_BUSY if packet status is Status::CAMERA_NOT_READY
     * @retval Error::TCSI__RESPONSE_STATUS_ERROR if packet status is not Status::OK or Status::CAMERA_NOT_READY
     */
    [[nodiscard]] etl::expected<void, Error> validateAsOkResponse(uint32_t address, uint8_t payloadDataSize) const;

    /**
     * @brief Validates the packet as a request packet.
     * @return An `etl::expected<void, Error>` indicating success or error.
     * @retval Error::TCSI__INVALID_SIZE if packet data size is invalid
     * @retval Error::TCSI__INVALID_SYNCHRONIZATION_VALUE if packet syncrhonization value is invalid
     * @retval Error::TCSI__INVALID_STATUS_OR_COMMAND if packet status or command byte is invalid or is not Command::READ or Command::WRITE
     * @retval Error::TCSI__INVALID_CHECKSUM if checksum is incorrect
     */
    [[nodiscard]] etl::expected<void, Error> validateAsRequest() const;

    /**
     * @brief Calculates the expected size of the data payload.
     * @return An `etl::expected<uint8_t, Error>` containing the expected data size or an error.
     * @retval Error::TCSI__INVALID_SIZE if packet size smaller than minimum packet size
     * @retval Error::TCSI__INVALID_SYNCHRONIZATION_VALUE if packet syncrhonization value is invalid
     * @retval Error::TCSI__INVALID_STATUS_OR_COMMAND if packet status or command byte is invalid
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getExpectedDataSize() const;

    /**
     * @brief Retrieves the packet ID.
     * @return The packet ID.
     */
    uint8_t getPacketId() const;

    /**
     * @brief Retrieves the data payload as a span.
     * @return A span of bytes representing the payload data.
     */
    etl::span<const uint8_t> getPayloadData() const;

    /**
     * @brief Retrieves the entire packet data as a constant reference.
     * @return A constant reference to the vector containing packet data.
     */
    const etl::ivector<uint8_t>& getPacketData() const;

    /**
     * @brief Retrieves the entire packet data as a reference.
     * @return A reference to the vector containing packet data.
     */
    etl::ivector<uint8_t>& getPacketData();

private:
    etl::span<const uint8_t> getPayloadDataImpl() const;

    static constexpr size_t SYNCHRONIZATION_AND_ID_POSITION = 0;
    static constexpr size_t STATUS_OR_COMMAND_POSITION = 1;
    static constexpr size_t ADDRESS_POSITION = 2;
    static constexpr size_t COUNT_POSITION = 6;
    static constexpr size_t DATA_POSITION = 7;

public:
    static constexpr size_t HEADER_SIZE = DATA_POSITION; /**< 1B sync + 1B status + 4B address + 1B count */
    static constexpr size_t MINIMUM_PACKET_SIZE = HEADER_SIZE + 1; /**< header + 1B checksum + 0B data */
    static constexpr size_t MAXIMUM_PACKET_SIZE = 255; /**< header + 1B checksum + 0B data */

private:
    enum class Command : uint8_t
    {
        READ  = 0x80,
        WRITE = 0x81,
    };

    uint8_t getStatusOrCommand() const;
    uint32_t getAddress() const;

    [[nodiscard]] static TCSIPacket createPacket(uint8_t statusOrCommand, uint8_t packetId, uint32_t address, etl::span<const uint8_t> payloadData);
    [[nodiscard]] static uint8_t calculateCheckSum(const etl::span<const uint8_t> packetData);

    static constexpr uint8_t SYNCHRONIZATION_VALUE = 0xA0;
    static constexpr uint8_t SYNCHRONIZATION_MASK  = 0xF0;
    static constexpr uint8_t PACKET_ID_MASK        = 0x0F;

    etl::vector<uint8_t, MAXIMUM_PACKET_SIZE> m_packetData;
};

} // namespace wl

#endif // WL_TCSIPACKET_H
