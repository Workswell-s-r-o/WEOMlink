#ifndef WL_WEOM_H
#define WL_WEOM_H

#include "wl/error.h"
#include "wl/time.h"
#include "wl/dataclasses/contrastbrightness.h"
#include "wl/dataclasses/firmwareversion.h"
#include "wl/dataclasses/framerate.h"
#include "wl/dataclasses/imageequalizationtype.h"
#include "wl/dataclasses/imageflip.h"
#include "wl/dataclasses/imagegenerator.h"
#include "wl/dataclasses/presetid.h"
#include "wl/dataclasses/shutterupdatemode.h"
#include "wl/dataclasses/status.h"
#include "wl/dataclasses/timedomainaveraging.h"
#include "wl/dataclasses/triggers.h"
#include "wl/dataclasses/videoformat.h"
#include "wl/dataclasses/agcnhsmoothing.h"
#include "wl/dataclasses/triggermode.h"
#include "wl/dataclasses/auxpinmode.h"
#include "wl/dataclasses/reticletype.h"
#include "wl/dataclasses/internalshutterposition.h"
#include "wl/dataclasses/baudrate.h"

#include "wl/communication/protocolinterfacetcsi.h"
#include "wl/weom/deviceinterfaceweom.h"
#include "wl/communication/idatalinkinterface.h"
#include "wl/communication/ideviceinterface.h"

#include <etl/string.h>
#include <etl/expected.h>


namespace wl {

/**
 * @class WEOM
 * @headerfile weom.h "wl/weom.h"
 * @brief Class for managing WEOM device configurations and operations.
 *
 * @details
 * This class provides methods to access and modify settings on the WEOM device, such as triggering actions,
 * managing image settings, and retrieving system status and identification information.
 *
 * @note The data link interface must be set using the `WEOM::setDataLinkInterface` method before invoking other methods.
 * Without a properly configured data link interface, the device cannot be accessed or configured.
 * @{
 */
class WEOM
{
public:
    /**
     * @brief Creates WEOM class
     * @param sleepFunction User-defined function to handle delays, taking a duration as input.
     */
    explicit WEOM(SleepFunction sleepFunction);
    ~WEOM();

    /**
     * @brief Sets the data link interface for communication with the device.
     * @param dataLinkInterface A unique pointer to an `IDataLinkInterface` instance.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setDataLinkInterface(etl::unique_ptr<IDataLinkInterface> dataLinkInterface);

    /**
     * @brief Retrieves the current status of the device.
     * @return An `etl::expected<Status, Error>` containing the device status or an error.
     * @see registers_status
     */
    [[nodiscard]] etl::expected<Status, Error> getStatus();

    /**
     * @brief Retrieves the current trigger configuration of the device.
     * @return An `etl::expected<Triggers, Error>` containing the trigger settings or an error.
     * @see registers_trigger
     */
    [[nodiscard]] etl::expected<Triggers, Error> getTriggers();

    /**
     * @brief Activates a specific trigger on the device.
     * @param trigger The trigger to activate.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_trigger
     */
    [[nodiscard]] etl::expected<void, Error> activateTrigger(Trigger trigger);

    /**
     * @brief Retrieves the brightness of the red LED on the device.
     * @return An `etl::expected<uint8_t, Error>` containing the red LED brightness or an error.
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getLedRedBrightness();

    /**
     * @brief Sets the brightness of the red LED on the device.
     * @param brightness The brightness value to set (1-7).
     * @param memoryType The memory region to set. 
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setLedRedBrightness(uint8_t brightness, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the brightness of the green LED on the device.
     * @return An `etl::expected<uint8_t, Error>` containing the green LED brightness or an error.
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getLedGreenBrightness();

    /**
     * @brief Sets the brightness of the green LED on the device.
     * @param brightness The brightness value to set (0-7).
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setLedGreenBrightness(uint8_t brightness, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the brightness of the blue LED on the device.
     * @return An `etl::expected<uint8_t, Error>` containing the blue LED brightness or an error.
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getLedBlueBrightness();

    /**
     * @brief Sets the brightness of the blue LED on the device.
     * @param brightness The brightness value to set (0-7).
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setLedBlueBrightness(uint8_t brightness, MemoryTypeWEOM memoryType);

    /**
     * @brief Maximal size of serial number string
     * @see registers_serial_number
     */
    static constexpr size_t SERIAL_NUMBER_STRING_SIZE = MemorySpaceWEOM::SERIAL_NUMBER_CURRENT.getSize() + 1;
    /**
     * @brief Retrieves the device serial number.
     * @return An `etl::expected<etl::string<WEOM::SERIAL_NUMBER_STRING_SIZE>, Error>` containing the serial number or an error.
     * @see registers_serial_number
     */
    [[nodiscard]] etl::expected<etl::string<SERIAL_NUMBER_STRING_SIZE>, Error> getSerialNumber();

    /**
     * @brief Maximal size of article number string
     * @see registers_article_number
     */
    static constexpr size_t ARTICLE_NUMBER_STRING_SIZE = MemorySpaceWEOM::ARTICLE_NUMBER_CURRENT.getSize() + 1;
    /**
     * @brief Retrieves the device article number.
     * @return An `etl::expected<etl::string<WEOM::ARTICLE_NUMBER_STRING_SIZE>, Error>` containing the article number or an error.
     * @see registers_article_number
     */
    [[nodiscard]] etl::expected<etl::string<ARTICLE_NUMBER_STRING_SIZE>, Error> getArticleNumber();

    /**
     * @brief Retrieves the firmware version of the device.
     * @return An `etl::expected<FirmwareVersion, Error>` containing the firmware version or an error.
     * @see registers_main_firmware_version
     */
    [[nodiscard]] etl::expected<FirmwareVersion, Error> getFirmwareVersion();

    /**
     * @brief Retrieves the current trigger mode.
     * @return An `etl::expected<TriggerMode, Error>` containing the trigger mode or an error.
     */
    [[nodiscard]] etl::expected<TriggerMode, Error> getTriggerMode();

    /**
     * @brief Sets the trigger mode.
     * @param mode The trigger mode to set.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setTriggerMode(TriggerMode mode, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the current configuration of an AUX pin.
     * @param pin The AUX pin to query (0, 1, or 2).
     * @return An `etl::expected<AuxPin, Error>` containing the pin configuration or an error.
     */
    [[nodiscard]] etl::expected<AuxPin, Error> getAuxPin(uint8_t pin);

    /**
     * @brief Sets the configuration of an AUX pin.
     * @param pin The AUX pin to configure (0, 1, or 2).
     * @param mode The configuration to set.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setAuxPin(uint8_t pin, AuxPin mode, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the current palette index setting.
     * @return An `etl::expected<uint8_t, Error>` containing the palette index or an error.
     * @see registers_palette_index
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getPaletteIndex();

    /**
     * @brief Sets the palette index.
     * @param paletteIndex The palette index to set. Allowed values are 0-15.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_palette_index
     */
    [[nodiscard]] etl::expected<void, Error> setPaletteIndex(uint8_t paletteIndex, MemoryTypeWEOM memoryType);

    /**
     * @brief Gets the palette name.
     * @param paletteIndex The palette index to get. Allowed values are 0-15.
     * @return An `etl::expected<etl::string<MemorySpaceWEOM::PALETTE_NAME_SIZE>, Error>` indicating success or failure.
     * @see registers_palette_index
     */
    [[nodiscard]] etl::expected<etl::string<MemorySpaceWEOM::PALETTE_NAME_SIZE>, Error> getPaletteName(unsigned paletteIndex);

    /**
     * @brief Retrieves the current frame rate setting.
     * @return An `etl::expected<Framerate, Error>` containing the frame rate or an error.
     * @see registers_frame_rate
     */
    [[nodiscard]] etl::expected<Framerate, Error> getFramerate();

    /**
     * @brief Sets the frame rate.
     * @param framerate The frame rate to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_frame_rate
     */
    [[nodiscard]] etl::expected<void, Error> setFramerate(Framerate framerate);

    /**
     * @brief Retrieves the current image flip setting.
     * @return An `etl::expected<ImageFlip, Error>` containing the image flip setting or an error.
     * @see registers_image_flip
     */
    [[nodiscard]] etl::expected<ImageFlip, Error> getImageFlip();

    /**
     * @brief Sets the image flip setting.
     * @param flip The desired image flip setting.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_image_flip
     */
    [[nodiscard]] etl::expected<void, Error> setImageFlip(const ImageFlip& flip);

    /**
     * @brief Checks if image freeze is enabled.
     * @return An `etl::expected<bool, Error>` indicating if freeze is enabled or an error.
     * @see registers_image_freeze
     */
    [[nodiscard]] etl::expected<bool, Error> getImageFreeze();

    /**
     * @brief Sets the image freeze status.
     * @param freeze Boolean to enable or disable freeze.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_image_freeze
     */
    [[nodiscard]] etl::expected<void, Error> setImageFreeze(bool freeze);

    /**
     * @brief Retrieves the current image generator setting.
     * @return An `etl::expected<ImageGenerator, Error>` containing the image generator or an error.
     * @see registers_test_pattern
     */
    [[nodiscard]] etl::expected<ImageGenerator, Error> getImageGenerator();

    /**
     * @brief Sets the image generator.
     * @param generator The image generator setting to use.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_test_pattern
     */
    [[nodiscard]] etl::expected<void, Error> setImageGenerator(ImageGenerator generator);

    /**
     * @brief Retrieves the current reticle type setting.
     * @return An `etl::expected<ReticleType, Error>` containing the reticle type or an error.
     * @see registers_reticle_type
     */
    [[nodiscard]] etl::expected<ReticleType, Error> getReticleType();

    /**
     * @brief Sets the reticle type.
     * @param mode The reticle type to set.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_reticle_type
     */
    [[nodiscard]] etl::expected<void, Error> setReticleType(ReticleType mode, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the current reticle X-axis coordinate.
     * @return An `etl::expected<int32_t, Error>` containing the reticle position or an error.
     * @see registers_reticle_position_x
     */
    [[nodiscard]] etl::expected<int32_t, Error> getReticlePositionX();

    /**
     * @brief Sets the reticle X-axis coordinate.
     * @param position The reticle position to set (-200, 200)
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_reticle_position_x
     */
    [[nodiscard]] etl::expected<void, Error> setReticlePositionX(int32_t position, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the current reticle Y-axis coordinate.
     * @return An `etl::expected<int32_t, Error>` containing the reticle position or an error.
     * @see registers_reticle_position_y
     */
    [[nodiscard]] etl::expected<int32_t, Error> getReticlePositionY();

    /**
     * @brief Sets the reticle Y-axis coordinate.
     * @param position The reticle position to set (-100, 100)
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_reticle_position_y
     */
    [[nodiscard]] etl::expected<void, Error> setReticlePositionY(int32_t position, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the current shutter counter value
     * @return An `etl::expected<uint32_t, Error>` containing the shutter counter or an error.
     * @see registers_shutter_counter
     */
    [[nodiscard]] etl::expected<uint32_t, Error> getShutterCounter();

    /**
     * @brief Retrieves the time since the last NUC offset update in milliseconds.
     * @return An `etl::expected<uint32_t, Error>` containing the time or an error.
     * @see registers_time_from_last_nuc_offset_update
     */
    [[nodiscard]] etl::expected<uint32_t, Error> getTimeFromLastNucOffsetUpdate();

    /**
     * @brief Retrieves the current internal shutter position.
     * @return An `etl::expected<InternalShutterPosition, Error>` containing the internal shutter position or an error.
     * @see registers_internal_shutter_position
     */
    [[nodiscard]] etl::expected<InternalShutterPosition, Error> getInternalShutterPosition();

    /**
     * @brief Sets the internal shutter position.
     * @param position The internal shutter position to set (open or closed).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_internal_shutter_position
     */
    [[nodiscard]] etl::expected<void, Error> setInternalShutterPosition(InternalShutterPosition position);

    /**
     * @brief Retrieves the shutter update mode.
     * @return An `etl::expected<ShutterUpdateMode, Error>` containing the shutter mode or an error.
     * @see registers_nuc_update_mode
     */
    [[nodiscard]] etl::expected<ShutterUpdateMode, Error> getShutterUpdateMode();

    /**
     * @brief Retrieves the shutter temperature.
     * @return An `etl::expected<double, Error>` containing the shutter temperature or an error.
     * @see registers_shutter_temperature
     */
    [[nodiscard]] etl::expected<double, Error> getShutterTemperature();

    /**
     * @brief Sets the shutter update mode.
     * @param mode The shutter update mode.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_nuc_update_mode
     */
    [[nodiscard]] etl::expected<void, Error> setShutterUpdateMode(ShutterUpdateMode mode, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the shutter max period.
     * @return An `etl::expected<uint16_t, Error>` containing the shutter max period or an error.
     * @see registers_nuc_max_period
     */
    [[nodiscard]] etl::expected<uint16_t, Error> getShutterMaxPeriod();

    /**
     * @brief Sets the shutter max period.
     * @param value The shutter max period. Allowed values 120 - 7200
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_nuc_max_period
     */
    [[nodiscard]] etl::expected<void, Error> setShutterMaxPeriod(uint16_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the shutter adaptive threshold.
     * @return An `etl::expected<ShutterUpdateMode, Error>` containing the shutter max period or an error.
     * @see registers_nuc_adaptive_threshold
     */
    [[nodiscard]] etl::expected<double, Error> getShutterAdaptiveThreshold();

    /**
     * @brief Sets the shutter adaptive threshold.
     * @param value The shutter adaptive threshold. Allowed values 0.25 - 10.0 (values are rounded to nearest 0.0625)
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_nuc_adaptive_threshold
     */
    [[nodiscard]] etl::expected<void, Error> setShutterAdaptiveThreshold(double value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the UART baudrate.
     * @return An `etl::expected<Baudrate, Error>` containing the baudrate or an error.
     * @see registers_uart_baudrate
     */
    [[nodiscard]] etl::expected<Baudrate, Error> getUartBaudrate();

    /**
     * @brief Sets the UART baudrate.
     * @param baudrate The baudrate to set.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_uart_baudrate
     */
    [[nodiscard]] etl::expected<void, Error> setUartBaudrate(Baudrate baudrate, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the time domain averaging setting.
     * @return An `etl::expected<TimeDomainAveraging, Error>` containing the averaging setting or an error.
     * @see registers_time_domain_average
     */
    [[nodiscard]] etl::expected<TimeDomainAveraging, Error> getTimeDomainAveraging();

    /**
     * @brief Sets the time domain averaging.
     * @param averaging The time domain averaging setting to apply.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_time_domain_average
     */
    [[nodiscard]] etl::expected<void, Error> setTimeDomainAveraging(TimeDomainAveraging averaging, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the image equalization type.
     * @return An `etl::expected<ImageEqualizationType, Error>` containing the equalization type or an error.
     * @see registers_linear_equalization_type
     */
    [[nodiscard]] etl::expected<ImageEqualizationType, Error> getImageEqualizationType();

    /**
     * @brief Sets the image equalization type.
     * @param type The desired equalization type.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_linear_equalization_type
     */
    [[nodiscard]] etl::expected<void, Error> setImageEqualizationType(ImageEqualizationType type, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the contrast and brightness settings.
     * @return An `etl::expected<ContrastBrightness, Error>` containing the settings or an error.
     * @see registers_mgc_contrast_brightness
     */
    [[nodiscard]] etl::expected<ContrastBrightness, Error> getMgcContrastBrightness();

    /**
     * @brief Sets the contrast and brightness.
     * @param contrastBrightness The contrast and brightness settings to apply.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_mgc_contrast_brightness
     */
    [[nodiscard]] etl::expected<void, Error> setMgcContrastBrightness(const ContrastBrightness& contrastBrightness, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the frame block median contrast and brightness settings.
     * @return An `etl::expected<ContrastBrightness, Error>` containing the settings or an error.
     * @see registers_frameblock_median_conbright
     */
    [[nodiscard]] etl::expected<ContrastBrightness, Error> getFrameBlockMedianConbright();

    /**
     * @brief Retrieves the AGC NH smoothing frames setting.
     * @return An `etl::expected<AGCNHSmoothing, Error>` containing the frame count or an error.
     * @see registers_agc_nh_smoothing
     */
    [[nodiscard]] etl::expected<AGCNHSmoothing, Error> getAgcNhSmoothingFrames();

    /**
     * @brief Sets the AGC NH smoothing frames count.
     * @param smoothing The number of smoothing frames.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_agc_nh_smoothing
     */
    [[nodiscard]] etl::expected<void, Error> setAgcNhSmoothingFrames(AGCNHSmoothing smoothing, MemoryTypeWEOM memoryType);

    /**
     * @brief Checks if the spatial median filter is enabled.
     * @return An `etl::expected<bool, Error>` indicating if the filter is enabled or an error.
     * @see registers_spatial_median_filter_enable
     */
    [[nodiscard]] etl::expected<bool, Error> getSpatialMedianFilterEnabled();

    /**
     * @brief Enables or disables the spatial median filter.
     * @param enabled Boolean to enable or disable the filter.
     * @param memoryType The memory region to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_spatial_median_filter_enable
     */
    [[nodiscard]] etl::expected<void, Error> setSpatialMedianFilterEnabled(bool enabled, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the linear gain weight.
     * @return An `etl::expected<uint8_t, Error>` containing the linear gain weight or an error.
     * @see registers_linear_gain_weight
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getLinearGainWeight();

    /**
     * @brief Sets the linear gain weight.
     * @param value The linear gain weight to set. Allowed values 0 - 10
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_linear_gain_weight
     */
    [[nodiscard]] etl::expected<void, Error> setLinearGainWeight(uint8_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the clip limit.
     * @return An `etl::expected<uint8_t, Error>` containing the clip limit or an error.
     * @see registers_clip_limit
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getClipLimit();

    /**
     * @brief Sets the clip limit.
     * @param value The clip limit to set. Allowed values 1 - 100
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_clip_limit
     */
    [[nodiscard]] etl::expected<void, Error> setClipLimit(uint8_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the plateau tail rejection.
     * @return An `etl::expected<uint8_t, Error>` containing the plateau tail rejection value or an error.
     * @see registers_plateau_tail_rejection
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getPlateauTailRejection();

    /**
     * @brief Sets the plateau tail rejection value.
     * @param value The plateau tail rejection value to set. Allowed values 0 - 49
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_plateau_tail_rejection
     */
    [[nodiscard]] etl::expected<void, Error> setPlateauTailRejection(uint8_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the smart time domain average threshold.
     * @return An `etl::expected<uint8_t, Error>` containing the smart time domain average threshold or an error.
     * @see registers_smart_time_domain_average_threshold
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getSmartTimeDomainAverageThreshold();

    /**
     * @brief Sets the smart time domain average threshold.
     * @param value The smart time domain average threshold to set. Allowed values 0 - 31
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_smart_time_domain_average_threshold
     */
    [[nodiscard]] etl::expected<void, Error> setSmartTimeDomainAverageThreshold(uint8_t value, MemoryTypeWEOM memoryType);

    /** 
     * @brief Retrieves the smart median threshold.
     * @return An `etl::expected<uint8_t, Error>` containing the smart median threshold or an error.
     * @see registers_smart_median_threshold
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getSmartMedianThreshold();

    /**
     * @brief Sets the smart median threshold.
     * @param value The smart median threshold to set. Allowed values 0 - 31
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_smart_median_threshold
     */
    [[nodiscard]] etl::expected<void, Error> setSmartMedianThreshold(uint8_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the gamma correction value.
     * @return An `etl::expected<double, Error>` containing the gamma correction value or an error.
     * @see registers_gamma_correction
     */
    [[nodiscard]] etl::expected<double, Error> getGammaCorrection();

    /** 
     * @brief Sets the gamma correction value.
     * @param value The gamma correction value to set. Allowed values 0.25 - 4.0 in steps of 0.25.
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_gamma_correction
     */
    [[nodiscard]] etl::expected<void, Error> setGammaCorrection(double value, MemoryTypeWEOM memoryType);

    /** 
     * @brief Retrieves the max amplification value.
     * @return An `etl::expected<double, Error>` containing the max amplification value or an error.
     * @see registers_max_amplification
     */
    [[nodiscard]] etl::expected<double, Error> getMaxAmplification();

    /** 
     * @brief Sets the max amplification value.
     * @param value The max amplification value to set. Allowed values 0.25 - 4.0 in steps of 0.25.
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_max_amplification
     */
    [[nodiscard]] etl::expected<void, Error> setMaxAmplification(double value, MemoryTypeWEOM memoryType);   
    
    /**
     * @brief Retrieves the damping factor value.
     * @return An `etl::expected<uint8_t, Error>` containing the damping factor value or an error.
     * @see registers_damping_factor
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getDampingFactor();

    /**
     * @brief Sets the damping factor value.
     * @param value The damping factor value to set. Allowed values 0 - 100
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_damping_factor
     */
    [[nodiscard]] etl::expected<void, Error> setDampingFactor(uint8_t value, MemoryTypeWEOM memoryType);

    /**
     * @brief Retrieves the preset ID by index.
     * @param index The index of the preset ID to retrieve. Allowed values 0 - number of presets
     * @return An `etl::expected<PresetId, Error>` containing the preset ID or an error.
     * @see registers_selected_preset_id
     */
    [[nodiscard]] etl::expected<PresetId, Error> getPresetId(uint8_t index);

    /**
     * @brief Retrieves the preset ID count.
     * @return An `etl::expected<uint8_t, Error>` containing the preset ID count or an error.
     * @see registers_number_of_presets_and_attributes
     */
    [[nodiscard]] etl::expected<std::uint8_t, Error> getPresetIdCount();

    /**
     * @brief Retrieves the current preset ID.
     * @return An `etl::expected<PresetId, Error>` containing the preset ID or an error.
     * @see registers_current_preset_id
     */
    [[nodiscard]] etl::expected<PresetId, Error> getPresetId();

    /**
     * @brief Sets the preset ID.
     * @param id The preset ID to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_selected_preset_id, registers_trigger
     */
    [[nodiscard]] etl::expected<void, Error> setPresetId(const PresetId& id);

    /**
     * @brief Save current preset index to flash memory.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_selected_preset_index
     */
    [[nodiscard]] etl::expected<void, Error> saveCurrentPresetIndexToFlash();

    /**
     * @brief Sets the video format.
     * @param videoFormat The video format to set.
     * @param memoryType The memory region to set (RAM or FLASH).
     * @return An `etl::expected<void, Error>` indicating success or failure.
     * @see registers_video_format
     */
    [[nodiscard]] etl::expected<void, Error> setVideoFormat(VideoFormat videoFormat, MemoryTypeWEOM memoryType);

private:
    etl::unique_ptr<DeviceInterfaceWEOM> m_deviceInterface;
    uint8_t m_lastPacketId;
    SleepFunction m_sleepFunction;

    template <const AddressRange& addressRange>
    etl::expected<etl::array<uint8_t, addressRange.getSize()>, Error> readAddressRange();

    etl::expected<void, Error> writeData(const etl::span<uint8_t>& data, const AddressRange& addressRange, MemoryTypeWEOM memoryType = MemoryTypeWEOM::REGISTERS_CONFIGURATION);
};
/** @} */

} // namespace wl

#endif // WL_WEOM_H
