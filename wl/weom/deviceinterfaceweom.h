#ifndef WL_DEVICEINTERFACEWEOM_H
#define WL_DEVICEINTERFACEWEOM_H

#include "wl/communication/ideviceinterface.h"
#include "wl/weom/memoryspaceweom.h"
#include "wl/error.h"
#include "wl/time.h"

#include <etl/mutex.h>
#include <etl/expected.h>
#include <etl/span.h>
#include <etl/optional.h>

#include <bitset>


namespace wl {

class ProtocolInterfaceTCSI;

/**
 * @class DeviceInterfaceWEOM
 * @headerfile deviceinterfaceweom.h "wl/weom/deviceinterfaceweom.h"
 * @brief Specialized device interface for WEOM devices, handling data read/write with error management.
 */
class DeviceInterfaceWEOM : public IDeviceInterface
{
    using BaseClass = IDeviceInterface;

public:
    /**
     * @brief Constructs a DeviceInterfaceWEOM with a specified protocol interface.
     * @param protocolInterface Unique pointer to the ProtocolInterfaceTCSI implementation.
     * @param sleepFunction User-defined function to handle delays, taking a duration as input.
     */
    explicit DeviceInterfaceWEOM(etl::unique_ptr<ProtocolInterfaceTCSI> protocolInterface, SleepFunction sleepFunction);

    /**
     * @brief Retrieves the memory space associated with this device.
     * @return A constant reference to the MemorySpaceWEOM object.
     */
    const MemorySpaceWEOM& getMemorySpace() const;

    /**
     * @brief Sets the memory space for this device.
     * @param memorySpace The MemorySpaceWEOM object to associate with this device.
     */
    void setMemorySpace(const MemorySpaceWEOM& memorySpace);


    /**
     * @brief Reads data from the specified address.
     * @param data Span of bytes to store the read data.
     * @param address The address from which to read data.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> readData(etl::span<uint8_t> data, uint32_t address) override;

    /**
     * @brief Writes data to the specified address.
     * @param data Span of bytes containing data to write.
     * @param address The address to which data should be written.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> writeData(const etl::span<const uint8_t> data, uint32_t address) override;


private:
    using Duration = std::chrono::steady_clock::duration;
    using ErrorWindow = std::bitset<8>;
    static constexpr size_t MAX_ERRORS_IN_WINDOW = 4;

    [[nodiscard]] etl::expected<void, Error> writeDataImpl(const etl::span<const uint8_t> data, uint32_t address, const Duration& expectedOperationDuration,
                                           const uint32_t maxDataSize, Duration& busyDelayTotal, ErrorWindow& lastErrors);
    [[nodiscard]] etl::expected<void, Error> readDataImpl(etl::span<uint8_t> data, uint32_t address, uint32_t maxDataSize);

    [[nodiscard]] etl::expected<void, Error> handleErrorResponse(etl::expected<void, Error> operationResult, ErrorWindow& lastErrors, Duration& busyDelayTotal);
    [[nodiscard]] etl::expected<MemoryDescriptorWEOM, Error> getMemoryDescriptorWithChecks(uint32_t address, etl::optional<size_t> dataSize) const;
    uint32_t getMaxDataSize(const MemoryDescriptorWEOM& memoryDescriptor) const;

    static constexpr Duration TIMEOUT_DEFAULT = std::chrono::milliseconds(1'000);

    static constexpr Duration BUSY_DEVICE_DELAY = std::chrono::milliseconds(500);
    static constexpr Duration BUSY_DEVICE_TIMEOUT = std::chrono::milliseconds(10'000);

    etl::unique_ptr<ProtocolInterfaceTCSI> m_protocolInterface;

    MemorySpaceWEOM m_memorySpace;
    SleepFunction m_sleepFunction;
};

} // namespace wl

#endif // WL_DEVICEINTERFACEWEOM_H
