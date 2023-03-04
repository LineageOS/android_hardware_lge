/*
 * Copyright (C) 2019 The Android Open Source Project
 * Copyright (C) 2019-2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Vibrator.h"

#include <android-base/logging.h>
#include <thread>

#include <cmath>
#include <cstring>
#include <cutils/properties.h>

#include <linux/tspdrv.h>

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

// How many buffer entries needed per ms
static constexpr double BUFFER_ENTRIES_PER_MS = 8.21;

// Maximum amplitude value
// The vibration is a sine curve, the negative parts are 255 + negative value
// So, 127 is the maximum before it starts going the other direction
static constexpr uint8_t MAXIMUM_AMPLITUDE = 127;

// Output buffer size (immvibed uses 40 and not size of VIBE_OUTPUT_SAMPLE_SIZE)
static constexpr int32_t OUTPUT_BUFFER_SIZE = 40;

// Click effect in ms
static constexpr int32_t WAVEFORM_CLICK_EFFECT_MS = 6;

// Tick effect in ms
static constexpr int32_t WAVEFORM_TICK_EFFECT_MS = 2;

// Double click effect in ms
static constexpr uint32_t WAVEFORM_DOUBLE_CLICK_EFFECT_MS = 135;

// Heavy click effect in ms
static constexpr uint32_t WAVEFORM_HEAVY_CLICK_EFFECT_MS = 8;

ndk::ScopedAStatus Vibrator::getCapabilities(int32_t* _aidl_return) {
    LOG(VERBOSE) << "Vibrator reporting capabilities";
    *_aidl_return = IVibrator::CAP_ON_CALLBACK | IVibrator::CAP_PERFORM_CALLBACK |
                    IVibrator::CAP_AMPLITUDE_CONTROL;
    return ndk::ScopedAStatus::ok();
}

Vibrator::Vibrator(int32_t file_desc, int32_t numActuators) {

    // Initialize default values for haptic motor
    mFile_desc = file_desc;
    mNumActuators = numActuators;

    mCurrentAmplitude = MAXIMUM_AMPLITUDE;

    mClickDuration = property_get_int32("ro.vibrator.hal.click.duration", WAVEFORM_CLICK_EFFECT_MS);
    mTickDuration = property_get_int32("ro.vibrator.hal.tick.duration", WAVEFORM_TICK_EFFECT_MS);
    mHeavyClickDuration = property_get_int32(
        "ro.vibrator.hal.heavyclick.duration", WAVEFORM_HEAVY_CLICK_EFFECT_MS);

}

ndk::ScopedAStatus Vibrator::off() {
    for(int32_t i = 0; i < mNumActuators; i++)
    {
        int32_t ret = ioctl(mFile_desc, TSPDRV_DISABLE_AMP, i);
        if(ret != 0)
        {
            LOG(ERROR) << "Failed to deactivate Actuator with index " << i;
            return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_TRANSACTION_FAILED));
        }
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::on(int32_t timeoutMs,
                                const std::shared_ptr<IVibratorCallback>& callback) {
    // Calculate needed buffer entries
    int32_t bufferSize = (int32_t) round(BUFFER_ENTRIES_PER_MS * timeoutMs); 
    u_int8_t fullBuffer[bufferSize];

    // turn previous vibrations off
    off();

    for(int32_t i = 0; i < bufferSize; i++)
    {
        // The vibration is a sine curve, the negative parts are 255 + negative value
        fullBuffer[i] = (u_int8_t) (mCurrentAmplitude * sin(i/BUFFER_ENTRIES_PER_MS));
    }

    // Amount of buffer arrays with size of OUTPUT_BUFFER_SIZE
    int32_t numBuffers = (int32_t) ceil((double)bufferSize / (double)OUTPUT_BUFFER_SIZE);
    u_int8_t outputBuffers[numBuffers][OUTPUT_BUFFER_SIZE];
    memset(outputBuffers, 0, sizeof(outputBuffers));  // zero the array before we fill it with values

    for(int32_t i = 0; i < bufferSize; i++)
    {
        // split fullBuffer into multiple smaller buffers with size OUTPUT_BUFFER_SIZE
        outputBuffers[i/OUTPUT_BUFFER_SIZE][i%OUTPUT_BUFFER_SIZE] = fullBuffer[i];
    }


    for(int32_t i = 0; i < mNumActuators; i++)
    {
        for(int32_t j = 0; j < numBuffers; j++)
        {
            char output[OUTPUT_BUFFER_SIZE + SPI_HEADER_SIZE];
            memset(output, 0, sizeof(output));
            output[0] = i;  // first byte is actuator index
            output[1] = 8;  // per definition has to be 8
            output[2] = OUTPUT_BUFFER_SIZE; // size of the following output buffer
            for(int32_t k = 3; k < OUTPUT_BUFFER_SIZE+3; k++)
            {
                output[k] = outputBuffers[j][k-3];
            }
            // write the buffer to the device
            write(mFile_desc, output, sizeof(output));
            if((j+1) % 4 == 0)
            {
                // every 4 buffers, but not the first if theres only 1, we send an ENABLE_AMP signal
                int32_t ret = ioctl(mFile_desc, TSPDRV_ENABLE_AMP, i);
                if(ret != 0)
                {
                    LOG(ERROR) << "Failed to activate Actuator with index " << i;
                    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_TRANSACTION_FAILED));
                }
            }
        }
    }

    return ndk::ScopedAStatus::ok();
}

static uint8_t convertEffectStrength(EffectStrength strength) {
    uint8_t amplitude;

    switch (strength) {
    case EffectStrength::LIGHT:
        amplitude = MAXIMUM_AMPLITUDE / 2;
        break;
    case EffectStrength::MEDIUM:
    case EffectStrength::STRONG:
        amplitude = MAXIMUM_AMPLITUDE;
        break;
    }

    return amplitude;
}

ndk::ScopedAStatus Vibrator::perform(Effect effect, EffectStrength strength,
                                     const std::shared_ptr<IVibratorCallback>& callback,
                                     int32_t* _aidl_return) {
    ndk::ScopedAStatus status = ndk::ScopedAStatus::ok();
    uint32_t timeMS;
    uint8_t currentAmplitude;

    switch (effect) {
    case Effect::CLICK:
        timeMS = mClickDuration;
        break;
    case Effect::DOUBLE_CLICK:
        timeMS = WAVEFORM_DOUBLE_CLICK_EFFECT_MS;
        break;
    case Effect::TICK:
        timeMS = mTickDuration;
        break;
    case Effect::HEAVY_CLICK:
        timeMS = mHeavyClickDuration;
        break;
    default:
        *_aidl_return = 0;
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
    }

    // save current amplitude
    currentAmplitude = mCurrentAmplitude;

    mCurrentAmplitude = convertEffectStrength(strength);
    on(timeMS, callback);

    // restore current amplitude
    mCurrentAmplitude = currentAmplitude;

    *_aidl_return = (size_t)timeMS;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getSupportedEffects(std::vector<Effect>* _aidl_return) {
    *_aidl_return = {Effect::CLICK, Effect::DOUBLE_CLICK, Effect::TICK, Effect::HEAVY_CLICK};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setAmplitude(float amplitude) {
    if (amplitude <= 0.0f || amplitude > 1.0f) {
        return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_ILLEGAL_ARGUMENT));
    }

    mCurrentAmplitude = (uint8_t)(amplitude * 127);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setExternalControl(bool /* enabled */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getCompositionDelayMax(int32_t*  /* maxDelayMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getCompositionSizeMax(int32_t* /* maxSize */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedPrimitives(std::vector<CompositePrimitive>* /* supported */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getPrimitiveDuration(CompositePrimitive /* primitive */,
                                                  int32_t* /* durationMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::compose(const std::vector<CompositeEffect>& /* composite */,
                                     const std::shared_ptr<IVibratorCallback>& /* callback */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedAlwaysOnEffects(std::vector<Effect>* /* _aidl_return */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::alwaysOnEnable(int32_t id, Effect /* effect */, EffectStrength /* strength */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::alwaysOnDisable(int32_t /* id */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getResonantFrequency(float * /* resonantFreqHz */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getQFactor(float * /* qFactor */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getFrequencyResolution(float * /* freqResolutionHz */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getFrequencyMinimum(float * /* freqMinimumHz */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getBandwidthAmplitudeMap(std::vector<float> * /* _aidl_return */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getPwlePrimitiveDurationMax(int32_t * /* durationMs */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getPwleCompositionSizeMax(int32_t * /* maxSize */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::getSupportedBraking(std::vector<Braking> * /* supported */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

ndk::ScopedAStatus Vibrator::composePwle(const std::vector<PrimitivePwle> & /* composite */,
                                         const std::shared_ptr<IVibratorCallback> & /* callback */) {
    return ndk::ScopedAStatus(AStatus_fromExceptionCode(EX_UNSUPPORTED_OPERATION));
}

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
