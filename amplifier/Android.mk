#
# Copyright 2024 The LineageOS Project
#
# * SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(AUDIO_FEATURE_ENABLED_EXT_AMPLIFIER),true)

include $(CLEAR_VARS)

ifeq ($(BOARD_LGE_HAS_HIFI_QUAD_DAC), true)
LOCAL_C_INCLUDES += \
    $(TARGET_DAC_PLUGIN_INCLUDE_DIR)

LOCAL_CFLAGS += \
    -DSUPPORT_HIFI_QUAD_DAC

ifneq ($(filter msm8996 msm8998,$(TARGET_BOARD_PLATFORM)),)
LOCAL_CFLAGS += \
    -DTARGET_LEGACY_UM
endif
endif

ifeq ($(BOARD_LGE_HAS_EXT_AMPLIFIER), true)
LOCAL_CFLAGS += \
    -DSUPPORT_EXT_AMPLIFIER
endif

LOCAL_MODULE := audio_amplifier.lge
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    amplifier.c
LOCAL_VENDOR_MODULE := true

LOCAL_C_INCLUDES += \
    $(call include-path-for, audio-route) \
    $(call include-path-for, audio-utils) \
    $(call project-path-for, qcom-audio)/hal \
    $(call project-path-for, qcom-audio)/hal/audio_extn \
    $(call project-path-for, qcom-audio)/hal/msm8974 \
    external/tinycompress/include \
    $(LOCAL_PATH)/include

LOCAL_HEADER_LIBRARIES += \
    generated_kernel_headers \
    libhardware_headers

LOCAL_SHARED_LIBRARIES += \
    audio.primary.$(TARGET_BOARD_PLATFORM) \
    libaudioroute \
    libcutils \
    libdl \
    liblog \
    libtinyalsa

LOCAL_CFLAGS += \
    -Wno-unused-parameter

include $(BUILD_SHARED_LIBRARY)

endif # AUDIO_FEATURE_ENABLED_EXT_AMPLIFIER
