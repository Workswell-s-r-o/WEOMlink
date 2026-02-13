#ifndef WL_PRESETID_H
#define WL_PRESETID_H

#include "wl/error.h"

#include <etl/expected.h>

#include <cstdint>

namespace wl {

/**
 * @struct Range
 * @headerfile presetid.h "wl/dataclasses/presetid.h"
 * @brief Represents possible ranges for the device.
 * @see registers_selected_preset_id, registers_selected_preset_id
 */
struct Range
{
    /**
     * @enum enum_type
     * @brief Enumeration of range types.
     */
    enum enum_type: uint8_t
    {
        NOT_DEFINED = 0xF, ///< Range not defined
        R1          = 0x0, ///< Radiometric 1
        R2          = 0x1, ///< Radiometric 2
        R3          = 0x2, ///< Radiometric 3
        HIGH_GAIN   = 0x7, ///< High gain
        LOW_GAIN    = 0x8, ///< Low gain
        SUPER_GAIN  = 0x9, ///< Super gain
    };

    /**
     * @brief Checks if the given range is radiometric.
     * @param item The range item.
     * @return `true` if the range is radiometric, `false` otherwise.
     */
    static bool isRadiometric(Range item);

    /**
     * @brief Gets the lower temperature limit for the given range.
     * @param item The range item.
     * @return The lower temperature limit.
     */
    static int getLowerTemperature(Range item);

    /**
     * @brief Gets the upper temperature limit for the given range. 
     * @param item The range item.
     * @return The upper temperature limit.
     */
    static int getUpperTemperature(Range item);

    ETL_DECLARE_ENUM_TYPE(Range, uint8_t)
    ETL_ENUM_TYPE(NOT_DEFINED, "NOT_DEFINED")
    ETL_ENUM_TYPE(R1, "R1")
    ETL_ENUM_TYPE(R2, "R2")
    ETL_ENUM_TYPE(R3, "R3")
    ETL_ENUM_TYPE(HIGH_GAIN, "HIGH_GAIN")
    ETL_ENUM_TYPE(LOW_GAIN, "LOW_GAIN")
    ETL_ENUM_TYPE(SUPER_GAIN, "SUPER_GAIN")
    ETL_END_ENUM_TYPE
};

/**
 * @struct Lens
 * @headerfile presetid.h "wl/dataclasses/presetid.h"
 * @brief Represents possible lenses for the device.
 * @see registers_selected_preset_id, registers_selected_preset_id
 */
struct Lens
{
    /**
     * @enum enum_type
     * @brief Enumeration of lens types.
     */
    enum enum_type: uint8_t
    {
        NOT_DEFINED = 0xF, ///< Lens not defined
        WTC_35      = 0x0, ///< 35 mm f/1.10
        WTC_25      = 0x1, ///< 25 mm f/1.20
        WTC_14      = 0x2, ///< 14 mm f/1.20
        WTC_7_5     = 0x3, ///< 7.5 mm f/1.20
        WTC_50      = 0x4, ///< 50 mm f/1.20
        WTC_7       = 0x5, ///< 7 mm f/1.00
        WTC_73      = 0x6, ///< 73.1 mm f/1.15
        WTC_13      = 0x7, ///< 13 mm f/1.00
        WTC_19      = 0x8, ///< 19 mm f/1.00
        WTC_60      = 0x9, ///< 60 mm f/1.00
        WTC_100     = 0xA, ///< 100 mm f/1.00
        WTC_35_105  = 0xB, ///< 35-105 mm f/1.60
        WTC_50_150  = 0xC  ///< 50-150 mm f/1.60
    };

    ETL_DECLARE_ENUM_TYPE(Lens, uint8_t)
    ETL_ENUM_TYPE(NOT_DEFINED, "NOT_DEFINED")
    ETL_ENUM_TYPE(WTC_35, "WTC_35")
    ETL_ENUM_TYPE(WTC_25, "WTC_25")
    ETL_ENUM_TYPE(WTC_14, "WTC_14")
    ETL_ENUM_TYPE(WTC_7_5, "WTC_7_5")
    ETL_ENUM_TYPE(WTC_50, "WTC_50")
    ETL_ENUM_TYPE(WTC_7, "WTC_7")
    ETL_ENUM_TYPE(WTC_73, "WTC_73")
    ETL_ENUM_TYPE(WTC_13, "WTC_13")
    ETL_ENUM_TYPE(WTC_19, "WTC_19")
    ETL_ENUM_TYPE(WTC_60, "WTC_60")
    ETL_ENUM_TYPE(WTC_100, "WTC_100")
    ETL_ENUM_TYPE(WTC_35_105, "WTC_35_105")
    ETL_ENUM_TYPE(WTC_50_150, "WTC_50_150")
    ETL_END_ENUM_TYPE
};

/**
 * @struct LensVariant
 * @headerfile presetid.h "wl/dataclasses/presetid.h"
 * @brief Represents possible lense variants
 * @see registers_selected_preset_id, registers_selected_preset_id
 */
struct LensVariant
{
    /**
     * @enum enum_type
     * @brief Enumeration of lens variants.
     */
    enum enum_type: uint8_t
    {
        NOT_DEFINED = 0xF, ///< Variant not defined
        A           = 0x0, ///< Variant A
        B           = 0x1, ///< Variant B
        C           = 0x2  ///< Variant C
    };

    ETL_DECLARE_ENUM_TYPE(LensVariant, uint8_t)
    ETL_ENUM_TYPE(NOT_DEFINED, "NOT_DEFINED")
    ETL_ENUM_TYPE(A, "A")
    ETL_ENUM_TYPE(B, "B")
    ETL_ENUM_TYPE(C, "C")
    ETL_END_ENUM_TYPE
};

/**
 * @struct PresetVersion
 * @headerfile presetid.h "wl/dataclasses/presetid.h"
 * @brief Represents possible preset versions
 * @see registers_selected_preset_id, registers_selected_preset_id
 */
struct PresetVersion
{
    /**
     * @enum enum_type
     * @brief Enumeration of preset versions
     */
    enum enum_type: uint8_t
    {
        NOT_DEFINED = 0xF, ///< Version not defined
        ONUC        = 0x0, ///< ONUC version
        SNUC        = 0x1  ///< SNUC version
    };

    ETL_DECLARE_ENUM_TYPE(PresetVersion, uint8_t)
    ETL_ENUM_TYPE(NOT_DEFINED, "NOT_DEFINED")
    ETL_ENUM_TYPE(ONUC, "ONUC")
    ETL_ENUM_TYPE(SNUC, "SNUC")
    ETL_END_ENUM_TYPE
};

/**
 * @class PresetId
 * @headerfile presetid.h "wl/dataclasses/presetid.h"
 * @brief Represents a unique combination of range and lens settings.
 * @see registers_selected_preset_id, registers_selected_preset_id
 */
class PresetId
{
public:
/**
     * @brief Default constructor.
     */
    explicit PresetId() = default;

    /**
     * @brief Constructs a `PresetId` instance with specified range, lens, version and lens variant.
     * @param range The range to associate with the preset.
     * @param lens The lens to associate with the preset.
     * @param lensVariant The lens variant to associate with the preset.
     * @param presetVersion The preset version to associate with the preset.
     * @see registers_selected_preset_id
     */
    explicit PresetId(Range range, Lens lens, PresetVersion presetVersion, LensVariant lensVariant);

        /**
     * @brief Constructs a `PresetId` instance from device value
     * @param deviceValue The value read from the device
     * @see registers_selected_preset_id
     */
    explicit PresetId(uint32_t deviceValue);

    /**
     * @brief Gets the range associated with the preset.
     * @return The current range of the preset.
     */
    Range getRange() const;

    /**
     * @brief Sets the range for the preset.
     * @param range The new range to set.
     */
    void setRange(Range range);

    /**
     * @brief Gets the lens associated with the preset.
     * @return The current lens of the preset.
     */
    Lens getLens() const;

    /**
     * @brief Sets the lens for the preset.
     * @param lens The new lens to set.
     */
    void setLens(Lens lens);

    /**
     * @brief Gets the preset version associated with the preset.
     * @return The current preset version of the preset.
     */
    PresetVersion getPresetVersion() const;

    /**
     * @brief Sets the preset version for the preset.
     * @param presetVersion The new preset version to set.
     */
    void setPresetVersion(PresetVersion presetVersion);
    /**
     * @brief Gets the lens variant associated with the preset.
     * @return The current lens variant of the preset.
     */
    LensVariant getLensVariant() const;

    /**
     * @brief Sets the lens variant for the preset.
     * @param lensVariant The new lens variant to set.
     */
    void setLensVariant(LensVariant lensVariant);

    /**
     * @brief Converts the preset ID to a device value.
     * @return The device value representing the preset ID.
     */
    uint32_t toDeviceValue() const;

private:
    Range m_range;
    Lens m_lens;
    PresetVersion m_presetVersion;
    LensVariant m_lensVariant;

    static constexpr uint32_t RANGE_MASK          = 0xF000'0000;
    static constexpr uint32_t RANGE_SHIFT         = 28;
    static constexpr uint32_t LENS_MASK           = 0x0FF0'0000;
    static constexpr uint32_t LENS_SHIFT          = 24;
    static constexpr uint32_t VERSION_MASK        = 0x000F'0000;
    static constexpr uint32_t VERSION_SHIFT       = 16;
    static constexpr uint32_t LENS_VARIANT_MASK   = 0x0000'00FF;
    static constexpr uint32_t LENS_VARIANT_SHIFT  = 0;
};

} // namespace wl

#endif // WL_PRESETID_H
