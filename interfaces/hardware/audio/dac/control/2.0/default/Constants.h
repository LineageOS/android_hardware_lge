/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/* Paths */
#define COMMON_ES9218_PATH                 "/sys/kernel/es9218_dac/"
#define AVC_VOLUME                         "avc_volume"
#define HIFI_MODE                          "headset_type"

/* Properties */
#ifndef PROPRIETARY_AUDIO_MODULE
#define PROPERTY_HIFI_DAC_ENABLED          "persist.vendor.audio.hifi.enabled"
#else
#define PROPERTY_HIFI_DAC_ENABLED          "persist.vendor.lge.audio.hifi_dac"
#endif

#define PROPERTY_DIGITAL_FILTER            "persist.vendor.lge.audio.hifi_dac.digitalFilter"
#define PROPERTY_SOUND_PRESET              "persist.vendor.lge.audio.hifi_dac.soundPreset"
#define PROPERTY_LEFT_BALANCE              "persist.vendor.lge.audio.hifi_dac.left_balance"
#define PROPERTY_RIGHT_BALANCE             "persist.vendor.lge.audio.hifi_dac.right_balance"
#define PROPERTY_HIFI_DAC_MODE             "persist.vendor.audio.ess.mode"
#define PROPERTY_HIFI_DAC_AVC_VOLUME       "persist.vendor.lge.audio.hifi_dac.avc_volume"

#ifndef PROPRIETARY_AUDIO_MODULE
#define PROPERTY_VALUE_HIFI_DAC_ENABLED    "true"
#define PROPERTY_VALUE_HIFI_DAC_DISABLED   "false"
#else
#define PROPERTY_VALUE_HIFI_DAC_ENABLED    "ON"
#define PROPERTY_VALUE_HIFI_DAC_DISABLED   "OFF"
#endif

/* Defaults */
#define AVC_VOLUME_DEFAULT                 -14
#define HIFI_MODE_DEFAULT                  0

/* Hal Commands */
#define DAC_COMMAND                         "hifi_dac"
#define SET_DAC_ON_COMMAND                  "on"
#define SET_DAC_OFF_COMMAND                 "off"

#define SET_DIGITAL_FILTER_COMMAND          "ESS_FILTER"
#define SET_SOUND_PRESET_COMMAND            "SOUND_PRESET"

#define SET_LEFT_BALANCE_COMMAND            "hifi_dac_l_volume"
#define SET_RIGHT_BALANCE_COMMAND           "hifi_dac_r_volume"
