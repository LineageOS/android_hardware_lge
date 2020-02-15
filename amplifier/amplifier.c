/*
 * Copyright (C) 2015 The CyanogenMod Open Source Project
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "lge_amplifier"
#define LOG_NDEBUG 0

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <cutils/str_parms.h>

#include <hardware/audio_amplifier.h>
#include <hardware/hardware.h>

#include "audio_hw.h"
#include "platform.h"
#include "platform_api.h"

#include "dac_plugin.h"

typedef struct lge_amplifier_device {
    amplifier_device_t amp_dev;
    struct audio_device *adev;
    struct audio_usecase* usecase_tx;
    struct pcm* pcm_out;
    bool hifi_dac_enabled;
} lge_amplifier_device_t;

static int lge_amplifier_is_speaker(uint32_t snd_device) {
    int speaker = 0;
    switch (snd_device) {
        case SND_DEVICE_OUT_SPEAKER:
        case SND_DEVICE_OUT_SPEAKER_REVERSE:
        case SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES:
        case SND_DEVICE_OUT_VOICE_SPEAKER:
        case SND_DEVICE_OUT_VOICE_SPEAKER_2:
        case SND_DEVICE_OUT_SPEAKER_AND_HDMI:
        case SND_DEVICE_OUT_SPEAKER_AND_USB_HEADSET:
        case SND_DEVICE_OUT_SPEAKER_AND_ANC_HEADSET:
            speaker = 1;
            break;
    }

    return speaker;
}

int lge_amplifier_start_feedback(amplifier_device_t* device, uint32_t snd_device) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;
    int pcm_dev_tx_id = 0, rc = 0;
    struct pcm_config pcm_config_lge_amplifier = {
            .channels = 2,
            .rate = 48000,
            .period_size = 256,
            .period_count = 4,
            .format = PCM_FORMAT_S16_LE,
            .start_threshold = 0,
            .stop_threshold = INT_MAX,
            .silence_threshold = 0,
    };

    if (!lge_amplifier) {
        ALOGE("%d: Invalid params", __LINE__);
        return -EINVAL;
    }

    if (lge_amplifier->pcm_out || !lge_amplifier_is_speaker(snd_device)) return 0;

    lge_amplifier->usecase_tx = (struct audio_usecase*)calloc(1, sizeof(struct audio_usecase));
    if (!lge_amplifier->usecase_tx) {
        ALOGE("%d: failed to allocate usecase", __LINE__);
        return -ENOMEM;
    }
    lge_amplifier->usecase_tx->id = USECASE_AUDIO_SPKR_CALIB_TX;
    lge_amplifier->usecase_tx->type = PCM_CAPTURE;
    lge_amplifier->usecase_tx->in_snd_device = SND_DEVICE_IN_CAPTURE_VI_FEEDBACK;

    list_add_tail(&lge_amplifier->adev->usecase_list, &lge_amplifier->usecase_tx->list);
    enable_snd_device(lge_amplifier->adev, lge_amplifier->usecase_tx->in_snd_device);
    enable_audio_route(lge_amplifier->adev, lge_amplifier->usecase_tx);

    pcm_dev_tx_id = platform_get_pcm_device_id(lge_amplifier->usecase_tx->id, lge_amplifier->usecase_tx->type);
    ALOGD("pcm_dev_tx_id = %d", pcm_dev_tx_id);
    if (pcm_dev_tx_id < 0) {
        ALOGE("%d: Invalid pcm device for usecase (%d)", __LINE__, lge_amplifier->usecase_tx->id);
        rc = -ENODEV;
        goto error;
    }

    lge_amplifier->pcm_out =
            pcm_open(lge_amplifier->adev->snd_card, pcm_dev_tx_id, PCM_IN, &pcm_config_lge_amplifier);
    if (!(lge_amplifier->pcm_out || pcm_is_ready(lge_amplifier->pcm_out))) {
        ALOGE("%d: %s", __LINE__, pcm_get_error(lge_amplifier->pcm_out));
        rc = -EIO;
        goto error;
    }

    rc = pcm_start(lge_amplifier->pcm_out);
    if (rc < 0) {
        ALOGE("%d: pcm start for TX failed", __LINE__);
        rc = -EINVAL;
        goto error;
    }
    return 0;

error:
    ALOGE("%s: error case", __func__);
    if (lge_amplifier->pcm_out != 0) {
        pcm_close(lge_amplifier->pcm_out);
        lge_amplifier->pcm_out = NULL;
    }
    list_remove(&lge_amplifier->usecase_tx->list);
    disable_snd_device(lge_amplifier->adev, lge_amplifier->usecase_tx->in_snd_device);
    disable_audio_route(lge_amplifier->adev, lge_amplifier->usecase_tx);
    free(lge_amplifier->usecase_tx);

    return rc;
}

void lge_amplifier_stop_feedback(amplifier_device_t* device, uint32_t snd_device) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;

    if (!lge_amplifier) {
        ALOGE("%s: Invalid params", __func__);
        return;
    }

    if (!lge_amplifier_is_speaker(snd_device)) return;

    if (lge_amplifier->pcm_out) {
        pcm_close(lge_amplifier->pcm_out);
        lge_amplifier->pcm_out = NULL;
    }

    disable_snd_device(lge_amplifier->adev, SND_DEVICE_IN_CAPTURE_VI_FEEDBACK);

    lge_amplifier->usecase_tx = get_usecase_from_list(lge_amplifier->adev, USECASE_AUDIO_SPKR_CALIB_TX);
    if (lge_amplifier->usecase_tx) {
        list_remove(&lge_amplifier->usecase_tx->list);
        disable_audio_route(lge_amplifier->adev, lge_amplifier->usecase_tx);
        free(lge_amplifier->usecase_tx);
    }
    return;
}

static int lge_amplifier_set_output_devices(struct amplifier_device* device, uint32_t devices) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;
    bool want_to_enable_hifi_dac = property_get_bool("persist.vendor.audio.hifi.enabled", false);

    ALOGD("%s: enter", __func__);
    if (!lge_amplifier) {
        ALOGE("%s: Invalid parameter", __func__);
        return -EINVAL;
    }

    ALOGD("%s: Current device: %u\n", __func__, devices);
    if (devices != SND_DEVICE_OUT_HEADPHONES) {
        ALOGD("%s: not a Hi-Fi DAC scenario.", __func__);
        return 0;
    }

    // If DAC is currently not enabled, but the prop is set to true, change paths
    if(want_to_enable_hifi_dac) {
        platform_set_snd_device_backend(SND_DEVICE_OUT_HEADPHONES, HIFI_DAC_BACKEND, 
            HIFI_DAC_INTERFACE);
        audio_route_apply_and_update_path(lge_amplifier->adev->audio_route, HIFI_DAC_MIXER_PATH);
        lge_amplifier->hifi_dac_enabled = true;
    }
    // In all other cases, go back to normal state
    else {
        platform_set_snd_device_backend(SND_DEVICE_OUT_HEADPHONES, DEFAULT_BACKEND, 
            DEFAULT_INTERFACE);
        audio_route_apply_and_update_path(lge_amplifier->adev->audio_route, 
            DEFAULT_MIXER_PATH);
        lge_amplifier->hifi_dac_enabled = false;
    }
    return 0;
}

static int lge_amplifier_set_parameters(struct amplifier_device *device, struct str_parms *parms) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;
    int ret = 0, err, len;
    char *kv_pairs = str_parms_to_str(parms);
    char *value = NULL;

    if(lge_amplifier == NULL) {
        ret = -ENOMEM;
        ALOGE("%s: amplifier device is NULL", __func__);
        goto done;
    }

    if(lge_amplifier->adev == NULL) {
        ret = -ENOMEM;
        ALOGE("%s: adev is NULL", __func__);
        goto done;
    }

    if(kv_pairs == NULL) {
        ret = -ENOMEM;
        ALOGE("%s: key-value pair is NULL", __func__);
        goto done;
    }

    ALOGV_IF(kv_pairs != NULL, "%s: enter: %s", __func__, kv_pairs);

    len = strlen(kv_pairs);
    value = (char*)calloc(len, sizeof(char));
    if(value == NULL) {
        ret = -ENOMEM;
        ALOGE("%s: failed to allocate memory", __func__);
        goto done;
    }

    // Add parameters to enable/disable the Hi-Fi Quad DAC feature
    err = str_parms_get_str(parms, "hifi_dac", value, sizeof(value));
    if (err >= 0) {
        if (!strncmp(value, "on", 2)) {
            if(lge_amplifier->hifi_dac_enabled) {
                ALOGD("%s: already enabled!", __func__);
                ret = -EINVAL;
                goto done;
            }
            ALOGD("%s: enabling Hi-Fi Quad DAC.", __func__);
            property_set("persist.vendor.audio.hifi.enabled", "true");
        } else if (!strncmp(value, "off", 3)) {
            if (!lge_amplifier->hifi_dac_enabled) {
                ALOGD("%s: already disabled!", __func__);
                ret = -EINVAL;
                goto done;
            }
            ALOGD("%s: disabling Hi-Fi Quad DAC.", __func__);
            property_set("persist.vendor.audio.hifi.enabled", "false");
        } else {
            ALOGE("%s: Invalid parameter", __func__);
            ret = -EINVAL;
            goto done;
        }

        struct audio_usecase *usecase;
        struct listnode *node;
        list_for_each(node, &lge_amplifier->adev->usecase_list) {
            usecase = node_to_item(node, struct audio_usecase, list);
            // If this was triggered during playback, switch immediately!
            if (usecase->stream.out && (usecase->type == PCM_PLAYBACK) &&
#ifndef TARGET_LEGACY_UM
                compare_device_type(&usecase->device_list, AUDIO_DEVICE_OUT_WIRED_HEADPHONE)) {
#else
                (usecase->devices & AUDIO_DEVICE_OUT_WIRED_HEADPHONE)) {
#endif
                /*
                    HACK: when selecting devices, and the device is the same, QCOM audio HAL does
                    not re-configure devices
                    Unfortunately, we are unable to define our own device for the DAC, so the 
                    audio HAL will not re-configure, thus we cannot turn it on (if only we could
                    just clone the HAL...)
                    We can abuse the `force_device_switch()` function, that would do it for us
                    The only way, in this HAL's context, to make that function return true, is
                    to set this variable to true
                */
                usecase->stream.out->stream_config_changed = true;
                // Trigger a switch to Hi-Fi DAC
                select_devices(lge_amplifier->adev, usecase->id);
                usecase->stream.out->stream_config_changed = false;
                goto done;
            }
        }
    }
done:
    ALOGV("%s: exit with code(%d)", __func__, ret);
    if(kv_pairs != NULL)
        free(kv_pairs);
    if(value != NULL)
        free(value);
    return ret;
}

static int lge_amplifier_set_feedback(amplifier_device_t* device, void* adev, uint32_t devices,
                            bool enable) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;
    lge_amplifier->adev = (struct audio_device*)adev;

    if (enable) {
        lge_amplifier_start_feedback(device, devices);
    } else {
        lge_amplifier_stop_feedback(device, devices);
    }
    return 0;
}

// We're not really calibrating anything. We just need to save the audio_device.
static int lge_amplifier_calibrate(struct amplifier_device *device, void *adev) {
    lge_amplifier_device_t *lge_amplifier = (lge_amplifier_device_t*) device;

    if (!adev) {
        ALOGE("%s: adev is NULL", __func__);
        return -ENOMEM;
    }

    lge_amplifier->adev = adev;
    return 0;
}

static int lge_amplifier_dev_close(hw_device_t* device) {
    if (device) free(device);

    return 0;
}

static int lge_amplifier_module_open(const hw_module_t* module, const char* name,
                                hw_device_t** device) {
    lge_amplifier_device_t* lge_amplifier;

    if (strcmp(name, AMPLIFIER_HARDWARE_INTERFACE)) {
        ALOGE("%s:%d: %s does not match amplifier hardware interface name\n", __func__, __LINE__,
              name);
        return -ENODEV;
    }

    lge_amplifier = calloc(1, sizeof(*lge_amplifier));
    if (!lge_amplifier) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n", __func__, __LINE__);
        return -ENOMEM;
    }

    lge_amplifier->amp_dev.common.tag = HARDWARE_DEVICE_TAG;
    lge_amplifier->amp_dev.common.module = (hw_module_t*)module;
    lge_amplifier->amp_dev.common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    lge_amplifier->amp_dev.common.close = lge_amplifier_dev_close;

    lge_amplifier->amp_dev.set_output_devices = lge_amplifier_set_output_devices;
    lge_amplifier->amp_dev.set_parameters = lge_amplifier_set_parameters;
    lge_amplifier->amp_dev.set_feedback = lge_amplifier_set_feedback;
    lge_amplifier->amp_dev.calibrate = lge_amplifier_calibrate;

    lge_amplifier->hifi_dac_enabled = false;
    *device = (hw_device_t*)lge_amplifier;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
        .open = lge_amplifier_module_open,
};

/* clang-format off */
amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_DEVICE_API_VERSION_CURRENT,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "LGE Audio Amplifier",
        .author = "The LineageOS Project",
        .methods = &hal_module_methods,
    },
};
