#include "wl/weom.h"
#include "boostdatalinkinterface.h"

#include <iostream>

#define HAS_VALUE_OR_RETURN(result) \
    do { \
        if (!result.has_value()) \
        { \
            std::cerr << "Error: " << result.error().c_str() << std::endl; \
            return 1; \
        } \
    } while(0) 

void sleepFunction(const wl::Clock::duration& duration)
{
    std::this_thread::sleep_for(duration);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <device location> <baudrate>" << std::endl;
        return 1;
    }

    wl::WEOM camera(sleepFunction);
    auto dataLink = BoostDataLinkInterface::connect(argv[1], std::atoi(argv[2]));
    if (!dataLink)
    {
        std::cerr << "Failed to connect to device" << std::endl;
        return 1;
    }
    
    auto result = camera.setDataLinkInterface(etl::move(dataLink));
    HAS_VALUE_OR_RETURN(result);

    std::cout << "GENERAL" << std::endl;

    auto serialNumber = camera.getSerialNumber();
    HAS_VALUE_OR_RETURN(serialNumber);
    std::cout << "\tSerial number: " << serialNumber.value().c_str() << std::endl;

    auto articleNumber = camera.getArticleNumber();
    HAS_VALUE_OR_RETURN(articleNumber);
    std::cout << "\tArticle number: " << articleNumber.value().c_str() << std::endl;

    auto firmwareVersion = camera.getFirmwareVersion();
    HAS_VALUE_OR_RETURN(firmwareVersion);
    std::cout << "\tFirmware version: " << firmwareVersion.value().toString().c_str() << std::endl;

    auto ledRedBrightness = camera.getLedRedBrightness();
    HAS_VALUE_OR_RETURN(ledRedBrightness);
    std::cout << "\tLED RED brightness: " << std::to_string(ledRedBrightness.value()) << std::endl;

    auto ledGreenBrightness = camera.getLedGreenBrightness();
    HAS_VALUE_OR_RETURN(ledGreenBrightness);
    std::cout << "\tLED GREEN brightness: " << std::to_string(ledGreenBrightness.value()) << std::endl;

    auto ledBlueBrightness = camera.getLedBlueBrightness();
    HAS_VALUE_OR_RETURN(ledBlueBrightness);
    std::cout << "\tLED BLUE brightness: " << std::to_string(ledBlueBrightness.value()) << std::endl;

    auto triggerMode = camera.getTriggerMode();
    HAS_VALUE_OR_RETURN(triggerMode);
    std::cout << "\tTrigger mode: " << triggerMode.value().c_str() << std::endl;

    for (int i = 0; i < 3; i++)
    {
        auto auxPin = camera.getAuxPin(i);
        HAS_VALUE_OR_RETURN(auxPin);
        std::cout << "\tAUX pin " << i << ": " << auxPin.value().c_str() << std::endl;
    }

    std::cout << "VIDEO" << std::endl;

    auto paletteIndex = camera.getPaletteIndex();
    HAS_VALUE_OR_RETURN(paletteIndex);
    auto paletteName = camera.getPaletteName(paletteIndex.value());
    std::cout << "\tPalette: " << std::to_string(paletteIndex.value()) << " - " << paletteName.value().c_str() << std::endl;

    auto frameRate = camera.getFramerate();
    HAS_VALUE_OR_RETURN(frameRate);
    std::cout << "\tFrame rate: " << frameRate.value().c_str() << std::endl;

    auto imageFlip = camera.getImageFlip();
    HAS_VALUE_OR_RETURN(imageFlip);
    std::cout << "\tImage flip: " << std::endl;
    std::cout << "\t\tHorizontal: " << (imageFlip.value().getHorizontalFlip() ? "on" : "off") << std::endl;
    std::cout << "\t\tVertical: " << (imageFlip.value().getVerticalFlip() ? "on" : "off") << std::endl;

    auto imageFreeze = camera.getImageFreeze();
    HAS_VALUE_OR_RETURN(imageFreeze);
    std::cout << "\tImage freeze: " << (imageFreeze.value() ? "on" : "off") << std::endl;

    auto videoFormat = camera.getVideoFormat();
    HAS_VALUE_OR_RETURN(videoFormat);
    std::cout << "\tVideo format: " << videoFormat.value().c_str() << std::endl;

    auto imageGenerator = camera.getImageGenerator();
    HAS_VALUE_OR_RETURN(imageGenerator);
    std::cout << "\tImage generator: " << imageGenerator.value().c_str() << std::endl;
    
    auto reticleType = camera.getReticleType();
    HAS_VALUE_OR_RETURN(reticleType);
    auto reticlePositionX = camera.getReticlePositionX();
    HAS_VALUE_OR_RETURN(reticlePositionX);
    auto reticlePositionY = camera.getReticlePositionY();
    HAS_VALUE_OR_RETURN(reticlePositionY);
    std::cout << "\tReticle: " << reticleType.value().c_str() 
              << " - (" << std::to_string(reticlePositionX.value())
              << "," << std::to_string(reticlePositionY.value()) << ")" << std::endl;


    std::cout << "NUC" << std::endl;

    auto shutterCounter = camera.getShutterCounter();
    HAS_VALUE_OR_RETURN(shutterCounter);
    std::cout << "\tShutter counter: " << std::to_string(shutterCounter.value()) << std::endl;

    auto timeFromLastNucOffsetUpdate = camera.getTimeFromLastNucOffsetUpdate();
    HAS_VALUE_OR_RETURN(timeFromLastNucOffsetUpdate);
    std::cout << "\tTime from last NUC offset update: " << std::to_string(timeFromLastNucOffsetUpdate.value()) << std::endl;

    auto shutterUpdateMode = camera.getShutterUpdateMode();
    HAS_VALUE_OR_RETURN(shutterUpdateMode);
    std::cout << "\tShutter update mode: " << shutterUpdateMode.value().c_str() << std::endl;

    auto internalShutterPosition = camera.getInternalShutterPosition();
    HAS_VALUE_OR_RETURN(internalShutterPosition);
    std::cout << "\tInternal shutter position: " << internalShutterPosition.value().c_str() << std::endl;

    auto shutterMaxPeriod = camera.getShutterMaxPeriod();
    HAS_VALUE_OR_RETURN(shutterMaxPeriod);
    std::cout << "\tShutter maximum period: " << std::to_string(shutterMaxPeriod.value()) << std::endl;

    auto shutterAdaptiveThreshold = camera.getShutterAdaptiveThreshold();
    HAS_VALUE_OR_RETURN(shutterAdaptiveThreshold);
    std::cout << "\tShutter adaptive threshold: " << std::to_string(shutterAdaptiveThreshold.value()) << std::endl;


    std::cout << "FILTERS" << std::endl;

    auto timeDomainAveraging = camera.getTimeDomainAveraging();
    HAS_VALUE_OR_RETURN(timeDomainAveraging);
    std::cout << "\tTime domain averaging: " << timeDomainAveraging.value().c_str() << std::endl;

    auto imageEqualizationType = camera.getImageEqualizationType();
    HAS_VALUE_OR_RETURN(imageEqualizationType);
    std::cout << "\tImage equalization type: " << imageEqualizationType.value().c_str() << std::endl;

    auto mgcContrastBrightness = camera.getMgcContrastBrightness();
    HAS_VALUE_OR_RETURN(mgcContrastBrightness);
    std::cout << "\tMGC: " << std::endl;
    std::cout << "\t\tContrast: " << std::to_string(mgcContrastBrightness.value().getContrastRaw()) << std::endl;
    std::cout << "\t\tBrightness: " << std::to_string(mgcContrastBrightness.value().getBrightnessRaw()) << std::endl;

    auto frameBlockMedianConbright = camera.getFrameBlockMedianConbright();
    HAS_VALUE_OR_RETURN(frameBlockMedianConbright);
    std::cout << "\tFrame block median: " << std::endl;
    std::cout << "\t\tContrast: " << std::to_string(frameBlockMedianConbright.value().getContrastRaw()) << std::endl;
    std::cout << "\t\tBrightness: " << std::to_string(frameBlockMedianConbright.value().getBrightnessRaw()) << std::endl;

    auto agcNhSmoothingFrames = camera.getAgcNhSmoothingFrames();
    HAS_VALUE_OR_RETURN(agcNhSmoothingFrames);
    std::cout << "\tAGC NH smoothing frames: " << agcNhSmoothingFrames.value().c_str() << std::endl;

    auto spatialMedianFilterEnabled = camera.getSpatialMedianFilterEnabled();
    HAS_VALUE_OR_RETURN(spatialMedianFilterEnabled);
    std::cout << "\tSpatial median filter: " << (spatialMedianFilterEnabled.value() ? "on" : "off") << std::endl;

    auto linearGainWeight = camera.getLinearGainWeight();
    HAS_VALUE_OR_RETURN(linearGainWeight);
    std::cout << "\tLinear gain weight: " << std::to_string(linearGainWeight.value()) << std::endl;

    auto clipLimit = camera.getClipLimit();
    HAS_VALUE_OR_RETURN(clipLimit);
    std::cout << "\tClip limit: " << std::to_string(clipLimit.value()) << std::endl;

    auto plateauTailRejection = camera.getPlateauTailRejection();
    HAS_VALUE_OR_RETURN(plateauTailRejection);
    std::cout << "\tPlateau tail rejection: " << std::to_string(plateauTailRejection.value()) << std::endl;

    auto smartTimeDomainAverageThreshold = camera.getSmartTimeDomainAverageThreshold();
    HAS_VALUE_OR_RETURN(smartTimeDomainAverageThreshold);
    std::cout << "\tSmart time domain average threshold: " << std::to_string(smartTimeDomainAverageThreshold.value()) << std::endl;

    auto smartMedianThreshold = camera.getSmartMedianThreshold();
    HAS_VALUE_OR_RETURN(smartMedianThreshold);
    std::cout << "\tSmart median threshold: " << std::to_string(smartMedianThreshold.value()) << std::endl;

    auto gammaCorrection = camera.getGammaCorrection();
    HAS_VALUE_OR_RETURN(gammaCorrection);
    std::cout << "\tGamma correction: " << std::to_string(gammaCorrection.value()) << std::endl;

    auto maxAmplification = camera.getMaxAmplification();
    HAS_VALUE_OR_RETURN(maxAmplification);
    std::cout << "\tMax amplification: " << std::to_string(maxAmplification.value()) << std::endl;

    auto dampingFactor = camera.getDampingFactor();
    HAS_VALUE_OR_RETURN(dampingFactor);
    std::cout << "\tDamping factor: " << std::to_string(dampingFactor.value()) << std::endl;


    std::cout << "PRESETS" << std::endl;

    auto presetId = camera.getPresetId();
    HAS_VALUE_OR_RETURN(dampingFactor);
    std::cout << "\tCurrent preset: " << std::endl
              << "\t\tRange: " << presetId.value().getRange().c_str() << std::endl
              << "\t\tLens: " << presetId.value().getLens().c_str() << std::endl
              << "\t\tVersion: " << presetId.value().getPresetVersion().c_str() << std::endl
              << "\t\tLens variant: " << presetId.value().getLensVariant().c_str() << std::endl;

    auto presetIdCount = camera.getPresetIdCount();
    HAS_VALUE_OR_RETURN(presetIdCount);
    std::cout << "\tNumber of presets: " << std::to_string(presetIdCount.value()) << std::endl;

    std::cout << "\tAll presets: " << std::endl;
    for (uint8_t presetIndex = 0; presetIndex < presetIdCount.value(); ++presetIndex)
    {
        auto preset = camera.getPresetId(presetIndex);
        HAS_VALUE_OR_RETURN(preset);
        std::cout << "\t\tPreset " << std::to_string(presetIndex) << ": " << std::endl
                  << "\t\t\tRange: " << preset.value().getRange().c_str() << std::endl
                  << "\t\t\tLens: " << preset.value().getLens().c_str() << std::endl
                  << "\t\t\tVersion: " << preset.value().getPresetVersion().c_str() << std::endl
                  << "\t\t\tLens variant: " << preset.value().getLensVariant().c_str() << std::endl;
    }

    return 0;
}