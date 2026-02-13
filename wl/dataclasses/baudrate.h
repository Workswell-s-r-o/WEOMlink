#ifndef WL_BAUDRATE_H
#define WL_BAUDRATE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct Baudrate
 * @headerfile baudrate.h "wl/dataclasses/baudrate.h"
 * @brief Defines baudrate values
 */
struct Baudrate
{
    /**
     * @enum enum_type
     * @brief Enum type representing the baudrate values
     */
    enum enum_type : uint8_t
    {
        B_115200  = 4,
        B_921600  = 7,
        B_3000000 = 9
    };

    ETL_DECLARE_ENUM_TYPE(Baudrate, uint8_t)
    ETL_ENUM_TYPE(B_115200, "B_115200")
    ETL_ENUM_TYPE(B_921600, "B_921600")
    ETL_ENUM_TYPE(B_3000000, "B_3000000")
    ETL_END_ENUM_TYPE
};

} // namespace wl


#endif // WL_BAUDRATE_H