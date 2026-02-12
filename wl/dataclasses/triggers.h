#ifndef WL_TRIGGERS_H
#define WL_TRIGGERS_H

#include <etl/enum_type.h>

#include <wl/dataclasses/triggermode.h>
#include <wl/dataclasses/auxpin.h>

namespace wl {

/**
 * @struct Trigger
 * @headerfile triggers.h "wl/dataclasses/triggers.h"
 * @brief Represents a collection of system triggers.
 */
struct Trigger
{
    /**
     * @enum enum_type
     * @brief Enumeration of trigger flags.
     * 
     * Each enumerator represents a specific system trigger, identified by a unique bit flag.
     */
    enum enum_type : uint32_t
    {
        RESET_FPGA               = 1 << 0, ///< Resets FPGA
        RESET_TO_LOADER          = 1 << 1, ///< Resets FPGA to loader
        NUC_OFFSET_UPDATE        = 1 << 2, ///< Performs NUC offset update
        CLEAN_USER_DP            = 1 << 3, ///< Cleans user dead pixels
        SET_SELECTED_PRESET      = 1 << 4, ///< Sets selected preset as current
        MOTORFOCUS_CALIBRATION   = 1 << 5, ///< Performs motorfocus calibration
        FRAME_CAPTURE_START      = 1 << 6, ///< Starts frame capture
        RESET_TO_FACTORY_DEFAULT = 1 << 7, ///< Resets all settings to factory default
        PERFORM_AUTOFOCUS        = 1 << 8  ///< Performs autofocus
    };

    ETL_DECLARE_ENUM_TYPE(Trigger, uint32_t)
    ETL_ENUM_TYPE(RESET_FPGA, "RESET_FPGA")
    ETL_ENUM_TYPE(RESET_TO_LOADER, "RESET_TO_LOADER")
    ETL_ENUM_TYPE(NUC_OFFSET_UPDATE, "NUC_OFFSET_UPDATE")
    ETL_ENUM_TYPE(CLEAN_USER_DP, "CLEAN_USER_DP")
    ETL_ENUM_TYPE(SET_SELECTED_PRESET, "SET_SELECTED_PRESET")
    ETL_ENUM_TYPE(MOTORFOCUS_CALIBRATION, "MOTORFOCUS_CALIBRATION")
    ETL_ENUM_TYPE(FRAME_CAPTURE_START, "FRAME_CAPTURE_START")
    ETL_ENUM_TYPE(RESET_TO_FACTORY_DEFAULT, "RESET_TO_FACTORY_DEFAULT")
    ETL_ENUM_TYPE(PERFORM_AUTOFOCUS, "PERFORM_AUTOFOCUS")
    ETL_END_ENUM_TYPE
};

/**
 * @class Triggers
 * @headerfile triggers.h "wl/dataclasses/triggers.h"
 * @brief Provides an interface for querying system triggers.
 */
class Triggers
{
public:
    /**
     * @brief Default constructor.
     */
    explicit Triggers() = default;

    /**
     * @brief Constructs a `Triggers` instance with the specified trigger value.
     * @param value A bitfield representing the active triggers.
     */
    explicit Triggers(uint32_t value);

    /**
     * @brief Checks if a specific trigger is active.
     * @param trigger The trigger to check.
     * @return `true` if the trigger is active, `false` otherwise.
     */
    bool isActive(Trigger trigger) const;

    /**
     * @brief Checks if any trigger is active.
     * @return `true` if any trigger is active, `false` otherwise.
     */
    bool isAnyActive() const;

private:
    uint32_t m_value;
};

} // namespace wl

#endif // WL_TRIGGERS_H
