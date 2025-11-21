#ifndef WL_IMAGEGENERATOR_H
#define WL_IMAGEGENERATOR_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct ImageGenerator
 * @headerfile imagegenerator.h "wl/dataclasses/imagegenerator.h"
 * @brief Defines the types of image generators
 * @see registers_test_pattern
 */
struct ImageGenerator
{
    /**
     * @enum enum_type
     * @brief Enum type representing the image generator types.
     */
    enum enum_type: uint8_t
    {
        SENSOR,           ///< Image from sensor
        ADC_1,            ///< Image from ADC 1
        ADC_2,            ///< Image from ADC 2
        INTERNAL_DYNAMIC, ///< Image from internal dynamic generator
        INTERNAL_STATIC,  ///< Image from internal static generator
    };

    ETL_DECLARE_ENUM_TYPE(ImageGenerator, uint8_t)
    ETL_ENUM_TYPE(SENSOR, "SENSOR")
    ETL_ENUM_TYPE(ADC_1, "ADC_1")
    ETL_ENUM_TYPE(ADC_2, "ADC_2")
    ETL_ENUM_TYPE(INTERNAL_DYNAMIC, "INTERNAL_DYNAMIC")
    ETL_ENUM_TYPE(INTERNAL_STATIC, "INTERNAL_STATIC")
    ETL_END_ENUM_TYPE
};

} // namespace wl

#endif // WL_IMAGEGENERATOR_H