#include "wl/dataclasses/triggers.h"

namespace wl {

Triggers::Triggers(uint32_t value)
    : m_value(value)
{

}

bool Triggers::isActive(Trigger trigger) const
{
    return (m_value & static_cast<uint32_t>(trigger)) != 0;
}

bool Triggers::isAnyActive() const
{
    return m_value != 0;
}

} // namespace wl