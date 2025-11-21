#ifndef WL_TIMEDOMAINAVERAGING_H
#define WL_TIMEDOMAINAVERAGING_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct TimeDomainAveraging
 * @headerfile timedomainaveraging.h "wl/dataclasses/timedomainaveraging.h"
 * @brief Represents time domain averaging modes.
 * @see registers_time_domain_average
 */
struct TimeDomainAveraging
{
    /**
     * @enum enum_type
     * @brief Enumeration of time domain averaging modes.
     */
    enum enum_type: uint8_t
    {
        OFF,       ///< No averaging is applied.
        FRAMES_2,  ///< Averaging over 2 frames.
        FRAMES_4   ///< Averaging over 4 frames.
    };

    ETL_DECLARE_ENUM_TYPE(TimeDomainAveraging, uint8_t)
    ETL_ENUM_TYPE(OFF, "OFF")
    ETL_ENUM_TYPE(FRAMES_2, "FRAMES_2")
    ETL_ENUM_TYPE(FRAMES_4, "FRAMES_4")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_TIMEDOMAINAVERAGING_H