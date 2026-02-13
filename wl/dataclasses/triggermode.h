#ifndef WL_TRIGGERMODE_H
#define WL_TRIGGERMODE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct TriggerMode
 * @headerfile triggermode.h "wl/dataclasses/triggermode.h"
 * @brief Defines trigger modes
 */
struct TriggerMode
{
    /**
     * @enum enum_type
     * @brief Enum type representing the trigger modes
     */
    enum enum_type: uint8_t
    {
        INTERNAL_TRIGGER = 0,
        EXTERNAL_CONTINUOUS_TRIGGER = 1,
        EXTERNAL_ONE_SHOT_TRIGGER = 2
    };
    
    ETL_DECLARE_ENUM_TYPE(TriggerMode, uint8_t)
    ETL_ENUM_TYPE(INTERNAL_TRIGGER, "INTERNAL_TRIGGER")
    ETL_ENUM_TYPE(EXTERNAL_CONTINUOUS_TRIGGER, "EXTERNAL_CONTINUOUS_TRIGGER")
    ETL_ENUM_TYPE(EXTERNAL_ONE_SHOT_TRIGGER,  "EXTERNAL_ONE_SHOT_TRIGGER")
    ETL_END_ENUM_TYPE
};

} // namespace wl


#endif // WL_TRIGGERMODE_H
