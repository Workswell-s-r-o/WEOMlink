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
    enum enum_type: uint16_t
    {
        NOT_DEFINED, ///< Range not defined
        R1,          ///< Radiometric 1
        R2,          ///< Radiometric 2
        R3,          ///< Radiometric 3
        HIGH_GAIN,   ///< High gain
        LOW_GAIN,    ///< Low gain
        SUPER_GAIN,    ///< Super gain
    };

    /**
     * @brief Gets the device-specific value for the given range.
     * @param item The range item.
     * @return The device-specific value corresponding to the range.
     */
    static uint16_t getDeviceValue(const Range item);

    /**
     * @brief Converts a device-specific value to a range item.
     * @param deviceValue The device-specific value.
     * @return An `etl::expected` containing the corresponding `Range` or an error.
     */
    static etl::expected<Range, Error> getFromDeviceValue(const uint16_t deviceValue);

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

    ETL_DECLARE_ENUM_TYPE(Range, uint16_t)
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
    enum enum_type: uint16_t
    {
        NOT_DEFINED, ///< Lens not defined
        WTC_35,      ///< L-WTC-35-11
        WTC_25,      ///< L-WTC-25-12
        WTC_14,      ///< L-WTC-14-12
        WTC_7_5,     ///< L-WTC-7-12
        WTC_50,
        WTC_7,
        USER_1,      ///< User 1
        USER_2,      ///< User 2
    };

    /**
     * @brief Gets the device-specific value for the given lens.
     * @param item The lens item.
     * @return The device-specific value corresponding to the lens.
     */
    static uint16_t getDeviceValue(const Lens item);

    /**
     * @brief Converts a device-specific value to a lens item.
     * @param deviceValue The device-specific value.
     * @return An `etl::expected` containing the corresponding `Lens` or an error.
     */
    static etl::expected<Lens, Error> getFromDeviceValue(const uint16_t deviceValue);

    /**
     * @brief Checks if the given lens is user-defined.
     * @param item The lens item.
     * @return `true` if the lens is user-defined, `false` otherwise.
     */
    static bool isUserDefined(Lens item);

    ETL_DECLARE_ENUM_TYPE(Lens, uint16_t)
    ETL_ENUM_TYPE(NOT_DEFINED, "NOT_DEFINED")
    ETL_ENUM_TYPE(WTC_35, "WTC_35")
    ETL_ENUM_TYPE(WTC_25, "WTC_25")
    ETL_ENUM_TYPE(WTC_14, "WTC_14")
    ETL_ENUM_TYPE(WTC_7_5, "WTC_7_5")
    ETL_ENUM_TYPE(WTC_50, "WTC_50")
    ETL_ENUM_TYPE(WTC_7, "WTC_7")
    ETL_ENUM_TYPE(USER_1, "USER_1")
    ETL_ENUM_TYPE(USER_2, "USER_2")
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
     * @brief Constructs a `PresetId` instance with specified range and lens.
     * @param range The range to associate with the preset.
     * @param lens The lens to associate with the preset.
     */
    explicit PresetId(Range range, Lens lens);

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

private:
    Range m_range;
    Lens m_lens;
};

} // namespace wl

#endif // WL_PRESETID_H
