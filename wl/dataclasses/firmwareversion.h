#ifndef WL_FIRMWAREVERSION_H
#define WL_FIRMWAREVERSION_H

#include <etl/string.h>

#include <cstdint>


namespace wl {

/**
 * @class FirmwareVersion
 * @headerfile firmwareversion.h "wl/dataclasses/firmareversion.h"
 * @brief Represents a firmware version with major, minor, and extended minor version numbers.
 * @see registers_main_firmware_version
 */
class FirmwareVersion
{
public:
    /**
     * @brief Default constructor.
     * Initializes the firmware version to zero for all components.
     */
    explicit FirmwareVersion() = default;

    /**
     * @brief Constructor to initialize the firmware version.
     * @param major Major version number.
     * @param minor Minor version number.
     * @param minor2 Extended minor version number (e.g., build number).
     */
    explicit FirmwareVersion(uint8_t major, uint8_t minor, uint16_t minor2);

    /**
     * @brief Gets the major version number.
     * @return Major version number as an 8-bit unsigned integer.
     */
    uint8_t getMajor() const;

    /**
     * @brief Gets the minor version number.
     * @return Minor version number as an 8-bit unsigned integer.
     */
    uint8_t getMinor() const;

    /**
     * @brief Gets the extended minor version number.
     * @return Extended minor version number (e.g., build number) as a 16-bit unsigned integer.
     */
    uint16_t getMinor2() const;

    /**
     * @brief Maximum size of the string representation of the firmware version.
     * 
     * This includes space for the format "xxx.xxx.xxxxx\0" (14 characters).
     */
    static constexpr size_t MAXIMUM_STRING_SIZE = 14;

    /**
     * @brief Converts the firmware version to a string representation.
     * 
     * The format of the string is "a.b.c", where `a` represents the 
     * major version, `b` represents minor version, and `c` represents the extended minor version.
     * 
     * @return A string representation of the firmware version.
     */
    etl::string<MAXIMUM_STRING_SIZE> toString() const;

private:
    uint8_t m_major;
    uint8_t m_minor;
    uint16_t m_minor2;
};

} // namespace wl

#endif // WL_FIRMWAREVERSION_H