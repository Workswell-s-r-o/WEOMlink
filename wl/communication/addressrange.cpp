#include "wl/communication/addressrange.h"


namespace wl {

bool AddressRange::contains(uint32_t address) const
{
    return address >= m_firstAddress && address <= m_lastAddress;
}

bool AddressRange::contains(const AddressRange& other) const
{
    return contains(other.m_firstAddress) && contains(other.m_lastAddress);
}

bool AddressRange::overlaps(const AddressRange& other) const
{
    return other.m_firstAddress <= m_lastAddress && other.m_lastAddress >= m_firstAddress;
}

} // namespace wl
