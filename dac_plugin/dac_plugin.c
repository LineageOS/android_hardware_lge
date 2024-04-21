/*
 * Copyright (C) 2015 The CyanogenMod Open Source Project
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "lge_hifi_dac"
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

typedef struct hifi_dac_device {
    amplifier_device_t amp_dev;
    struct audio_device *adev;
    bool enabled;
} hifi_dac_device_t;

static hifi_dac_device_t* hifi_dac = NULL;

static int hifi_dac_set_output_devices(struct amplifier_device* device, uint32_t devices) {
    bool want_to_enable_hifi_dac = property_get_bool("persist.vendor.audio.hifi.enabled", false);

    ALOGD("%s: enter", __func__);
    if (!hifi_dac) {
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
        audio_route_apply_and_update_path(hifi_dac->adev->audio_route, HIFI_DAC_MIXER_PATH);
        hifi_dac->enabled = true;
    }
    // In all other cases, go back to normal state
    else {
        platform_set_snd_device_backend(SND_DEVICE_OUT_HEADPHONES, DEFAULT_BACKEND, 
            DEFAULT_INTERFACE);
        audio_route_apply_and_update_path(hifi_dac->adev->audio_route, 
            DEFAULT_MIXER_PATH);
        hifi_dac->enabled = false;
    }
    return 0;
}

static int hifi_dac_set_parameters(struct amplifier_device *device, struct str_parms *parms) {
    int ret = 0, err, len;
    char *kv_pairs = str_parms_to_str(parms);
    char *value = NULL;

    if(hifi_dac == NULL) {
        ret = -ENOMEM;
        ALOGE("%s: amplifier device is NULL", __func__);
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
            if(hifi_dac->enabled) {
                ALOGD("%s: already enabled!", __func__);
                ret = -EINVAL;
                goto done;
            }
            ALOGD("%s: enabling Hi-Fi Quad DAC.", __func__);
            property_set("persist.vendor.audio.hifi.enabled", "true");
        } else if (!strncmp(value, "off", 3)) {
            if (!hifi_dac->enabled) {
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
        list_for_each(node, &hifi_dac->adev->usecase_list) {
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
                select_devices(hifi_dac->adev, usecase->id);
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

// We're not really calibrating anything. We just need to save the audio_device.
static int hifi_dac_calibrate(void *adev) {
    if (!adev) {
        ALOGE("%s: adev is NULL", __func__);
        return -ENOMEM;
    }

    hifi_dac->adev = adev;
    return 0;
}

static int hifi_dac_dev_close(hw_device_t* device) {
    if (device) free(device);

    return 0;
}

static int hifi_dac_module_open(const hw_module_t* module, const char* name,
                                hw_device_t** device) {
    if (strcmp(name, AMPLIFIER_HARDWARE_INTERFACE)) {
        ALOGE("%s:%d: %s does not match amplifier hardware interface name\n", __func__, __LINE__,
              name);
        return -ENODEV;
    }

    hifi_dac = calloc(1, sizeof(*hifi_dac));
    if (!hifi_dac) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n", __func__, __LINE__);
        return -ENOMEM;
    }

    hifi_dac->amp_dev.common.tag = HARDWARE_DEVICE_TAG;
    hifi_dac->amp_dev.common.module = (hw_module_t*)module;
    hifi_dac->amp_dev.common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    hifi_dac->amp_dev.common.close = hifi_dac_dev_close;

    hifi_dac->amp_dev.set_output_devices = hifi_dac_set_output_devices;
    hifi_dac->amp_dev.set_parameters = hifi_dac_set_parameters;
    hifi_dac->amp_dev.calibrate = hifi_dac_calibrate;

    hifi_dac->enabled = false;
    *device = (hw_device_t*)hifi_dac;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
        .open = hifi_dac_module_open,
};

/* clang-format off */
amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_DEVICE_API_VERSION_CURRENT,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "LGE Hi-Fi Quad DAC Plugin",
        .author = "Han Sol Jin <hansol@hansol.ca>",
        .methods = &hal_module_methods,
    },
};
