#ifndef WL_AGCNHSMOOTHING_H
#define WL_AGCNHSMOOTHING_H

#include <etl/enum_type.h>

#include <cstdint>


namespace wl {

/**
 * @struct AGCNHSmoothing
 * @headerfile agcnhsmoothing.h "wl/dataclasses/agcnhsmoothing.h"
 * @brief Represents time domain averaging modes.
 * @see registers_agc_nh_smoothing
 */
struct AGCNHSmoothing
{
    /**
     * @enum enum_type
     * @brief Enumeration of time domain averaging modes.
     */
    enum enum_type : uint8_t
    {
        NO_AVERAGING,     ///< No averaging is applied.
        AVERAGE_2_FRAMES, ///< Averaging over 2 frames.
        AVERAGE_4_FRAMES, ///< Averaging over 4 frames.
        AVERAGE_8_FRAMES, ///< Averaging over 8 frames.
        AVERAGE_16_FRAMES ///< Averaging over 16 frames.
    };

    ETL_DECLARE_ENUM_TYPE(AGCNHSmoothing, uint8_t)
    ETL_ENUM_TYPE(NO_AVERAGING, "NO_AVERAGING")
    ETL_ENUM_TYPE(AVERAGE_2_FRAMES, "AVERAGE_2_FRAMES")
    ETL_ENUM_TYPE(AVERAGE_4_FRAMES, "AVERAGE_4_FRAMES")
    ETL_ENUM_TYPE(AVERAGE_8_FRAMES, "AVERAGE_8_FRAMES")
    ETL_ENUM_TYPE(AVERAGE_16_FRAMES, "AVERAGE_16_FRAMES")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_AGCNHSMOOTHING_H