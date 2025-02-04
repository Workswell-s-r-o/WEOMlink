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
     */
    [[nodiscard]] etl::expected<Status, Error> getStatus();

    /**
     * @brief Retrieves the current trigger configuration of the device.
     * @return An `etl::expected<Triggers, Error>` containing the trigger settings or an error.
     */
    [[nodiscard]] etl::expected<Triggers, Error> getTriggers();

     /**
     * @brief Activates a specific trigger on the device.
     * @param trigger The trigger to activate.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> activateTrigger(Trigger trigger);

    static constexpr size_t SERIAL_NUMBER_STRING_SIZE = MemorySpaceWEOM::SERIAL_NUMBER_CURRENT.getSize() + 1; /**< Maximal size of serial number string */
    /**
     * @brief Retrieves the device serial number.
     * @return An `etl::expected<etl::string<WEOM::SERIAL_NUMBER_STRING_SIZE>, Error>` containing the serial number or an error.
     */
    [[nodiscard]] etl::expected<etl::string<SERIAL_NUMBER_STRING_SIZE>, Error> getSerialNumber();

    static constexpr size_t ARTICLE_NUMBER_STRING_SIZE = MemorySpaceWEOM::ARTICLE_NUMBER_CURRENT.getSize() + 1; /**< Maximal size of article number string */
    /**
     * @brief Retrieves the device article number.
     * @return An `etl::expected<etl::string<WEOM::ARTICLE_NUMBER_STRING_SIZE>, Error>` containing the article number or an error.
     */
    [[nodiscard]] etl::expected<etl::string<ARTICLE_NUMBER_STRING_SIZE>, Error> getArticleNumber();
    
    /**
     * @brief Retrieves the firmware version of the device.
     * @return An `etl::expected<FirmwareVersion, Error>` containing the firmware version or an error.
     */
    [[nodiscard]] etl::expected<FirmwareVersion, Error> getFirmwareVersion();

    /**
     * @brief Retrieves the current palette index setting.
     * @return An `etl::expected<uint8_t, Error>` containing the palette index or an error.
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getPaletteIndex();

    /**
     * @brief Sets the palette index.
     * @param index The palette index to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setPaletteIndex(uint8_t index);

    /**
     * @brief Retrieves the current frame rate setting.
     * @return An `etl::expected<Framerate, Error>` containing the frame rate or an error.
     */
    [[nodiscard]] etl::expected<Framerate, Error> getFramerate();

    /**
     * @brief Sets the frame rate.
     * @param framerate The frame rate to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setFramerate(Framerate framerate);

    /**
     * @brief Retrieves the current image flip setting.
     * @return An `etl::expected<ImageFlip, Error>` containing the image flip setting or an error.
     */
    [[nodiscard]] etl::expected<ImageFlip, Error> getImageFlip();

    /**
     * @brief Sets the image flip setting.
     * @param flip The desired image flip setting.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setImageFlip(const ImageFlip& flip);

    /**
     * @brief Checks if image freeze is enabled.
     * @return An `etl::expected<bool, Error>` indicating if freeze is enabled or an error.
     */
    [[nodiscard]] etl::expected<bool, Error> getImageFreeze();

    /**
     * @brief Sets the image freeze status.
     * @param freeze Boolean to enable or disable freeze.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setImageFreeze(bool freeze);

    /**
     * @brief Retrieves the current image generator setting.
     * @return An `etl::expected<ImageGenerator, Error>` containing the image generator or an error.
     */
    [[nodiscard]] etl::expected<ImageGenerator, Error> getImageGenerator();

    /**
     * @brief Sets the image generator.
     * @param generator The image generator setting to use.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setImageGenerator(ImageGenerator generator);

    /**
     * @brief Retrieves the shutter update mode.
     * @return An `etl::expected<ShutterUpdateMode, Error>` containing the shutter mode or an error.
     */
    [[nodiscard]] etl::expected<ShutterUpdateMode, Error> getShutterUpdateMode();

    /**
     * @brief Sets the shutter update mode.
     * @param mode The shutter update mode.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setShutterUpdateMode(ShutterUpdateMode mode);

    /**
     * @brief Retrieves the time domain averaging setting.
     * @return An `etl::expected<TimeDomainAveraging, Error>` containing the averaging setting or an error.
     */
    [[nodiscard]] etl::expected<TimeDomainAveraging, Error> getTimeDomainAveraging();

    /**
     * @brief Sets the time domain averaging.
     * @param averaging The time domain averaging setting to apply.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setTimeDomainAveraging(TimeDomainAveraging averaging);

    /**
     * @brief Retrieves the image equalization type.
     * @return An `etl::expected<ImageEqualizationType, Error>` containing the equalization type or an error.
     */
    [[nodiscard]] etl::expected<ImageEqualizationType, Error> getImageEqualizationType();

    /**
     * @brief Sets the image equalization type.
     * @param type The desired equalization type.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setImageEqualizationType(ImageEqualizationType type);

    /**
     * @brief Retrieves the contrast and brightness settings.
     * @return An `etl::expected<ContrastBrightness, Error>` containing the settings or an error.
     */
    [[nodiscard]] etl::expected<ContrastBrightness, Error> getMgcContrastBrightness();

    /**
     * @brief Sets the contrast and brightness.
     * @param contrastBrightness The contrast and brightness settings to apply.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setMgcContrastBrightness(const ContrastBrightness& contrastBrightness);

    /**
     * @brief Retrieves the AGC NH smoothing frames setting.
     * @return An `etl::expected<uint8_t, Error>` containing the frame count or an error.
     */
    [[nodiscard]] etl::expected<uint8_t, Error> getAgcNhSmoothingFrames();

    /**
     * @brief Sets the AGC NH smoothing frames count.
     * @param frames The number of smoothing frames.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setAgcNhSmoothingFrames(uint8_t frames);

    /**
     * @brief Checks if the spatial median filter is enabled.
     * @return An `etl::expected<bool, Error>` indicating if the filter is enabled or an error.
     */
    [[nodiscard]] etl::expected<bool, Error> getSpatialMedianFilterEnabled();

    /**
     * @brief Enables or disables the spatial median filter.
     * @param enabled Boolean to enable or disable the filter.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setSpatialMedianFilterEnabled(bool enabled);

    /**
     * @brief Retrieves the current preset ID.
     * @return An `etl::expected<PresetId, Error>` containing the preset ID or an error.
     */
    [[nodiscard]] etl::expected<PresetId, Error> getPresetId();

    /**
     * @brief Sets the preset ID.
     * @param id The preset ID to set.
     * @return An `etl::expected<void, Error>` indicating success or failure.
     */
    [[nodiscard]] etl::expected<void, Error> setPresetId(const PresetId& id);
    
private:
    etl::unique_ptr<DeviceInterfaceWEOM> m_deviceInterface;
    uint8_t m_lastPacketId;
    SleepFunction m_sleepFunction;

    template <const AddressRange& addressRange>
    etl::expected<etl::array<uint8_t, addressRange.getSize()>, Error> readAddressRange();


    etl::expected<void, Error> writeData(const etl::span<uint8_t>& data, uint32_t address);
};


template <const AddressRange& addressRange>
etl::expected<etl::array<uint8_t, addressRange.getSize()>, Error> WEOM::readAddressRange()
{
    if (!m_deviceInterface)
    {
        return etl::unexpected<Error>(Error::PROTOCOL__NO_DATALINK);
    }
    return m_deviceInterface->readAddressRange<addressRange>();
}

} // namespace wl

#endif // WL_WEOM_H