#ifndef WL_ENDIAN_H
#define WL_ENDIAN_H

#include <etl/endianness.h>

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

} // namespace wl

#endif // WL_ENDIAN_H