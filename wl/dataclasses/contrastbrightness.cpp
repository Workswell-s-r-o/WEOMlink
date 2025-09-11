#include "wl/dataclasses/contrastbrightness.h"

#include <algorithm>

namespace wl {

ContrastBrightness::ContrastBrightness(uint16_t contrastRaw, uint16_t brightnessRaw)
    : m_contrast(std::min(contrastRaw, UINT14_MAX_VALUE))
    , m_brightness(std::min(brightnessRaw, UINT14_MAX_VALUE))
{
}

ContrastBrightness::ContrastBrightness(float contrastPercent, float brightnessPercent)
    : m_contrast(std::min(percentToRaw(contrastPercent), UINT14_MAX_VALUE))
    , m_brightness(std::min(percentToRaw(brightnessPercent), UINT14_MAX_VALUE))
{
}

uint16_t ContrastBrightness::getContrastRaw() const
{
    return m_contrast;
}

uint16_t ContrastBrightness::getBrightnessRaw() const
{
    return m_brightness;
}

float ContrastBrightness::getContrastPercent() const
{
    return rawToPercent(m_contrast);
}

float ContrastBrightness::getBrightnessPercent() const
{
    return rawToPercent(m_brightness);
}

void ContrastBrightness::setContrastRaw(uint16_t contrastRaw)
{
    m_contrast = std::max(contrastRaw, UINT14_MAX_VALUE);
}

void ContrastBrightness::setBrightnessRaw(uint16_t brightnessRaw)
{
    m_brightness = std::max(brightnessRaw, UINT14_MAX_VALUE);
}

void ContrastBrightness::setContrastPercent(float contrastPercent)
{
    m_contrast = std::max(percentToRaw(contrastPercent), UINT14_MAX_VALUE);
}

void ContrastBrightness::setBrightnessPercent(float brightnessPercent)
{
    m_brightness = std::max(percentToRaw(brightnessPercent), UINT14_MAX_VALUE);
}

float ContrastBrightness::rawToPercent(uint16_t raw)
{
    return 100.0f * (static_cast<float>(raw) / static_cast<float>(UINT14_MAX_VALUE));
}

uint16_t ContrastBrightness::percentToRaw(float percent)
{
    return static_cast<uint16_t>((percent / 100.0f) * static_cast<float>(UINT14_MAX_VALUE));
}

}
