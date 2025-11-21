#ifndef WL_FRAMERATE_H
#define WL_FRAMERATE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct Framerate
 * @headerfile framerate.h "wl/dataclasses/framerate.h"
 * @brief Defines framerate values
 * @see registers_frame_rate
 */
struct Framerate
{
    /**
     * @enum enum_type
     * @brief Enum type representing the framerate options.
     */
    enum enum_type: uint8_t
    {
        FPS_8_57, ///< 8.57 FPS
        FPS_30,   ///< 30 FPS
        FPS_60,   ///< 60 FPS
    };
    
    ETL_DECLARE_ENUM_TYPE(Framerate, uint8_t)
    ETL_ENUM_TYPE(FPS_8_57, "FPS_8_57")
    ETL_ENUM_TYPE(FPS_30, "FPS_30")
    ETL_ENUM_TYPE(FPS_60,  "FPS_60")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_FRAMERATE_H