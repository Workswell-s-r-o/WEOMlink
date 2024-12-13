#include "wl/dataclasses/firmwareversion.h"

#include <cstdio>

namespace wl {

FirmwareVersion::FirmwareVersion(uint8_t major, uint8_t minor, uint16_t minor2)
    : m_major(major)
    , m_minor(minor)
    , m_minor2(minor2)
{

}

uint8_t FirmwareVersion::getMajor() const
{
    return m_major;
}

uint8_t FirmwareVersion::getMinor() const
{
    return m_minor;
}

uint16_t FirmwareVersion::getMinor2() const
{
    return m_minor2;
}

etl::string<FirmwareVersion::MAXIMUM_STRING_SIZE> FirmwareVersion::toString() const
{
    etl::string<FirmwareVersion::MAXIMUM_STRING_SIZE> string;
    std::sprintf(string.data(), "%d.%d.%d", m_major, m_minor, m_minor2);
    return string;
}

} // namespace wl
