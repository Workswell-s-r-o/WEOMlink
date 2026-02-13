#ifndef WL_ENDIAN_H
#define WL_ENDIAN_H

#include <etl/endianness.h>
#include <etl/span.h>

#include <cstring>

namespace wl {

template <typename T>
constexpr typename etl::enable_if<etl::is_integral<T>::value, T>::type fromLittleEndian(T value)
{
    if (etl::endianness::value() == etl::endian::big)
    {
        return etl::reverse_bytes(value);
    }
    else
    {
        return value;
    }
}

template <typename T>
constexpr typename etl::enable_if<etl::is_integral<T>::value, T>::type fromBigEndian(T value)
{
    if (etl::endianness::value() == etl::endian::little)
    {
        return etl::reverse_bytes(value);
    }
    else
    {
        return value;
    }
}

template <typename T>
constexpr typename etl::enable_if<etl::is_integral<T>::value, T>::type toLittleEndian(T value)
{
    if (etl::endianness::value() == etl::endian::big)
    {
        return etl::reverse_bytes(value);
    }
    else
    {
        return value;
    }
}

template <typename T>
constexpr typename etl::enable_if<etl::is_integral<T>::value, T>::type toBigEndian(T value)
{
    if (etl::endianness::value() == etl::endian::little)
    {
        return etl::reverse_bytes(value);
    }
    else
    {
        return value;
    }
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
void serialize(T value, uint8_t* data, size_t size, size_t offset = 0)
{
    assert(offset + size <= sizeof(T));
    std::memcpy(data, reinterpret_cast<const uint8_t*>(&value) + offset, size);
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
T deserialize(const etl::span<uint8_t>& data)
{
    assert(data.size() >= sizeof(T));
    T value = {};
    std::memcpy(&value, data.data(), sizeof(T));
    return value;
}

} // namespace wl

#endif // WL_ENDIAN_H