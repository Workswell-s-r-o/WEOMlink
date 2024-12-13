#include "wl/dataclasses/status.h"

namespace wl {

Status::Status(uint32_t value)
    : m_value(value)
{

}

bool Status::isNucActive() const
{
    return (m_value & (1 << 0)) != 0;
}

bool Status::isCameraNotReady() const
{
    return (m_value & (1 << 1)) != 0;
}

bool Status::isValidTfpa() const
{
    return (m_value & (1 << 2)) != 0;
}

DeviceType Status::getDeviceType() const
{
    return static_cast<DeviceType>((m_value >> 3) & 0b11);
}

bool Status::isMotorfocusBusy() const
{
    return (m_value & (1 << 5)) != 0;
}

bool Status::isMotorfocusAvailable() const
{
    return (m_value & (1 << 6)) != 0;
}

BayonetState Status::getBayonetState() const
{
    return static_cast<BayonetState>((m_value >> 7) & 0b11);
}

bool Status::isMotorfocusRunning() const
{
    return (m_value & (1 << 9)) != 0;
}

bool Status::isMotorfocusPositionReached() const
{
    return (m_value & (1 << 10)) != 0;
}

bool Status::isAnyTriggerActive() const
{
    return (m_value & (1 << 11)) != 0;
}

bool Status::nucRegistersChanged() const
{
    return (m_value & (1 << 27)) != 0;
}

bool Status::bolometerRegistersChanged() const
{
    return (m_value & (1 << 28)) != 0;
}

bool Status::focusRegistersChanged() const
{
    return (m_value & (1 << 30)) != 0;
}

bool Status::presetsRegistersChanged() const
{
    return (m_value & (1 << 31)) != 0;
}

bool Status::operator==(const Status& other) const
{
    return m_value == other.m_value;
}

bool Status::operator!=(const Status& other) const
{
    return m_value != other.m_value;
}

} // namespace wl
