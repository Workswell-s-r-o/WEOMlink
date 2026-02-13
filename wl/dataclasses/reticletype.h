#ifndef WL_RETICLETYPE_H
#define WL_RETICLETYPE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct ReticleType
 * @headerfile reticletype.h "wl/dataclasses/reticletype.h"
 * @brief Defines reticle types
 */
struct ReticleType
{
    /**
     * @enum enum_type
     * @brief Enum type representing the reticle types
     */
    enum enum_type: uint8_t
    {
        OFF = 0,
        DARK = 1,
        BRIGHT = 2,
        ADAPTIVE = 3,
        INVERTED = 4
    };
    
    ETL_DECLARE_ENUM_TYPE(ReticleType, uint8_t)
    ETL_ENUM_TYPE(OFF, "OFF")
    ETL_ENUM_TYPE(DARK, "DARK")
    ETL_ENUM_TYPE(BRIGHT, "BRIGHT")
    ETL_ENUM_TYPE(ADAPTIVE, "ADAPTIVE")
    ETL_ENUM_TYPE(INVERTED, "INVERTED")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_RETICLETYPE_H