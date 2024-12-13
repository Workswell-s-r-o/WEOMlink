#ifndef WL_ADDRESSRANGE_H
#define WL_ADDRESSRANGE_H

#include <etl/algorithm.h>

#include <cstdint>
#include <cassert>
#include <compare>


namespace wl {

/**
 * @class AddressRange
 * @headerfile addressrange.h "wl/communication/addressrange.h"
 * @brief Represents a range of addresses, with utility methods for range manipulation,
 * containment checks, and comparison.
 */
class AddressRange
{
    explicit constexpr AddressRange(uint32_t firstAddress, uint32_t lastAddress);

public:
    /**
     * @brief Creates an AddressRange given a starting address and a size.
     * @param firstAddress The starting address of the range.
     * @param size The number of addresses in the range.
     * @return An AddressRange object with the specified start and size.
     */
    static constexpr AddressRange firstAndSize(uint32_t firstAddress, uint32_t size);

    /**
     * @brief Creates an AddressRange given a starting and an ending address.
     * @param firstAddress The starting address of the range.
     * @param lastAddress The ending address of the range.
     * @return An AddressRange object with the specified start and end addresses.
     */
    static constexpr AddressRange firstToLast(uint32_t firstAddress, uint32_t lastAddress);

    /**
     * @brief Retrieves the first address in the range.
     * @return The starting address of the range.
     */
    constexpr uint32_t getFirstAddress() const;

    /**
     * @brief Retrieves the last address in the range.
     * @return The ending address of the range.
     */
    constexpr uint32_t getLastAddress() const;

    /**
     * @brief Calculates the size of the address range.
     * @return The number of addresses in the range.
     */
    constexpr uint32_t getSize() const;

    /**
     * @brief Checks if a specified address is within the range.
     * @param address The address to check.
     * @return True if the address is within the range, false otherwise.
     */
    bool contains(uint32_t address) const;

    /**
     * @brief Checks if another AddressRange is fully contained within this range.
     * @param other The AddressRange to check.
     * @return True if the other range is fully contained within this range, false otherwise.
     */
    bool contains(const AddressRange& other) const;

    /**
     * @brief Checks if another AddressRange overlaps with this range.
     * @param other The AddressRange to check for overlap.
     * @return True if the ranges overlap, false otherwise.
     */
    bool overlaps(const AddressRange& other) const;

    /**
     * @brief Returns a new AddressRange moved by a specified offset.
     * @param offset The offset to apply to the range.
     * @return A new AddressRange shifted by the given offset.
     */
    constexpr AddressRange moved(uint32_t offset) const;

    /**
     * @brief Compares two AddressRange objects for ordering.
     * @param other The AddressRange to compare with.
     * @return A std::strong_ordering value indicating the comparison result.
     */
    std::strong_ordering operator<=>(const AddressRange& other) const = default;

private:
    uint32_t m_firstAddress;
    uint32_t m_lastAddress;
};


// Impl

constexpr AddressRange::AddressRange(uint32_t firstAddress, uint32_t lastAddress) :
    m_firstAddress(firstAddress),
    m_lastAddress(etl::max(firstAddress, lastAddress))
{
    assert(firstAddress <= lastAddress);
}

constexpr AddressRange AddressRange::firstAndSize(uint32_t firstAddress, uint32_t size)
{
    return AddressRange(firstAddress, firstAddress + size - 1);
}

constexpr AddressRange AddressRange::firstToLast(uint32_t firstAddress, uint32_t lastAddress)
{
    return AddressRange(firstAddress, lastAddress);
}

constexpr uint32_t AddressRange::getFirstAddress() const
{
    return m_firstAddress;
}

constexpr uint32_t AddressRange::getLastAddress() const
{
    return m_lastAddress;
}

constexpr uint32_t AddressRange::getSize() const
{
    return m_lastAddress + 1 - m_firstAddress;
}

constexpr AddressRange AddressRange::moved(uint32_t offset) const
{
    return AddressRange(m_firstAddress + offset, m_lastAddress + offset);
}

} // namespace wl

#endif // WL_ADDRESSRANGE_H
