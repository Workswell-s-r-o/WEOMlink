#include "wl/dataclasses/presetid.h"

namespace wl {


uint16_t Range::getDeviceValue(const Range item)
{
    switch (item)
    {
        case NOT_DEFINED: return 0x0F;
        case R1: return 0x00;
        case R2: return 0x01;
        case R3: return 0x02;
        case HIGH_GAIN: return 0x07;
        case LOW_GAIN: return 0x08;
    }
    assert(false);
    return 0x0F;
}

etl::expected<Range, Error> Range::getFromDeviceValue(const uint16_t deviceValue)
{
    switch (deviceValue)
    {
        case 0x0F: return Range(NOT_DEFINED);
        case 0x00: return Range(R1);
        case 0x01: return Range(R2);
        case 0x02: return Range(R3);
        case 0x07: return Range(HIGH_GAIN);
        case 0x08: return Range(LOW_GAIN);
        default: return etl::unexpected<Error>(Error::INVALID_DATA);
    }
}

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

        default:
            assert(false);
            return 0;
    }
}

uint16_t Lens::getDeviceValue(const Lens item)
{
    switch(item)
    {
        case NOT_DEFINED: return 0xF0;
        case WTC_35: return 0x00;
        case WTC_25: return 0x01;
        case WTC_14: return 0x02;
        case WTC_7_5: return 0x03;
        case USER_1: return 0x07;
        case USER_2: return 0x08;
    }
    assert(false);
    return 0xF0;
}

etl::expected<Lens, Error> Lens::getFromDeviceValue(const uint16_t deviceValue)
{
    switch(deviceValue)
    {
        case 0x0F: return Lens(NOT_DEFINED);
        case 0x00: return Lens(WTC_35);
        case 0x01: return Lens(WTC_25);
        case 0x02: return Lens(WTC_14);
        case 0x03: return Lens(WTC_7_5);
        case 0x07: return Lens(USER_1);
        case 0x08: return Lens(USER_2);
        default: return etl::unexpected<Error>(Error::INVALID_DATA);
    }
}

bool Lens::isUserDefined(Lens item)
{
    switch (item)
    {
        case USER_1:
        case USER_2:
            return true;

        default:
            return false;
    }
}

PresetId::PresetId(Range range, Lens lens)
    : m_range(range)
    , m_lens(lens)
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

} // namespace wl