#ifndef WL_VIDEOFORMAT_H
#define WL_VIDEOFORMAT_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct VideoFormat
 * @headerfile videoformat.h "wl/dataclasses/videoformat.h"
 * @brief Represents video formats.
 * @see registers_video_format
 */
struct VideoFormat 
{
    /**
     * @enum enum_type
     * @brief Enumeration of video formats
     */
    enum enum_type : uint8_t 
    {
        PRE_IGC = 0,  ///< 
        POST_IGC = 1, ///< 
    };

    ETL_DECLARE_ENUM_TYPE(VideoFormat, uint8_t)
    ETL_ENUM_TYPE(PRE_IGC, "PRE_IGC")
    ETL_ENUM_TYPE(POST_IGC, "POST_IGC")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_VIDEOFORMAT_H