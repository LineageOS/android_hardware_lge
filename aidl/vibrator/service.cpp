/*
 * Copyright (C) 2019 The Android Open Source Project
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Vibrator.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <fcntl.h>
#include <vector>
#include <linux/tspdrv.h>

using aidl::android::hardware::vibrator::Vibrator;

int registerVibratorService(std::vector<int>& initializeArgs) {

    // Open device file as read/write for ioctl and write
    int file_desc = open(TSPDRV, O_RDWR);
    if(file_desc < 0)
    {
        LOG(ERROR) << "Failed to open device file: " << TSPDRV;
        return -1;
    }

    // create default device parameters
    device_parameter dev_param1 { 0, VIBE_KP_CFG_FREQUENCY_PARAM1, 0};
    device_parameter dev_param2 { 0, VIBE_KP_CFG_FREQUENCY_PARAM2, 0};
    device_parameter dev_param3 { 0, VIBE_KP_CFG_FREQUENCY_PARAM3, 0};
    device_parameter dev_param4 { 0, VIBE_KP_CFG_FREQUENCY_PARAM4, 400};
    device_parameter dev_param5 { 0, VIBE_KP_CFG_FREQUENCY_PARAM5, 13435};
    device_parameter dev_param6 { 0, VIBE_KP_CFG_FREQUENCY_PARAM6, 0};
    device_parameter dev_param_update_rate {0, VIBE_KP_CFG_UPDATE_RATE_MS, 5};

    // Set magic number for vibration driver, wont allow us to write data without!
    int ret = ioctl(file_desc, TSPDRV_SET_MAGIC_NUMBER, TSPDRV_MAGIC_NUMBER);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set magic number";
        return -ret;
    }

    // Set default device parameter 1
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param1);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 1";
        return -ret;
    }

    // Set default device parameter 2
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param2);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 2";
        return -ret;
    }

    // Set default device parameter 3
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param3);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 3";
        return -ret;
    }

    // Set default device parameter 4
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param4);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 4";
        return -ret;
    }

    // Set default device parameter 5
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param5);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 5";
        return -ret;
    }

    // Set default device parameter 6
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param6);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter 6";
        return -ret;
    }

    // Set default device parameter update rate
    ret = ioctl(file_desc, TSPDRV_SET_DEVICE_PARAMETER, &dev_param_update_rate);
    if(ret != 0)
    {
        LOG(ERROR) << "Failed to set device parameter update rate";
        return -ret;
    }

    // Get number of actuators the device has
    ret = ioctl(file_desc, TSPDRV_GET_NUM_ACTUATORS, 0);
    if(ret == 0)
    {
        LOG(ERROR) << "No actuators found!";
        return -2;
    }

    initializeArgs.push_back(file_desc);
    initializeArgs.push_back(ret);

    return 0;
}

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::vector<int> args;
    int ret = registerVibratorService(args);
    if(ret != 0)
        return EXIT_FAILURE;

    std::shared_ptr<Vibrator> vibrator = ndk::SharedRefBase::make<Vibrator>(args.at(0), args.at(1));
    const std::string vibName = std::string() + Vibrator::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(vibrator->asBinder().get(), vibName.c_str());
    CHECK_EQ(status, STATUS_OK);

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
