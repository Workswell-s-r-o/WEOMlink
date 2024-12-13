#ifndef WL_IPROTOCOLINTERFACE_H
#define WL_IPROTOCOLINTERFACE_H

#include "wl/error.h"

#include <etl/span.h>
#include <etl/expected.h>

#include <chrono>


namespace wl {

/**
 * @class IProtocolInterface
 * @headerfile iprotocolinterface.h "wl/communication/iprotocolinterface.h"
 * @brief Interface class for communication protocols, defining methods for data transfer.
 * 
 * This abstract class provides a common interface for protocol implementations, enabling data
 * reading and writing functionality with specified timeout handling.
 */
class IProtocolInterface
{
public:
    virtual ~IProtocolInterface() {}

    /**
     * @brief Gets the maximum supported data size for this protocol.
     * @return The maximum data size, in bytes, that can be handled by the protocol implementation.
     */
    virtual uint32_t getMaxDataSize() const = 0;

    /**
     * @brief Reads data from a specified address with a timeout.
     * 
     * This function attempts to read data into the provided buffer from the specified address
     * within a given timeout period. It returns either success or an error state.
     * 
     * @param data A span representing the buffer where the read data will be stored.
     * @param address The memory address or register to read from.
     * @param timeout The maximum duration to wait for the read operation to complete.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> readData(etl::span<uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout) = 0;
    
    /**
     * @brief Writes data to a specified address with a timeout.
     * 
     * This function attempts to write data from the provided buffer to the specified address
     * within a given timeout period. It returns either success or an error state.
     * 
     * @param data A span representing the buffer of data to be written.
     * @param address The memory address or register to write to.
     * @param timeout The maximum duration to wait for the write operation to complete.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> writeData(const etl::span<const uint8_t> data, uint32_t address, const std::chrono::steady_clock::duration& timeout) = 0;
};

} // namespace wl

#endif // WL_IPROTOCOLINTERFACE_H
