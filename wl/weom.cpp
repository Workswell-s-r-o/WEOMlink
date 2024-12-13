#include "wl/weom.h"

#include <cassert>

namespace wl {

WEOM::WEOM(SleepFunction sleepFunction)
    : m_deviceInterface(nullptr)
    , m_lastPacketId(0)
    , m_sleepFunction(sleepFunction)
{

}

WEOM::~WEOM()
{

}

etl::expected<void, Error> WEOM::setDataLinkInterface(etl::unique_ptr<IDataLinkInterface> dataLinkInterface)
{
    m_lastPacketId = 0;
    auto protocolInterface = etl::unique_ptr<ProtocolInterfaceTCSI>(new ProtocolInterfaceTCSI(m_sleepFunction));
    protocolInterface->setDataLinkInterface(etl::move(dataLinkInterface));
    m_deviceInterface = etl::unique_ptr<DeviceInterfaceWEOM>(new DeviceInterfaceWEOM(etl::move(protocolInterface), m_sleepFunction));

    auto result = readAddressRange<MemorySpaceWEOM::DEVICE_IDENTIFICATOR>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    static constexpr uint8_t WEOM_IDENTIFICATOR_BYTE_0 = 0x57;
    static constexpr uint8_t WEOM_IDENTIFICATOR_BYTE_1 = 0x06;
    static constexpr uint8_t WEOM_IDENTIFICATOR_BYTE_2 = 0x4D;
    if ((result.value().at(0) != WEOM_IDENTIFICATOR_BYTE_0) 
        || (result.value().at(1) != WEOM_IDENTIFICATOR_BYTE_1) 
        || (result.value().at(2) != WEOM_IDENTIFICATOR_BYTE_2))
    {
        return etl::unexpected<Error>(Error::DEVICE__NO_PROTOCOL);
    }
    return {};
}

etl::expected<Status, Error> WEOM::getStatus()
{
    auto result = readAddressRange<MemorySpaceWEOM::STATUS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return Status(result.value().at(0) || (result.value().at(1) << 8) || (result.value().at(1) << 16) || (result.value().at(1) << 24));
}

etl::expected<Triggers, Error> WEOM::getTriggers()
{
    auto result = readAddressRange<MemorySpaceWEOM::TRIGGER>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return Triggers(result.value().at(0) || (result.value().at(1) << 8) || (result.value().at(1) << 16) || (result.value().at(1) << 24));
}

etl::expected<void, Error> WEOM::activateTrigger(Trigger trigger)
{
    etl::array<uint8_t, MemorySpaceWEOM::TRIGGER.getSize()> data = {};
    data.at(0) = static_cast<uint32_t>(trigger) & 0x000000FF;
    data.at(1) = (static_cast<uint32_t>(trigger) & 0x0000FF00) >> 8;
    data.at(2) = (static_cast<uint32_t>(trigger) & 0x00FF0000) >> 16;
    data.at(3) = (static_cast<uint32_t>(trigger) & 0xFF000000) >> 24;
    return writeData(data, MemorySpaceWEOM::TRIGGER.getFirstAddress());
}

etl::expected<etl::string<WEOM::SERIAL_NUMBER_STRING_SIZE>, Error> WEOM::getSerialNumber()
{
    auto result = readAddressRange<MemorySpaceWEOM::SERIAL_NUMBER_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return etl::string<WEOM::SERIAL_NUMBER_STRING_SIZE>(result.value().begin(), result.value().end());
}

etl::expected<etl::string<WEOM::ARTICLE_NUMBER_STRING_SIZE>, Error> WEOM::getArticleNumber()
{
    auto result = readAddressRange<MemorySpaceWEOM::ARTICLE_NUMBER_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return etl::string<WEOM::ARTICLE_NUMBER_STRING_SIZE>(result.value().begin(), result.value().end());
}

etl::expected<FirmwareVersion, Error> WEOM::getFirmwareVersion()
{
    auto result = readAddressRange<MemorySpaceWEOM::MAIN_FIRMWARE_VERSION>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    } 
    return FirmwareVersion(result.value().at(3), result.value().at(2), (result.value().at(1) << 8) | result.value().at(0));
}

etl::expected<uint8_t, Error> WEOM::getPaletteIndex()
{
    auto result = readAddressRange<MemorySpaceWEOM::PALETTE_INDEX_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    } 
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setPaletteIndex(uint8_t index)
{
    etl::array<uint8_t, MemorySpaceWEOM::PALETTE_INDEX_CURRENT.getSize()> data = {};
    data.at(0) = index;
    return writeData(data, MemorySpaceWEOM::PALETTE_INDEX_CURRENT.getFirstAddress());
}

etl::expected<Framerate, Error> WEOM::getFramerate()
{
    auto result = readAddressRange<MemorySpaceWEOM::FRAME_RATE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    } 
    return static_cast<Framerate>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setFramerate(Framerate framerate)
{
    etl::array<uint8_t, MemorySpaceWEOM::FRAME_RATE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(framerate);
    return writeData(data, MemorySpaceWEOM::FRAME_RATE_CURRENT.getFirstAddress());
}

etl::expected<ImageFlip, Error> WEOM::getImageFlip()
{
    auto result = readAddressRange<MemorySpaceWEOM::IMAGE_FLIP_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return ImageFlip(result.value().at(0) & 0b01, result.value().at(0) & 0b10);
}

etl::expected<void, Error> WEOM::setImageFlip(const ImageFlip& flip)
{
    etl::array<uint8_t, MemorySpaceWEOM::IMAGE_FLIP_CURRENT.getSize()> data = {};
    if (flip.getVerticalFlip())
    {
        data.at(0) |= 0b01; 
    }
    if (flip.getHorizontalFlip())
    {
        data.at(0) |= 0b10;
    }
    return writeData(data, MemorySpaceWEOM::IMAGE_FLIP_CURRENT.getFirstAddress());
}

etl::expected<bool, Error> WEOM::getImageFreeze()
{
    auto result = readAddressRange<MemorySpaceWEOM::STATUS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0) == 1;
}

etl::expected<void, Error> WEOM::setImageFreeze(bool freeze)
{
    etl::array<uint8_t, MemorySpaceWEOM::IMAGE_FREEZE.getSize()> data = {};
    data.at(0) = freeze ? 1 : 0;
    return writeData(data, MemorySpaceWEOM::IMAGE_FREEZE.getFirstAddress());
}

etl::expected<ImageGenerator, Error> WEOM::getImageGenerator()
{
    auto result = readAddressRange<MemorySpaceWEOM::TEST_PATTERN>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<ImageGenerator>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setImageGenerator(ImageGenerator generator)
{
    etl::array<uint8_t, MemorySpaceWEOM::TEST_PATTERN.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(generator);
    return writeData(data, MemorySpaceWEOM::TEST_PATTERN.getFirstAddress());
}

etl::expected<ShutterUpdateMode, Error> WEOM::getShutterUpdateMode()
{
    auto result = readAddressRange<MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<ShutterUpdateMode>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setShutterUpdateMode(ShutterUpdateMode mode)
{
    auto result = readAddressRange<MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }

    etl::array<uint8_t, MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(mode);
    return writeData(data, MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT.getFirstAddress());
}

etl::expected<TimeDomainAveraging, Error> WEOM::getTimeDomainAveraging()
{
    auto result = readAddressRange<MemorySpaceWEOM::TIME_DOMAIN_AVERAGE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<TimeDomainAveraging>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setTimeDomainAveraging(TimeDomainAveraging averaging)
{
    etl::array<uint8_t, MemorySpaceWEOM::TIME_DOMAIN_AVERAGE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(averaging);;
    return writeData(data, MemorySpaceWEOM::TIME_DOMAIN_AVERAGE_CURRENT.getFirstAddress());
}

etl::expected<ImageEqualizationType, Error> WEOM::getImageEqualizationType()
{
    auto result = readAddressRange<MemorySpaceWEOM::IMAGE_EQUALIZATION_TYPE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<ImageEqualizationType>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setImageEqualizationType(ImageEqualizationType type)
{
    etl::array<uint8_t, MemorySpaceWEOM::IMAGE_EQUALIZATION_TYPE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(type);
    return writeData(data, MemorySpaceWEOM::IMAGE_EQUALIZATION_TYPE_CURRENT.getFirstAddress());
}

etl::expected<ContrastBrightness, Error> WEOM::getMgcConstrastBrightness()
{
    auto result = readAddressRange<MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return ContrastBrightness(static_cast<uint16_t>((result.value().at(1) << 8) | result.value().at(0)), 
                              static_cast<uint16_t>((result.value().at(3) << 8) | result.value().at(2)));
}

etl::expected<void, Error> WEOM::setMgcContrastBrightness(const ContrastBrightness& contrastBrightness)
{
    etl::array<uint8_t, MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(contrastBrightness.getContrastRaw() & 0x00FF);
    data.at(1) = static_cast<uint8_t>((contrastBrightness.getContrastRaw() & 0xFF00) >> 8);
    data.at(2) = static_cast<uint8_t>(contrastBrightness.getBrightnessRaw() & 0x00FF);
    data.at(3) = static_cast<uint8_t>((contrastBrightness.getBrightnessRaw() & 0xFF00) >> 8);
    return writeData(data, MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT.getFirstAddress());
}

etl::expected<uint8_t, Error> WEOM::getAgcNhSmoothingFrames()
{
    auto result = readAddressRange<MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setAgcNhSmoothingFrames(uint8_t frames)
{
    etl::array<uint8_t, MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT.getSize()> data = {};
    data.at(0) = frames;
    return writeData(data, MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT.getFirstAddress());
}

etl::expected<bool, Error> WEOM::getSpatialMedianFilterEnabled()
{
    auto result = readAddressRange<MemorySpaceWEOM::SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0) == 1;
}

etl::expected<void, Error> WEOM::setSpatialMedianFilterEnabled(bool enabled)
{
    etl::array<uint8_t, MemorySpaceWEOM::SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT.getSize()> data = {};
    data.at(0) = enabled ? 1 : 0;
    return writeData(data, MemorySpaceWEOM::SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT.getFirstAddress());
}

etl::expected<PresetId, Error> WEOM::getPresetId()
{
    auto result = readAddressRange<MemorySpaceWEOM::CURRENT_PRESET_ID>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }

    auto range = Range::getFromDeviceValue((result.value().at(0) | (result.value().at(1) << 8)));
    if (!range.has_value())
    {
        return etl::unexpected<Error>(range.error());
    }
    auto lens = Lens::getFromDeviceValue((result.value().at(2) | (result.value().at(3) << 8)));
    if (!lens.has_value())
    {
        return etl::unexpected<Error>(lens.error());
    }

    return PresetId(range.value(), lens.value());
}

etl::expected<void, Error> WEOM::setPresetId(const PresetId& id)
{
    etl::array<uint8_t, MemorySpaceWEOM::SELECTED_PRESET_ID.getSize()> data = {};
    data.at(0) = Range::getDeviceValue(id.getRange()) & 0x00FF;
    data.at(1) = (Range::getDeviceValue(id.getRange()) & 0xFF00) >> 8;
    data.at(2) = Lens::getDeviceValue(id.getLens()) & 0x00FF;
    data.at(3) = (Lens::getDeviceValue(id.getLens()) & 0xFF00) >> 8;
    auto result = writeData(data, MemorySpaceWEOM::SELECTED_PRESET_ID.getFirstAddress());
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    result = activateTrigger(Trigger::SET_SELECTED_PRESET);
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return {};
}

etl::expected<void, Error> WEOM::writeData(const etl::span<uint8_t>& data, uint32_t address)
{
    if (!m_deviceInterface)
    {
        return etl::unexpected<Error>(wl::Error::PROTOCOL__NO_DATALINK);
    }
    return m_deviceInterface->writeData(data, address);
}

} // namespace wl
