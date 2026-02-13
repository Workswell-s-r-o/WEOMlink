#include "wl/weom.h"

#include <cassert>

namespace wl {
namespace {
static constexpr uint16_t FIXED_POINT_BITS = 12;
static constexpr uint16_t FIXED_POINT_FRACTIONAL_BITS = 4;
static constexpr uint16_t FIXED_POINT_MASK = (1 << FIXED_POINT_BITS) - 1;
static constexpr uint16_t FIXED_POINT_SIGN_MASK = 1 << FIXED_POINT_BITS;

etl::expected<double, Error> fixedPointToDouble(uint16_t value, bool signedFormat)
{
    int16_t extendedValue = value & FIXED_POINT_MASK;
    const bool isValueFixedNegative = value & FIXED_POINT_SIGN_MASK;

    if (extendedValue == 0 && isValueFixedNegative)
    {
        return -0.0;
    }

    if (signedFormat && isValueFixedNegative)
    {
        extendedValue |= ~FIXED_POINT_MASK;
    }
    return (static_cast<double>(extendedValue) / static_cast<double>(1 << FIXED_POINT_FRACTIONAL_BITS));
}

etl::expected<uint16_t, Error> doubleToFixedPoint(double value)
{
    uint16_t valueFixed = std::round(value * static_cast<double>(1 << FIXED_POINT_FRACTIONAL_BITS));
    valueFixed &= FIXED_POINT_MASK;

    if (std::signbit(value))
    {
        valueFixed |= FIXED_POINT_SIGN_MASK;
    }
    return valueFixed;

}

} // namespace

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
    return Status(deserialize<uint32_t>(result.value()));
}

etl::expected<Triggers, Error> WEOM::getTriggers()
{
    auto result = readAddressRange<MemorySpaceWEOM::TRIGGER>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return Triggers(deserialize<uint32_t>(result.value()));
}

etl::expected<void, Error> WEOM::activateTrigger(Trigger trigger)
{
    etl::array<uint8_t, MemorySpaceWEOM::TRIGGER.getSize()> data = {};
    serialize(static_cast<uint32_t>(trigger), data.data(), data.size());
    return writeData(data, MemorySpaceWEOM::TRIGGER);
}

etl::expected<uint8_t, Error> WEOM::getLedRedBrightness()
{
    auto result = readAddressRange<MemorySpaceWEOM::LED_R_BRIGHTNESS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setLedRedBrightness(uint8_t brightness, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::LED_R_BRIGHTNESS.getSize()> data = {};
    data.at(0) = brightness;
    return writeData(data, MemorySpaceWEOM::LED_R_BRIGHTNESS, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getLedGreenBrightness()
{
    auto result = readAddressRange<MemorySpaceWEOM::LED_G_BRIGHTNESS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setLedGreenBrightness(uint8_t brightness, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::LED_G_BRIGHTNESS.getSize()> data = {};
    data.at(0) = brightness;
    return writeData(data, MemorySpaceWEOM::LED_G_BRIGHTNESS, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getLedBlueBrightness()
{
    auto result = readAddressRange<MemorySpaceWEOM::LED_B_BRIGHTNESS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setLedBlueBrightness(uint8_t brightness, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::LED_B_BRIGHTNESS.getSize()> data = {};
    data.at(0) = brightness;
    return writeData(data, MemorySpaceWEOM::LED_B_BRIGHTNESS, memoryType);
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

etl::expected<void, Error> WEOM::setPaletteIndex(uint8_t index, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::PALETTE_INDEX_CURRENT.getSize()> data = {};
    data.at(0) = index;
    return writeData(data, MemorySpaceWEOM::PALETTE_INDEX_CURRENT, memoryType);
}

etl::expected<etl::string<MemorySpaceWEOM::PALETTE_NAME_SIZE>, Error> WEOM::getPaletteName(unsigned paletteIndex)
{
    if (!m_deviceInterface)
    {
        return etl::unexpected<Error>(Error::PROTOCOL__NO_DATALINK);
    }

    const auto addressRange = MemorySpaceWEOM::getPaletteNameAddressRange(paletteIndex);
    etl::array<uint8_t, MemorySpaceWEOM::PALETTE_NAME_SIZE> data = {};
    auto result = m_deviceInterface->readData(data, addressRange.getFirstAddress());
    if (!result.has_value())
    {
        return etl::unexpected(result.error());
    }
    return etl::string<MemorySpaceWEOM::PALETTE_NAME_SIZE>(data.begin(), data.end());
}

etl::expected<TriggerMode, Error> WEOM::getTriggerMode()
{
    auto result = readAddressRange<MemorySpaceWEOM::TRIGGER_MODE>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<TriggerMode>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setTriggerMode(TriggerMode mode, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::TRIGGER_MODE.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(mode);
    return writeData(data, MemorySpaceWEOM::TRIGGER_MODE, memoryType);
}

etl::expected<AuxPin, Error> WEOM::getAuxPin(uint8_t pin)
{
    if (pin > 2)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_PIN);
    }

    etl::expected<etl::array<uint8_t, 4>, Error> result;

    switch (pin)
    {
        case 0:
            result = readAddressRange<MemorySpaceWEOM::AUX_PIN_0>();
            break;
        case 1:
            result = readAddressRange<MemorySpaceWEOM::AUX_PIN_1>();
            break;
        case 2:
            result = readAddressRange<MemorySpaceWEOM::AUX_PIN_2>();
            break;
    }

    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }

    return static_cast<AuxPin>(result.value()[0]);
}

etl::expected<void, Error> WEOM::setAuxPin(uint8_t pin, AuxPin mode, MemoryTypeWEOM memoryType)
{
    if (pin > 2)
    {
        return etl::unexpected<Error>(Error::DEVICE__INVALID_PIN);
    }
    AddressRange addressRange = MemorySpaceWEOM::AUX_PIN_0;
    if (pin == 1) 
    {
        addressRange = MemorySpaceWEOM::AUX_PIN_1;
    }
    else if (pin == 2) 
    {
        addressRange = MemorySpaceWEOM::AUX_PIN_2;
    }

    etl::array<uint8_t, 4> data = {};
    data.at(0) = static_cast<uint8_t>(mode);
    return writeData(data, addressRange, memoryType);
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
    return writeData(data, MemorySpaceWEOM::FRAME_RATE_CURRENT);
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
    return writeData(data, MemorySpaceWEOM::IMAGE_FLIP_CURRENT);
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
    return writeData(data, MemorySpaceWEOM::IMAGE_FREEZE);
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
    return writeData(data, MemorySpaceWEOM::TEST_PATTERN);
}

etl::expected<ReticleType, Error> WEOM::getReticleType()
{
    auto result = readAddressRange<MemorySpaceWEOM::RETICLE_TYPE>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<ReticleType>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setReticleType(ReticleType mode, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::RETICLE_TYPE.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(mode);
    return writeData(data, MemorySpaceWEOM::RETICLE_TYPE, memoryType);
}

etl::expected<int32_t, Error> WEOM::getReticlePositionX()
{
    auto result = readAddressRange<MemorySpaceWEOM::RETICLE_POSITION_X>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return deserialize<int32_t>(result.value());
}

etl::expected<void, Error> WEOM::setReticlePositionX(int32_t position, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::RETICLE_POSITION_X.getSize()> data = {};
    serialize(position, data.data(), data.size());
    return writeData(data, MemorySpaceWEOM::RETICLE_POSITION_X, memoryType);
}

etl::expected<int32_t, Error> WEOM::getReticlePositionY()
{
    auto result = readAddressRange<MemorySpaceWEOM::RETICLE_POSITION_Y>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return deserialize<int32_t>(result.value());
}

etl::expected<void, Error> WEOM::setReticlePositionY(int32_t position, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::RETICLE_POSITION_Y.getSize()> data = {};
    serialize(position, data.data(), data.size());
    return writeData(data, MemorySpaceWEOM::RETICLE_POSITION_Y, memoryType);
}

etl::expected<uint32_t, Error> WEOM::getShutterCounter()
{
    auto result = readAddressRange<MemorySpaceWEOM::SHUTTER_COUNTER>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return deserialize<uint32_t>(result.value());
}

etl::expected<uint32_t, Error> WEOM::getTimeFromLastNucOffsetUpdate()
{
    auto result = readAddressRange<MemorySpaceWEOM::TIME_FROM_LAST_NUC_OFFSET_UPDATE>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return deserialize<uint32_t>(result.value());
}

etl::expected<InternalShutterPosition, Error> WEOM::getInternalShutterPosition()
{
    auto result = readAddressRange<MemorySpaceWEOM::INTERNAL_SHUTTER_POSITION>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<InternalShutterPosition>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setInternalShutterPosition(InternalShutterPosition position)
{
    etl::array<uint8_t, MemorySpaceWEOM::INTERNAL_SHUTTER_POSITION.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(position);
    return writeData(data, MemorySpaceWEOM::INTERNAL_SHUTTER_POSITION);
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

etl::expected<double, Error> WEOM::getShutterTemperature()
{
    auto result = readAddressRange<MemorySpaceWEOM::SHUTTER_TEMPERATURE>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return fixedPointToDouble(deserialize<uint16_t>(result.value()), true);
}

etl::expected<void, Error> WEOM::setShutterUpdateMode(ShutterUpdateMode mode, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(mode);
    return writeData(data, MemorySpaceWEOM::NUC_UPDATE_MODE_CURRENT, memoryType);
}

etl::expected<uint16_t, Error> WEOM::getShutterMaxPeriod()
{
    auto result = readAddressRange<MemorySpaceWEOM::NUC_MAX_PERIOD_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return deserialize<uint16_t>(result.value());
}

etl::expected<void, Error> WEOM::setShutterMaxPeriod(uint16_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::NUC_MAX_PERIOD_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(value & 0x00FF);
    data.at(1) = static_cast<uint8_t>((value & 0xFF00) >> 8);
    return writeData(data, MemorySpaceWEOM::NUC_MAX_PERIOD_CURRENT, memoryType);
}

etl::expected<double, Error> WEOM::getShutterAdaptiveThreshold()
{
    auto result = readAddressRange<MemorySpaceWEOM::NUC_ADAPTIVE_THRESHOLD_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return fixedPointToDouble(deserialize<uint16_t>(result.value()), false);
}

etl::expected<void, Error> WEOM::setShutterAdaptiveThreshold(double value, MemoryTypeWEOM memoryType)
{
    const auto fixedValue = doubleToFixedPoint(value);
    if (!fixedValue.has_value())
    {
        return etl::unexpected<Error>(fixedValue.error());
    }

    etl::array<uint8_t, MemorySpaceWEOM::NUC_ADAPTIVE_THRESHOLD_CURRENT.getSize()> data = {};
    serialize(fixedValue.value(), data.data(), sizeof(uint16_t));
    return writeData(data, MemorySpaceWEOM::NUC_ADAPTIVE_THRESHOLD_CURRENT, memoryType);
}

etl::expected<Baudrate, Error> WEOM::getUartBaudrate()
{
    auto result = readAddressRange<MemorySpaceWEOM::UART_BAUDRATE_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<Baudrate>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setUartBaudrate(Baudrate baudrate, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::UART_BAUDRATE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(baudrate);
    return writeData(data, MemorySpaceWEOM::UART_BAUDRATE_CURRENT, memoryType);
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

etl::expected<void, Error> WEOM::setTimeDomainAveraging(TimeDomainAveraging averaging, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::TIME_DOMAIN_AVERAGE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(averaging);;
    return writeData(data, MemorySpaceWEOM::TIME_DOMAIN_AVERAGE_CURRENT, memoryType);
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

etl::expected<void, Error> WEOM::setImageEqualizationType(ImageEqualizationType type, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::IMAGE_EQUALIZATION_TYPE_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(type);
    return writeData(data, MemorySpaceWEOM::IMAGE_EQUALIZATION_TYPE_CURRENT, memoryType);
}

etl::expected<ContrastBrightness, Error> WEOM::getMgcContrastBrightness()
{
    auto result = readAddressRange<MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return ContrastBrightness(static_cast<uint16_t>((result.value().at(1) << 8) | result.value().at(0)),
                              static_cast<uint16_t>((result.value().at(3) << 8) | result.value().at(2)));
}

etl::expected<void, Error> WEOM::setMgcContrastBrightness(const ContrastBrightness& contrastBrightness, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT.getSize()> data = {};
    serialize(contrastBrightness.getContrastRaw(), data.data(), sizeof(uint16_t));
    serialize(contrastBrightness.getBrightnessRaw(), data.data() + sizeof(uint16_t), sizeof(uint16_t));
    return writeData(data, MemorySpaceWEOM::MGC_CONTRAST_BRIGHTNESS_CURRENT, memoryType);
}

etl::expected<ContrastBrightness, Error> WEOM::getFrameBlockMedianConbright()
{
    auto result = readAddressRange<MemorySpaceWEOM::FRAME_BLOCK_MEDIAN_CONBRIGHT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return ContrastBrightness(static_cast<uint16_t>((result.value().at(1) << 8) | result.value().at(0)),
                              static_cast<uint16_t>((result.value().at(3) << 8) | result.value().at(2)));
}

etl::expected<AGCNHSmoothing, Error> WEOM::getAgcNhSmoothingFrames()
{
    auto result = readAddressRange<MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<AGCNHSmoothing>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setAgcNhSmoothingFrames(AGCNHSmoothing smoothing, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(smoothing);
    return writeData(data, MemorySpaceWEOM::AGC_NH_SMOOTHING_CURRENT, memoryType);
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

etl::expected<void, Error> WEOM::setSpatialMedianFilterEnabled(bool enabled, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT.getSize()> data = {};
    data.at(0) = enabled ? 1 : 0;
    return writeData(data, MemorySpaceWEOM::SPATIAL_MEDIAN_FILTER_ENABLE_CURRENT, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getLinearGainWeight()
{
    auto result = readAddressRange<MemorySpaceWEOM::LINEAR_GAIN_WEIGHT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setLinearGainWeight(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::LINEAR_GAIN_WEIGHT.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::LINEAR_GAIN_WEIGHT, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getClipLimit()
{
    auto result = readAddressRange<MemorySpaceWEOM::CLIP_LIMIT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setClipLimit(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::CLIP_LIMIT.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::CLIP_LIMIT, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getPlateauTailRejection()
{
    auto result = readAddressRange<MemorySpaceWEOM::PLATEAU_TAIL_REJECTION>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setPlateauTailRejection(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::PLATEAU_TAIL_REJECTION.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::PLATEAU_TAIL_REJECTION, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getSmartTimeDomainAverageThreshold()
{
    auto result = readAddressRange<MemorySpaceWEOM::SMART_TIME_DOMAIN_AVERAGE_THRESHOLD>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setSmartTimeDomainAverageThreshold(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::SMART_TIME_DOMAIN_AVERAGE_THRESHOLD.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::SMART_TIME_DOMAIN_AVERAGE_THRESHOLD, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getSmartMedianThreshold()
{
    auto result = readAddressRange<MemorySpaceWEOM::SMART_MEDIAN_THRESHOLD>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setSmartMedianThreshold(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::SMART_MEDIAN_THRESHOLD.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::SMART_MEDIAN_THRESHOLD, memoryType);
}

etl::expected<double, Error> WEOM::getGammaCorrection()
{
    auto result = readAddressRange<MemorySpaceWEOM::GAMMA_CORRECTION>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return fixedPointToDouble(deserialize<uint16_t>(result.value()), false);
}

etl::expected<void, Error> WEOM::setGammaCorrection(double value, MemoryTypeWEOM memoryType)
{
    const auto fixedValue = doubleToFixedPoint(value);
    if (!fixedValue.has_value())
    {
        return etl::unexpected<Error>(fixedValue.error());
    }

    etl::array<uint8_t, MemorySpaceWEOM::GAMMA_CORRECTION.getSize()> data = {};
    serialize(fixedValue.value(), data.data(), sizeof(uint16_t));
    return writeData(data, MemorySpaceWEOM::GAMMA_CORRECTION, memoryType);
}

etl::expected<double, Error> WEOM::getMaxAmplification()
{
    auto result = readAddressRange<MemorySpaceWEOM::MAX_AMPLIFICATION>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return fixedPointToDouble(deserialize<uint16_t>(result.value()), false);
}

etl::expected<void, Error> WEOM::setMaxAmplification(double value, MemoryTypeWEOM memoryType)
{
    const auto fixedValue = doubleToFixedPoint(value);
    if (!fixedValue.has_value())
    {
        return etl::unexpected<Error>(fixedValue.error());
    }

    etl::array<uint8_t, MemorySpaceWEOM::MAX_AMPLIFICATION.getSize()> data = {};
    serialize(fixedValue.value(), data.data(), sizeof(uint16_t));
    return writeData(data, MemorySpaceWEOM::MAX_AMPLIFICATION, memoryType);
}

etl::expected<uint8_t, Error> WEOM::getDampingFactor()
{
    auto result = readAddressRange<MemorySpaceWEOM::DAMPING_FACTOR>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(0);
}

etl::expected<void, Error> WEOM::setDampingFactor(uint8_t value, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::DAMPING_FACTOR.getSize()> data = {};
    data.at(0) = value;
    return writeData(data, MemorySpaceWEOM::DAMPING_FACTOR, memoryType);
}

etl::expected<PresetId, Error> WEOM::getPresetId(uint8_t index)
{
    if (!m_deviceInterface)
    {
        return etl::unexpected<Error>(Error::PROTOCOL__NO_DATALINK);
    }

    etl::array<uint8_t, MemorySpaceWEOM::SELECTED_ATTRIBUTE_AND_PRESET_INDEX.getSize()> data = {};
    data.at(0) = 2;
    data.at(2) = index;
    auto writeResult = writeData(data, MemorySpaceWEOM::SELECTED_ATTRIBUTE_AND_PRESET_INDEX);
    if (!writeResult.has_value())
    {
        return etl::unexpected<Error>(writeResult.error());
    }

    auto result = readAddressRange<MemorySpaceWEOM::ATTRIBUTE_ADDRESS>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }

    auto address = deserialize<uint32_t>(result.value());
    etl::array<uint8_t, 4> presetData = {};
    if (auto result = m_deviceInterface->readData(presetData, address);!result.has_value())
    {
        return etl::unexpected(result.error());
    }

    return PresetId(deserialize<uint32_t>(presetData));
}

etl::expected<std::uint8_t, Error> WEOM::getPresetIdCount()
{
    auto result = readAddressRange<MemorySpaceWEOM::NUMBER_OF_PRESETS_AND_ATTRIBUTES>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return result.value().at(2);
}

etl::expected<PresetId, Error> WEOM::getPresetId()
{
    auto result = readAddressRange<MemorySpaceWEOM::CURRENT_PRESET_ID>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return PresetId(deserialize<uint32_t>(result.value()));
}

etl::expected<void, Error> WEOM::setPresetId(const PresetId& id)
{
    etl::array<uint8_t, MemorySpaceWEOM::SELECTED_PRESET_ID.getSize()> data = {};
    serialize(id.toDeviceValue(), data.data(), sizeof(uint32_t));
    auto result = writeData(data, MemorySpaceWEOM::SELECTED_PRESET_ID);
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

etl::expected<void, Error> WEOM::saveCurrentPresetIndexToFlash()
{
    auto readResult = readAddressRange<MemorySpaceWEOM::CURRENT_PRESET_INDEX>();
    if (!readResult.has_value())
    {
        return etl::unexpected<Error>(readResult.error());
    }
    auto result = writeData(readResult.value(), MemorySpaceWEOM::SELECTED_PRESET_INDEX, MemoryTypeWEOM::FLASH_MEMORY);
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return {};
}

etl::expected<VideoFormat, Error> WEOM::getVideoFormat()
{
    auto result = readAddressRange<MemorySpaceWEOM::VIDEO_FORMAT>();
    if (!result.has_value())
    {
        return etl::unexpected<Error>(result.error());
    }
    return static_cast<VideoFormat>(result.value().at(0));
}

etl::expected<void, Error> WEOM::setVideoFormat(VideoFormat videoFormat, MemoryTypeWEOM memoryType)
{
    etl::array<uint8_t, MemorySpaceWEOM::VIDEO_FORMAT.getSize()> data = {};
    data.at(0) = static_cast<uint8_t>(videoFormat);
    return writeData(data, MemorySpaceWEOM::VIDEO_FORMAT, memoryType);
}

etl::expected<void, Error> WEOM::writeData(const etl::span<uint8_t>& data, const AddressRange& addressRange, MemoryTypeWEOM memoryType)
{
    if (!m_deviceInterface)
    {
        return etl::unexpected<Error>(wl::Error::PROTOCOL__NO_DATALINK);
    }
    auto firstAddress = addressRange.getFirstAddress();
    switch (memoryType) {
    case MemoryTypeWEOM::REGISTERS_CONFIGURATION:
        break;
    case MemoryTypeWEOM::FLASH_MEMORY:
        firstAddress += MemorySpaceWEOM::ADDRESS_FLASH_REGISTERS_START;
        break;
    }
    return m_deviceInterface->writeData(data, firstAddress);
}

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
