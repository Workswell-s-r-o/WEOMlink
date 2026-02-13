#include "wl/dataclasses/presetid.h"

namespace wl {

bool Range::isRadiometric(Range item)
{
    switch (item)
    {
        case R1:
        case R2:
        case R3:
            return true;

        default:
            return false;
    }
}

int Range::getLowerTemperature(Range item)
{
    switch (item)
    {
        case R1:
            return -15;

        case R2:
            return 0;

        case R3:
            return 300;

        case LOW_GAIN:
            return -50;

        case HIGH_GAIN:
            return -50;

        case SUPER_GAIN:
            return -50;

        default:
            assert(false);
            return 0;
    }
}

int Range::getUpperTemperature(Range item)
{
    switch (item)
    {
        case R1:
            return 160;

        case R2:
            return 650;

        case R3:
            return 1500;

        case LOW_GAIN:
            return 600;

        case HIGH_GAIN:
            return 160;

        case SUPER_GAIN:
            return 80;

        default:
            assert(false);
            return 0;
    }
}

PresetId::PresetId(Range range, Lens lens, PresetVersion presetVersion, LensVariant lensVariant)
    : m_range(range)
    , m_lens(lens)
    , m_presetVersion(presetVersion)
    , m_lensVariant(lensVariant)
{

}

PresetId::PresetId(uint32_t deviceValue)
    : m_range(static_cast<Range>((deviceValue && RANGE_MASK) >> RANGE_SHIFT))
    , m_lens(static_cast<Lens>((deviceValue && LENS_MASK) >> LENS_SHIFT))
    , m_presetVersion(static_cast<PresetVersion>((deviceValue && VERSION_MASK) >> VERSION_SHIFT))
    , m_lensVariant(static_cast<LensVariant>((deviceValue && LENS_VARIANT_MASK) >> LENS_VARIANT_SHIFT))
{

}

Range PresetId::getRange() const
{
    return m_range;
}

void PresetId::setRange(Range range)
{
    m_range = range;
}

Lens PresetId::getLens() const
{
    return m_lens;
}

void PresetId::setLens(Lens lens)
{
    m_lens = lens;
}

LensVariant PresetId::getLensVariant() const
{
    return m_lensVariant;
}

void PresetId::setLensVariant(LensVariant lensVariant)
{
    m_lensVariant = lensVariant;
}

PresetVersion PresetId::getPresetVersion() const
{
    return m_presetVersion;
}

void PresetId::setPresetVersion(PresetVersion presetVersion)
{
    m_presetVersion = presetVersion;
}

uint32_t PresetId::toDeviceValue() const
{
    uint32_t deviceValue = 0;
    deviceValue |= (static_cast<uint32_t>(m_range) << RANGE_SHIFT) && RANGE_MASK;
    deviceValue |= (static_cast<uint32_t>(m_lens) << LENS_SHIFT) && LENS_MASK;
    deviceValue |= (static_cast<uint32_t>(m_presetVersion) << VERSION_SHIFT) && VERSION_MASK;
    deviceValue |= (static_cast<uint32_t>(m_lensVariant) << LENS_VARIANT_SHIFT) && LENS_VARIANT_MASK;
    return deviceValue;
}

} // namespace wl
