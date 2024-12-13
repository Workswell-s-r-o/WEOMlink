#ifndef WL_IDATALINKINTERFACE_H
#define WL_IDATALINKINTERFACE_H

#include "wl/error.h"
#include "wl/time.h"

#include <etl/span.h>
#include <etl/expected.h>

#include <chrono>


namespace wl {

/**
 * @class IDataLinkInterface
 * @headerfile idatalinkinterface.h "wl/communication/idatalinkinterface.h"
 * @brief Interface for data link communication, providing functionality for managing
 * connections, data transfer, and error handling.
 */
class IDataLinkInterface
{
public:
    virtual ~IDataLinkInterface() {}

    /**
     * @brief Checks if the data link connection is currently open.
     * @return True if the connection is open, false otherwise.
     */
    virtual bool isOpened() const = 0;

    /**
     * @brief Closes the data link connection.
     */
    virtual void closeConnection() = 0;

    /**
     * @brief Gets the maximum data size that can be transferred in a single operation.
     * @return The maximum size of data in bytes.
     */
    virtual size_t getMaxDataSize() const = 0;

    /**
     * @brief Reads data from the data link into a buffer with a specified timeout.
     * @param buffer Span of bytes to store the read data.
     * @param timeout Maximum duration to wait for data to be read.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] virtual etl::expected<void, Error> read(etl::span<uint8_t> buffer, const Clock::duration& timeout) = 0;

    /**
     * @brief Writes data to the data link from a buffer with a specified timeout.
     * @param buffer Span of bytes containing data to write.
     * @param timeout Maximum duration to wait for data to be written.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] virtual etl::expected<void, Error> write(etl::span<const uint8_t> buffer, const Clock::duration& timeout) = 0;

    /**
     * @brief Discards any pending data in the data link, clearing the internal buffer.
     */
    virtual void dropPendingData() = 0;

    /**
     * @brief Checks if the connection has been lost.
     * @return True if the connection is lost, false otherwise.
     */
    virtual bool isConnectionLost() const = 0;
};

} // namespace wl

#endif // WL_IDATALINKINTERFACE_H
