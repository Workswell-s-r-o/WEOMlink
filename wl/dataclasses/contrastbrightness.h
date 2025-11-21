#ifndef WL_CONTRASTBRIGHTNESS_H
#define WL_CONTRASTBRIGHTNESS_H

#include <cstdint>

namespace wl {

/**
 * @class ContrastBrightness
 * @headerfile contrastbrightness.h "wl/dataclasses/contrastbrightness.h"
 * @brief A utility class to manage and convert contrast and brightness values 
 *        between raw 14bit values and percentages. 
 * @see registers_mgc_contrast_brightness
 */
class ContrastBrightness
{
public:
    /**
     * @brief Default constructor.
     * Initializes contrast and brightness values to zero.
     */
    explicit ContrastBrightness() = default;

    /**
     * @brief Constructor to initialize with raw contrast and brightness values.
     * @param contrastRaw Raw contrast value (0 to `UINT14_MAX_VALUE`).
     * @param brightnessRaw Raw brightness value (0 to `UINT14_MAX_VALUE`).
     */
    explicit ContrastBrightness(uint16_t contrastRaw, uint16_t brightnessRaw);

    /**
     * @brief Constructor to initialize with percentage contrast and brightness values.
     * @param contrastPercent Contrast value in percentage (0.0 to 100.0).
     * @param brightnessPercent Brightness value in percentage (0.0 to 100.0).
     */
    explicit ContrastBrightness(float contrastPercent, float brightnessPercent);

    /**
     * @brief Gets the raw contrast value.
     * @return Raw contrast value (0 to `UINT14_MAX_VALUE`).
     */
    uint16_t getContrastRaw() const;

    /**
     * @brief Gets the raw brightness value.
     * @return Raw brightness value (0 to `UINT14_MAX_VALUE`).
     */
    uint16_t getBrightnessRaw() const;

    /**
     * @brief Gets the contrast value in percentage.
     * @return Contrast percentage (0.0 to 100.0).
     */
    float getContrastPercent() const;

    /**
     * @brief Gets the brightness value in percentage.
     * @return Brightness percentage (0.0 to 100.0).
     */
    float getBrightnessPercent() const;


    /**
     * @brief Sets the raw contrast value.
     * @param contrastRaw Raw contrast value (0 to `UINT14_MAX_VALUE`).
     */
    void setContrastRaw(uint16_t contrastRaw);

    /**
     * @brief Sets the raw brightness value.
     * @param brightnessRaw Raw brightness value (0 to `UINT14_MAX_VALUE`).
     */
    void setBrightnessRaw(uint16_t brightnessRaw);


    /**
     * @brief Sets the contrast value in percentage.
     * @param contrastPercent Contrast percentage (0.0 to 100.0).
     */
    void setContrastPercent(float contrastPercent);

    /**
     * @brief Sets the brightness value in percentage.
     * @param brightnessPercent Brightness percentage (0.0 to 100.0).
     */
    void setBrightnessPercent(float brightnessPercent);

private:
    static constexpr uint16_t UINT14_MAX_VALUE = (1 << 14) - 1;
    static float rawToPercent(uint16_t raw);
    static uint16_t percentToRaw(float percent);

    uint16_t m_contrast;
    uint16_t m_brightness;
};

}

#endif // WL_CONTRASTBRIGHTNESS_H