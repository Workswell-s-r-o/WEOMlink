#ifndef WL_IMAGEEQUALIZATIONTYPE_H
#define WL_IMAGEEQUALIZATIONTYPE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct ImageEqualizationType
 * @headerfile imageequalizationtype.h "wl/dataclasses/imageequalizationtype.h"
 * @brief Defines the types of image equalization
 * @see registers_linear_equalization_type
 */
struct ImageEqualizationType
{
    /**
     * @enum enum_type
     * @brief Enum type representing image equalization methods.
     */
    enum enum_type: uint8_t
    {
        AGC_NH, ///< Automatic Gain Control
        MGC,    ///< Manual Gain Control.
    };

    ETL_DECLARE_ENUM_TYPE(ImageEqualizationType, uint8_t)
    ETL_ENUM_TYPE(AGC_NH, "AGC_NH")
    ETL_ENUM_TYPE(MGC, "MGC")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_IMAGEEQUALIZATIONTYPE_H