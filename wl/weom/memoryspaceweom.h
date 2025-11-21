#ifndef WL_MEMORYSPACEWEOM_H
#define WL_MEMORYSPACEWEOM_H

#include "wl/communication/addressrange.h"
#include "wl/error.h"

#include <etl/expected.h>
#include <etl/vector.h>

namespace wl
{

    /**
     * @brief Enumeration representing different types of memory for WEOM devices.
     */
    enum class MemoryTypeWEOM
    {
        REGISTERS_CONFIGURATION = 1 << 0,
        FLASH_MEMORY = 1 << 1,
    };

    /**
     * @class MemoryDescriptorWEOM
     * @headerfile memoryspaceweom.h wl/weom/memoryspaceweom.h
     * @brief Describes a specific memory segment, including address range, type, and data size constraints.
     */
    struct MemoryDescriptorWEOM
    {
        AddressRange addressRange; /**< The address range of the memory segment. */

        MemoryTypeWEOM type{MemoryTypeWEOM::REGISTERS_CONFIGURATION}; /**< The type of memory. */

        uint32_t minimumDataSize{0}; /**< Minimum data size for operations in this memory segment. */
        uint32_t maximumDataSize{0}; /**< Maximum data size for operations in this memory segment. */

        /**
         * @brief Default strong ordering operator for memory descriptors.
         */
        std::strong_ordering operator<=>(const MemoryDescriptorWEOM &other) const = default;

        /**
         * @brief Constructs a memory descriptor with a specified address range and type.
         * @param addressRange The address range for the memory segment.
         * @param type The type of memory.
         */
        MemoryDescriptorWEOM(const AddressRange &addressRange, MemoryTypeWEOM type);

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
     * @brief Defines the memory space for a WEOM device, containing memory descriptors and address ranges of registers.
     * @see registers
     */
    class MemorySpaceWEOM
    {
        MemorySpaceWEOM(const etl::ivector<MemoryDescriptorWEOM> &memoryDescriptors);

    public:
        /**
         * @brief Retrieves a memory descriptor for a specified address range.
         * @param addressRange The address range to look up.
         * @return An `etl::expected<void, Error>` indicating success or error.
         */
        etl::expected<MemoryDescriptorWEOM, Error> getMemoryDescriptor(const AddressRange &addressRange) const;

        /**
         * @brief Gets all memory descriptors in the memory space.
         * @return A constant reference to a vector of memory descriptors.
         */
        const etl::ivector<MemoryDescriptorWEOM> &getMemoryDescriptors() const;

        /**
         * @brief Retrieves the device's complete memory space configuration.
         * @return A MemorySpaceWEOM object representing the device's memory layout.
         */
        static MemorySpaceWEOM getDeviceSpace();

        static constexpr AddressRange CONFIGURATION_REGISTERS = AddressRange::firstToLast(0x00000000, 0x300040FF); ///< Address range of configuration registers
        static constexpr AddressRange FLASH_MEMORY = AddressRange::firstToLast(0xD0000000, 0xDFFFFFFF);            ///< Address range of flash memory
        static constexpr uint32_t ADDRESS_FLASH_REGISTERS_START = FLASH_MEMORY.getFirstAddress() + 0x00800000;     ///< Starting address of flash registers

        // Control - 0x00xx
        /**
         * @brief Address range of device identificator register
         * @see registers_device_identificator
         */
        static constexpr AddressRange DEVICE_IDENTIFICATOR = AddressRange::firstAndSize(0x0000, 4);

        /**
         * @brief Address range of trigger register
         * @see registers_trigger
         */
        static constexpr AddressRange TRIGGER = AddressRange::firstAndSize(0x0004, 4);

        /**
         * @brief Address range of status register
         * @see registers_status
         */
        static constexpr AddressRange STATUS = AddressRange::firstAndSize(0x000C, 4);

        // General - 0x01xx
        /**
         * @brief Address range of firmware version
         * @see registers_main_firmware_version
         */
        static constexpr AddressRange MAIN_FIRMWARE_VERSION = AddressRange::firstAndSize(0x0100, 4); ///< 

        /**
         * @brief Address range of shutter temperature register
         * @see registers_shutter_temperature
         */
        static constexpr AddressRange SHUTTER_TEMPERATURE = AddressRange::firstAndSize(0x0110, 4); ///< 

        /**
         * @brief Address range of serial number registers
         * @see registers_serial_number
         */
        static constexpr AddressRange SERIAL_NUMBER_CURRENT = AddressRange::firstAndSize(0x0114, 32);

        /**
         * @brief Address range of article number registers
         * @see registers_article_number
         */
        static constexpr AddressRange ARTICLE_NUMBER_CURRENT = AddressRange::firstAndSize(0x0134, 32);

        // Video - 0x02xx
        /**
         * @brief Address range of palette index register
         * @see registers_palette_index
         */
        static constexpr AddressRange PALETTE_INDEX_CURRENT = AddressRange::firstAndSize(0x0200, 4);

        /**
         * @brief Address range of framerate register
         * @see registers_frame_rate
         */
        static constexpr AddressRange FRAME_RATE_CURRENT = AddressRange::firstAndSize(0x0204, 4);

        /**
         * @brief Address range of image flip register
         * @see registers_image_flip
         */
        static constexpr AddressRange IMAGE_FLIP_CURRENT = AddressRange::firstAndSize(0x0208, 4);

        /**
         * @brief Address range of video format register
         * @see registers_video_format
         */
        static constexpr AddressRange VIDEO_FORMAT = AddressRange::firstAndSize(0x0210, 4);

        /**
         * @brief Address range of image freeze register
         * @see registers_image_freeze
         */
        static constexpr AddressRange IMAGE_FREEZE = AddressRange::firstAndSize(0x020C, 4);

        /**
         * @brief Address range of test pattern register
         * @see registers_test_pattern
         */
        static constexpr AddressRange TEST_PATTERN = AddressRange::firstAndSize(0x0214, 4);

        // NUC - 0x03xx
        /**
         * @brief Address range of nuc update mode register
         * @see registers_nuc_update_mode
         */
        static constexpr AddressRange NUC_UPDATE_MODE_CURRENT = AddressRange::firstAndSize(0x0308, 4);

        /**
         * @brief Address range of nuc max period register
         * @see registers_nuc_max_period
         */
        static constexpr AddressRange NUC_MAX_PERIOD_CURRENT = AddressRange::firstAndSize(0x0320, 4);

        /**
         * @brief Address range of nuc adaptive threshold register
         * @see registers_nuc_adaptive_threshold
         */
        static constexpr AddressRange NUC_ADAPTIVE_THRESHOLD_CURRENT = AddressRange::firstAndSize(0x0324, 4);

        // Filters - 0x06xx
        /**
         * @brief Address range of time domain average register
         * @see registers_time_domain_average
         */
        static constexpr AddressRange TIME_DOMAIN_AVERAGE_CURRENT = AddressRange::firstAndSize(0x0600, 4);

        /**
         * @brief Address range of image equalization type register
         * @see registers_image_equalization_type
         */
        static constexpr AddressRange IMAGE_EQUALIZATION_TYPE_CURRENT = AddressRange::firstAndSize(0x0604, 4);

        /**
         * @brief Address range of MGC contrast brightness register
         * @see registers_mgc_contrast_brightness
         */
        static constexpr AddressRange MGC_CONTRAST_BRIGHTNESS_CURRENT = AddressRange::firstAndSize(0x0608, 4);

        /**
         * @brief Address range of AGC NH smoothing register
         * @see registers_agc_nh_smoothing
         */
        static constexpr AddressRange AGC_NH_SMOOTHING_CURRENT = AddressRange::firstAndSize(0x0610, 4);

        /**
         * @brief Address range of spatial median filter enable register
         * @see registers_spatial_median_filter_enable
         */
        static constexpr AddressRange SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT = AddressRange::firstAndSize(0x0614, 4);

        /**
         * @brief Address range of linear gain weight register
         * @see registers_linear_gain_weight
         */
        static constexpr AddressRange LINEAR_GAIN_WEIGHT = AddressRange::firstAndSize(0x0620, 4);

        /**
         * @brief Address range of clip limit register
         * @see registers_clip_limit
         */
        static constexpr AddressRange CLIP_LIMIT = AddressRange::firstAndSize(0x0624, 4);

        /**
         * @brief Address range of plateau tail rejection register
         * @see registers_plateau_tail_rejection
         */
        static constexpr AddressRange PLATEAU_TAIL_REJECTION = AddressRange::firstAndSize(0x0628, 4);

        // Presets - 0x0Axx
        /**
         * @brief Address range of selected preset index register
         * @see registers_selected_preset_index
         */
        static constexpr AddressRange SELECTED_PRESET_INDEX = AddressRange::firstAndSize(0x0A00, 4);

        /**
         * @brief Address range of current preset index register
         * @see registers_current_preset_index
         */
        static constexpr AddressRange CURRENT_PRESET_INDEX = AddressRange::firstAndSize(0x0A04, 4);

        /**
         * @brief Address range of selected attribute and preset index register
         * @see registers_selected_attribute_and_preset_index
         */
        static constexpr AddressRange SELECTED_ATTRIBUTE_AND_PRESET_INDEX = AddressRange::firstAndSize(0x0A08, 4);

        /**
         * @brief Address range of attribute address register
         * @see registers_attribute_address
         */
        static constexpr AddressRange ATTRIBUTE_ADDRESS = AddressRange::firstAndSize(0x0A0C, 4);

        /**
         * @brief Address range of number of presets and attributes register
         * @see registers_number_of_presets_and_attributes
         */
        static constexpr AddressRange NUMBER_OF_PRESETS_AND_ATTRIBUTES = AddressRange::firstAndSize(0x0A10, 8);

        /**
         * @brief Address range of selected preset ID register
         * @see registers_selected_preset_id
         */
        static constexpr AddressRange SELECTED_PRESET_ID = AddressRange::firstAndSize(0x0A14, 4);
        /**
         * @brief Address range of current preset ID register
         * @see registers_current_preset_id
         */
        static constexpr AddressRange CURRENT_PRESET_ID = AddressRange::firstAndSize(0x0A18, 4);

        // Palettes data
        static constexpr unsigned PALETTES_FACTORY_MAX_COUNT = 14;
        static constexpr unsigned PALETTES_USER_MAX_COUNT = 2;
        static constexpr uint32_t PALETTE_NAME_SIZE = 16;
        static constexpr AddressRange PALETTES_REGISTERS = AddressRange::firstToLast(0x30000000, 0x300040FF);

        static constexpr AddressRange getPaletteNameAddressRange(unsigned paletteIndex);

    private:
        etl::vector<MemoryDescriptorWEOM, 10> m_memoryDescriptors;
    };

    constexpr AddressRange MemorySpaceWEOM::getPaletteNameAddressRange(unsigned paletteIndex)
    {
        assert(paletteIndex < (PALETTES_FACTORY_MAX_COUNT + PALETTES_USER_MAX_COUNT));
        return AddressRange::firstAndSize(PALETTES_REGISTERS.getFirstAddress() + 0x4000 + paletteIndex * PALETTE_NAME_SIZE, PALETTE_NAME_SIZE);
    }

} // namespace wl

#endif // WL_MEMORYSPACEWEOM_H
