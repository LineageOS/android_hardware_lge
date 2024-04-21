/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/*
 * Board specific devices and paths
 *
 * If your kernel exposes these controls in another place, you can either
 * symlink to the locations given here, or override this header in your
 * device tree.
 */

#define DEFAULT_BACKEND "headphones"
#define DEFAULT_INTERFACE "SLIMBUS_6_RX"
#define DEFAULT_MIXER_PATH "headphones"

#define HIFI_DAC_BACKEND "TERT_MI2S_RX"
#define HIFI_DAC_INTERFACE "TERT_MI2S_RX"
#define HIFI_DAC_MIXER_PATH "headphones-hifi-dac"
#define HIFI_DAC_MIXER_PATH_BYPASS_MODE "ess-bypass-mode"
#define HIFI_DAC_MIXER_PATH_HIFI_MODE "ess-hifi-mode"
