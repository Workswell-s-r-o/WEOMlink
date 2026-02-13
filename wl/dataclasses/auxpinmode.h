#ifndef WL_AUXPINMODE_H
#define WL_AUXPINMODE_H

#include <etl/enum_type.h>

#include <cstdint>

namespace wl {

/**
 * @struct AuxPinMode
 * @headerfile auxpinmode.h "wl/dataclasses/auxpinmode.h"
 * @brief Defines AUX pin modes
 */
struct AuxPinMode
{
    /**
     * @enum enum_type
     * @brief Enum type representing the AUX pin modes
     */
    enum enum_type : uint8_t
    {
        HIGH_IMPEDANCE                   = 0,
        FIXED_HIGH_LOGIC_LEVEL           = 1,
        FIXED_LOW_LOGIC_LEVEL            = 2,
        INTERNAL_TRIGGER_OUTPUT          = 3,
        EXTERNAL_TRIGGER_OUTPUT          = 4,
        NUC_OFFSET_UPDATE_TRIGGER_OUTPUT = 5,
        NUC_OFFSET_UPDATE_REQUEST_INPUT  = 6,
    };

    ETL_DECLARE_ENUM_TYPE(AuxPinMode, uint8_t)
    ETL_ENUM_TYPE(HIGH_IMPEDANCE, "HIGH_IMPEDANCE")
    ETL_ENUM_TYPE(FIXED_HIGH_LOGIC_LEVEL, "FIXED_HIGH_LOGIC_LEVEL")
    ETL_ENUM_TYPE(FIXED_LOW_LOGIC_LEVEL, "FIXED_LOW_LOGIC_LEVEL")
    ETL_ENUM_TYPE(INTERNAL_TRIGGER_OUTPUT, "INTERNAL_TRIGGER_OUTPUT")
    ETL_ENUM_TYPE(EXTERNAL_TRIGGER_OUTPUT, "EXTERNAL_TRIGGER_OUTPUT")
    ETL_ENUM_TYPE(NUC_OFFSET_UPDATE_TRIGGER_OUTPUT, "NUC_OFFSET_UPDATE_TRIGGER_OUTPUT")
    ETL_ENUM_TYPE(NUC_OFFSET_UPDATE_REQUEST_INPUT, "NUC_OFFSET_UPDATE_REQUEST_INPUT")
    ETL_END_ENUM_TYPE
};

} // namespace wl


#endif // WL_AUXPINMODE_H