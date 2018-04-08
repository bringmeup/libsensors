/*
 * Copyright (C) 2008-2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>

#include <utils/Atomic.h>
#include <utils/Log.h>

#include <hardware/sensors.h>
#include "sensors.h"
#include "base/SensorsManager.h"

#define LOG_TAG "sensors"
#include "util.h"

SensorsManager *kSensorManager;

static int nexwell_open_sensors(const struct hw_module_t *module,
                                const char *id,
                                struct hw_device_t **device);

static int nexwell_get_sensors_list(struct sensors_module_t*,
                                    struct sensor_t const** list);


static struct hw_module_methods_t nexwell_sensors_methods = {
    open: nexwell_open_sensors,
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
    common: {
      tag: HARDWARE_MODULE_TAG,
      version_major: 1,
      version_minor: 3,
      id: SENSORS_HARDWARE_MODULE_ID,
      name: "dummy-sensor HAL module",
      author: "Nexwell  ",
      methods: &nexwell_sensors_methods,
      dso: NULL,
      reserved: { 0 },
    },
    get_sensors_list: nexwell_get_sensors_list,
    set_operation_mode: NULL,
};

static int nexwell_open_sensors(
        const struct hw_module_t* module, const char* id,
        struct hw_device_t** device)
{
    kSensorManager = new SensorsManager(module);
    *device = &kSensorManager->device.common;
    return 0;
}

static int nexwell_get_sensors_list(struct sensors_module_t*, struct sensor_t const** list) {
    return kSensorManager->getSensorsList(list);
}

