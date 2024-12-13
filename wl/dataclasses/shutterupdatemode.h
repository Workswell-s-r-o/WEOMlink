#ifndef WL_SHUTTERUPDATEMODE_H
#define WL_SHUTTERUPDATEMODE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct ShutterUpdateMode
 * @headerfile shutterupdatemode.h "wl/dataclasses/shutterupdatemode.h"
 * @brief Represents the modes of shutter updates.
 */
struct ShutterUpdateMode
{
    /**
     * @enum enum_type
     * @brief Enumeration of shutter update modes.
     */
    enum enum_type: uint8_t
    {
        PERIODIC, ///< Shutter updates occur at regular intervals.
        ADAPTIVE, ///< Shutter updates occur adaptively based on system conditions.
    };

    ETL_DECLARE_ENUM_TYPE(ShutterUpdateMode, uint8_t)
    ETL_ENUM_TYPE(PERIODIC, "PERIODIC")
    ETL_ENUM_TYPE(ADAPTIVE, "ADAPTIVE")
    ETL_END_ENUM_TYPE
};


} // namespace wl

#endif // WL_SHUTTERUPDATEMODE_H