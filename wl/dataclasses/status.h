#ifndef WL_STATUS_H
#define WL_STATUS_H

#include <etl/enum_type.h>

#include <cstdint>


namespace wl {

/**
 * @struct BayonetState
 * @headerfile status.h "wl/dataclasses/status.h"
 * @brief Represents the state of the bayonet connection.
 */
struct BayonetState
{
    /**
     * @enum enum_type
     * @brief Enumeration of bayonet connection states.
     */
    enum enum_type: uint8_t
    {
        UNKNOWN_STATE     = 0b00, ///< The state is unknown.
        DISCONNECTED      = 0b01, ///< The bayonet is disconnected.
        CONNECTED_UNKNOWN = 0b10, ///< The bayonet is connected but the type is unknown.
        CONNECTED_KNOWN   = 0b11  ///< The bayonet is connected with a known type.
    };

    ETL_DECLARE_ENUM_TYPE(BayonetState, uint8_t)
    ETL_ENUM_TYPE(UNKNOWN_STATE, "UNKNOWN_STATE")
    ETL_ENUM_TYPE(DISCONNECTED, "DISCONNECTED")
    ETL_ENUM_TYPE(CONNECTED_UNKNOWN, "CONNECTED_UNKNOWN")
    ETL_ENUM_TYPE(CONNECTED_KNOWN, "CONNECTED_KNOWN")
    ETL_END_ENUM_TYPE
};

/**
 * @struct DeviceType
 * @headerfile status.h "wl/dataclasses/status.h"
 * @brief Represents the type of the device.
 * @see registers_status
 */
struct DeviceType
{
    /**
     * @enum enum_type
     * @brief Enumeration of device types.
     */
    enum enum_type: uint8_t
    {
        MAIN_USER,  ///< Main program in user mode
        MAIN_ADMIN, ///< Main program in admin mode
        LOADER      ///< Loader
    };

    ETL_DECLARE_ENUM_TYPE(DeviceType, uint8_t)
    ETL_ENUM_TYPE(MAIN_USER, "MAIN_USER")
    ETL_ENUM_TYPE(MAIN_ADMIN, "MAIN_ADMIN")
    ETL_ENUM_TYPE(LOADER, "LOADER")
    ETL_END_ENUM_TYPE
};

/**
 * @class Status
 * @headerfile status.h "wl/dataclasses/status.h"
 * @brief Represents the status of the device
 * @see registers_status
 */
class Status
{
public:
    /**
     * @brief Default constructor.
     */
    explicit Status() = default;

    /**
     * @brief Constructs a `Status` object with a given value.
     * @param value A 32-bit integer representing the status flags.
     */
    explicit Status(uint32_t value);
    
    /**
     * @brief Checks if the NUC (non-uniformity correction) is active.
     * @return `true` if NUC is active, `false` otherwise.
     */
    bool isNucActive() const;

    /**
     * @brief Checks if the camera is not ready.
     * @return `true` if the camera is not ready, `false` otherwise.
     */
    bool isCameraNotReady() const;

    /**
     * @brief Checks if the TFPA is valid.
     * @return `true` if the TFPA is valid, `false` otherwise.
     */
    bool isValidTfpa() const;

    /**
     * @brief Gets the device type.
     * @return The current device type as a `DeviceType`.
     */
    DeviceType getDeviceType() const;

    /**
     * @brief Checks if the motor focus is busy.
     * @return `true` if the motor focus is busy, `false` otherwise.
     */
    bool isMotorfocusBusy() const;

    /**
     * @brief Checks if the motor focus is available.
     * @return `true` if the motor focus is available, `false` otherwise.
     */
    bool isMotorfocusAvailable() const;

    /**
     * @brief Gets the bayonet state.
     * @return The current bayonet state as a `BayonetState`.
     */
    BayonetState getBayonetState() const;

    /**
     * @brief Checks if the motor focus is running.
     * @return `true` if the motor focus is running, `false` otherwise.
     */
    bool isMotorfocusRunning() const;

    /**
     * @brief Checks if the motor focus position has been reached.
     * @return `true` if the position is reached, `false` otherwise.
     */
    bool isMotorfocusPositionReached() const;

    /**
     * @brief Checks if any trigger is active.
     * @return `true` if any trigger is active, `false` otherwise.
     */
    bool isAnyTriggerActive() const;

    /**
     * @brief Checks if the NUC registers have changed.
     * @return `true` if the NUC registers have changed, `false` otherwise.
     */
    bool nucRegistersChanged() const;
    
    /**
     * @brief Checks if the bolometer registers have changed.
     * @return `true` if the bolometer registers have changed, `false` otherwise.
     */
    bool bolometerRegistersChanged() const;

    /**
     * @brief Checks if the focus registers have changed.
     * @return `true` if the focus registers have changed, `false` otherwise.
     */
    bool focusRegistersChanged() const;

    /**
     * @brief Checks if the presets registers have changed.
     * @return `true` if the presets registers have changed, `false` otherwise.
     */
    bool presetsRegistersChanged() const;

    /**
     * @brief Compares two `Status` objects for equality.
     */
    bool operator==(const Status& other) const;

    /**
     * @brief Compares two `Status` objects for non-equality.
     */
    bool operator!=(const Status& other) const;

private:
    uint32_t m_value {0};
};

} // namespace wl

#endif // WL_STATUS_H