#ifndef WL_INTERNALSHUTTERPOSITION_H
#define WL_INTERNALSHUTTERPOSITION_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct InternalShutterPosition
 * @headerfile internalshutterposition.h "wl/dataclasses/internalshutterposition.h"
 * @brief Defines internal shutter positions
 */
struct InternalShutterPosition
{
    /**
     * @enum enum_type
     * @brief Enum type representing the internal shutter positions
     */
    enum enum_type : uint8_t
    {
        CLOSED = 0,
        OPEN = 1
    };

    ETL_DECLARE_ENUM_TYPE(InternalShutterPosition, uint8_t)
    ETL_ENUM_TYPE(CLOSED, "CLOSED")
    ETL_ENUM_TYPE(OPEN, "OPEN")
    ETL_END_ENUM_TYPE
};

} // namespace wl


#endif // WL_INTERNALSHUTTERPOSITION_H