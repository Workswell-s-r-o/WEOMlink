#include "wl/weom/memoryspaceweom.h"


namespace wl {


MemorySpaceWEOM::MemorySpaceWEOM(const etl::ivector<MemoryDescriptorWEOM>& memoryDescriptors) :
    m_memoryDescriptors(memoryDescriptors.begin(), memoryDescriptors.end())
{
}

etl::expected<MemoryDescriptorWEOM, Error> MemorySpaceWEOM::getMemoryDescriptor(const AddressRange& addressRange) const
{
    for (const auto& descriptor : m_memoryDescriptors)
    {
        if (descriptor.addressRange.contains(addressRange))
        {
            return descriptor;
        }
    }

    return etl::unexpected<Error>(Error::MEMORYSPACE__INVALID_ADDRESS);
}

const etl::ivector<MemoryDescriptorWEOM>& MemorySpaceWEOM::getMemoryDescriptors() const
{
    return m_memoryDescriptors;
}

MemorySpaceWEOM MemorySpaceWEOM::getDeviceSpace()
{
    etl::vector<MemoryDescriptorWEOM, 1> memoryDescriptors;
    memoryDescriptors.push_back(MemoryDescriptorWEOM{CONFIGURATION_REGISTERS, MemoryTypeWEOM::REGISTERS_CONFIGURATION});
    return MemorySpaceWEOM(memoryDescriptors);

}

MemoryDescriptorWEOM::MemoryDescriptorWEOM(const AddressRange& addressRange, MemoryTypeWEOM type) :
    addressRange(addressRange),
    type(type),
    minimumDataSize(getMinimumDataSize(type)),
    maximumDataSize(getMaximumDataSize(type))
{
}

uint32_t MemoryDescriptorWEOM::getMinimumDataSize(MemoryTypeWEOM type)
{
    switch (type)
    {
        case MemoryTypeWEOM::REGISTERS_CONFIGURATION:
            return 4;

        default:
            assert(false);
            return 0;
    }
}

uint32_t MemoryDescriptorWEOM::getMaximumDataSize(MemoryTypeWEOM type)
{
    switch (type)
    {
        case MemoryTypeWEOM::REGISTERS_CONFIGURATION:
            return 4;

        default:
            assert(false);
            return 0;
    }
}

} // namespace wl
