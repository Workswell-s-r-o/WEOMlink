#ifndef WL_IDEVICEINTERFACE
#define WL_IDEVICEINTERFACE

#include "wl/communication/addressrange.h"
#include "wl/misc/endian.h"
#include "wl/error.h"

#include <etl/expected.h>

#include <etl/vector.h>
#include <etl/span.h>
#include <etl/array.h>


namespace wl {

/**
 * @class IDeviceInterface
 * @headerfile ideviceinterface.h "wl/communication/ideviceinterface.h"
 * @brief Interface for device communication, providing read and write operations with specified endianness handling.
 */
class IDeviceInterface
{
public:
    /**
     * @brief Enum for specifying device endianness.
     */
    enum class DeviceEndianity
    {
        LITTLE,
        BIG
    };

    /**
     * @brief Constructs the device interface with the specified endianness.
     * @param deviceEndianity The endianness format of the device.
     */
    explicit IDeviceInterface(DeviceEndianity deviceEndianity);
    virtual ~IDeviceInterface();

    /**
     * @brief Reads data from the specified address on the device.
     * @param data Span of bytes to store the read data.
     * @param address The address from which data should be read.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> readData(etl::span<uint8_t> data, uint32_t address) = 0;

    /**
     * @brief Writes data to the specified address on the device.
     * @param data Span of bytes containing data to write.
     * @param address The address to which data should be written.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    [[nodiscard]] virtual etl::expected<void, Error> writeData(etl::span<const uint8_t> data, uint32_t address) = 0;

    /**
     * @brief Reads data from a specified address range on the device.
     * @tparam addressRange The address range to read from, specified as a template parameter.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    template <const AddressRange& addressRange>
    etl::expected<etl::array<uint8_t, addressRange.getSize()>, Error> readAddressRange();

    /**
     * @brief Converts a value from the device's endianness to the host's endianness.
     * @tparam T The type of the value to convert.
     * @param value The value in device endianness.
     * @return The value converted to host endianness.
     */
    template<class T>
    [[nodiscard]] T fromDeviceEndianity(const T& value) const;

    /**
     * @brief Converts a value from the host's endianness to the device's endianness.
     * @tparam T The type of the value to convert.
     * @param value The value in host endianness.
     * @return The value converted to device endianness.
     */
    template<class T>
    [[nodiscard]] T toDeviceEndianity(const T& value) const;

private:
    DeviceEndianity m_deviceEndianity {DeviceEndianity::LITTLE};
};

// Impl

template <const AddressRange& addressRange>
etl::expected<etl::array<uint8_t, addressRange.getSize()>, Error> IDeviceInterface::readAddressRange()
{
    etl::array<uint8_t, addressRange.getSize()> data = {};
    auto result = readData(data, addressRange.getFirstAddress());
    if (!result.has_value())
    {
        return etl::unexpected(result.error());
    }
    return data;
}

template<class T>
T IDeviceInterface::fromDeviceEndianity(const T& value) const
{
    if (m_deviceEndianity == DeviceEndianity::LITTLE)
    {
        return wl::fromLittleEndian(value);
    }
    else
    {
        return wl::fromBigEndian(value);
    }
}

template<class T>
T IDeviceInterface::toDeviceEndianity(const T& value) const
{
    if (m_deviceEndianity == DeviceEndianity::LITTLE)
    {
        return wl::toLittleEndian(value);
    }
    else
    {
        return wl::toBigEndian(value);
    }
}

} // namespace wl

#endif // WL_IDEVICEINTERFACE
