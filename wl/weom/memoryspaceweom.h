#ifndef WL_MEMORYSPACEWEOM_H
#define WL_MEMORYSPACEWEOM_H

#include "wl/communication/addressrange.h"
#include "wl/error.h"

#include <etl/expected.h>
#include <etl/vector.h>


namespace wl {

/**
 * @brief Enumeration representing different types of memory for WEOM devices.
 */
enum class MemoryTypeWEOM
{
    REGISTERS_CONFIGURATION = 1 << 0,
    FLASH_MEMORY            = 1 << 1,
};

/**
 * @class MemoryDescriptorWEOM
 * @headerfile memoryspaceweom.h wl/weom/memoryspaceweom.h
 * @brief Describes a specific memory segment, including address range, type, and data size constraints.
 */
struct MemoryDescriptorWEOM
{
    AddressRange addressRange; /**< The address range of the memory segment. */

    MemoryTypeWEOM type {MemoryTypeWEOM::REGISTERS_CONFIGURATION}; /**< The type of memory. */


    uint32_t minimumDataSize {0}; /**< Minimum data size for operations in this memory segment. */
    uint32_t maximumDataSize {0}; /**< Maximum data size for operations in this memory segment. */

    /**
     * @brief Default strong ordering operator for memory descriptors.
     */
    std::strong_ordering operator<=>(const MemoryDescriptorWEOM& other) const = default;

    /**
     * @brief Constructs a memory descriptor with a specified address range and type.
     * @param addressRange The address range for the memory segment.
     * @param type The type of memory.
     */
    MemoryDescriptorWEOM(const AddressRange& addressRange, MemoryTypeWEOM type);

    /**
     * @brief Gets the minimum data size for a specified memory type.
     * @param type The memory type.
     * @return The minimum data size in bytes.
     */
    static uint32_t getMinimumDataSize(MemoryTypeWEOM type);

    /**
     * @brief Gets the maximum data size for a specified memory type.
     * @param type The memory type.
     * @return The maximum data size in bytes.
     */
    static uint32_t getMaximumDataSize(MemoryTypeWEOM type);
};

/**
 * @class MemorySpaceWEOM
 * @headerfile memoryspaceweom.h wl/weom/memoryspaceweom.h
 * @brief Defines the memory space for a WEOM device, containing memory descriptors and address ranges.
 */
class MemorySpaceWEOM
{
    MemorySpaceWEOM(const etl::ivector<MemoryDescriptorWEOM>& memoryDescriptors);

public:
    /**
     * @brief Retrieves a memory descriptor for a specified address range.
     * @param addressRange The address range to look up.
     * @return An `etl::expected<void, Error>` indicating success or error.
     */
    etl::expected<MemoryDescriptorWEOM, Error> getMemoryDescriptor(const AddressRange& addressRange) const;

    /**
     * @brief Gets all memory descriptors in the memory space.
     * @return A constant reference to a vector of memory descriptors.
     */
    const etl::ivector<MemoryDescriptorWEOM>& getMemoryDescriptors() const;

    /**
     * @brief Retrieves the device's complete memory space configuration.
     * @return A MemorySpaceWEOM object representing the device's memory layout.
     */
    static MemorySpaceWEOM getDeviceSpace();

    static constexpr AddressRange CONFIGURATION_REGISTERS              = AddressRange::firstToLast(0x00000000, 0x00000FFF); ///< Address range of configuration registers
    static constexpr AddressRange FLASH_MEMORY                         = AddressRange::firstToLast(0xD0000000, 0xDFFFFFFF);
    static constexpr uint32_t ADDRESS_FLASH_REGISTERS_START            = FLASH_MEMORY.getFirstAddress() + 0x00800000;

    // Control - 0x00xx
    static constexpr AddressRange DEVICE_IDENTIFICATOR                 = AddressRange::firstAndSize(0x0000, 4); ///< Address range of device identificator
    static constexpr AddressRange TRIGGER                              = AddressRange::firstAndSize(0x0004, 4); ///< Address range of trigger register
    static constexpr AddressRange STATUS                               = AddressRange::firstAndSize(0x000C, 4); ///< Address range of status register

    // General - 0x01xx
    static constexpr AddressRange MAIN_FIRMWARE_VERSION                = AddressRange::firstAndSize(0x0100, 4);  ///< Address range of firmware version
    static constexpr AddressRange SERIAL_NUMBER_CURRENT                = AddressRange::firstAndSize(0x0114, 32); ///< Address range of serial number
    static constexpr AddressRange ARTICLE_NUMBER_CURRENT               = AddressRange::firstAndSize(0x0134, 32); ///< Address range of article number

    // Video - 0x02xx
    static constexpr AddressRange PALETTE_INDEX_CURRENT                = AddressRange::firstAndSize(0x0200, 4); ///< Address range of palette index register
    static constexpr AddressRange FRAME_RATE_CURRENT                   = AddressRange::firstAndSize(0x0204, 4); ///< Address range of framerate register
    static constexpr AddressRange IMAGE_FLIP_CURRENT                   = AddressRange::firstAndSize(0x0208, 4); ///< Address range of image flip register
    static constexpr AddressRange IMAGE_FREEZE                         = AddressRange::firstAndSize(0x020C, 4); ///< Address range of image freeze register
    static constexpr AddressRange TEST_PATTERN                         = AddressRange::firstAndSize(0x0214, 4); ///< Address range of test pattern register

    // NUC - 0x03xx
    static constexpr AddressRange NUC_UPDATE_MODE_CURRENT              = AddressRange::firstAndSize(0x0308, 4); ///< Address range of nuc update mode register
    static constexpr AddressRange NUC_MAX_PERIOD_CURRENT               = AddressRange::firstAndSize(0x0320, 4);
    static constexpr AddressRange NUC_ADAPTIVE_THRESHOLD_CURRENT       = AddressRange::firstAndSize(0x0324, 4);

    // Filters - 0x06xx
    static constexpr AddressRange TIME_DOMAIN_AVERAGE_CURRENT          = AddressRange::firstAndSize(0x0600, 4); ///< Address range of time domain average register
    static constexpr AddressRange IMAGE_EQUALIZATION_TYPE_CURRENT      = AddressRange::firstAndSize(0x0604, 4); ///< Address range of image equalization type register
    static constexpr AddressRange MGC_CONTRAST_BRIGHTNESS_CURRENT      = AddressRange::firstAndSize(0x0608, 4); ///< Address range of MGC contrast brightness register
    static constexpr AddressRange AGC_NH_SMOOTHING_CURRENT             = AddressRange::firstAndSize(0x0610, 4); ///< Address range of AGC NH smoothing register
    static constexpr AddressRange SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT = AddressRange::firstAndSize(0x0614, 4); ///< Address range of spatial median filter enable register

    // Presets - 0x0Axx
    static constexpr AddressRange SELECTED_PRESET_ID                  = AddressRange::firstAndSize(0x0A14, 4); ///< Address range of selected preset ID
    static constexpr AddressRange CURRENT_PRESET_ID                   = AddressRange::firstAndSize(0x0A18, 4); ///< Address range of current preset ID

private:
    etl::vector<MemoryDescriptorWEOM, 10> m_memoryDescriptors;
};

} // namespace wl

#endif // WL_MEMORYSPACEWEOM_H
