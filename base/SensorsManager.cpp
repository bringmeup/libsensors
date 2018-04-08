/*
 * SensorsManager.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: nazgee
 */


#include <string.h>
#include <utils/Log.h>
#include <utils/Timers.h>
#include <chrono>
#include <thread>
#include "../sensors/Light.h"
#include "../sensors/Proximity.h"
#include "SensorsManager.h"

#define LOG_TAG "sensors::SensorManager"
#include "util.h"

static int wrapper_close(struct hw_device_t *dev) {
    DEBUG("");
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    if (ctx) {
        delete ctx;
    }
    return 0;
}

static int wrapper_activate(struct sensors_poll_device_t *dev, int handle,
        int enabled) {
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    return ctx->activate(handle, enabled);
}

static int wrapper_setDelay(struct sensors_poll_device_t *dev, int handle,
        int64_t ns) {
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    return ctx->setDelay(handle, ns);
}

static int wrapper_poll(struct sensors_poll_device_t *dev,
        sensors_event_t* data, int count) {
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    return ctx->pollEvents(data, count);
}

static int wrapper_batch(struct sensors_poll_device_1 *dev, int handle,
        int flags, int64_t period_ns, int64_t timeout) {
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    return ctx->batch(handle, flags, period_ns, timeout);
}

static int wrapper_flush(struct sensors_poll_device_1 *dev, int handle) {
    SensorsManager *ctx = reinterpret_cast<SensorsManager *>(dev);
    return ctx->flush(handle);
}

//-----------------------------------------------------------------------------

SensorsManager::SensorsManager(const struct hw_module_t *module) {
    DEBUG("");
    memset(&device, 0, sizeof(sensors_poll_device_1_t));

    device.common.tag = HARDWARE_DEVICE_TAG;
    device.common.version = SENSORS_DEVICE_API_VERSION_1_3;
    device.common.module = const_cast<hw_module_t *>(module);
    device.common.close = wrapper_close;
    device.activate = wrapper_activate;
    device.setDelay = wrapper_setDelay;
    device.poll = wrapper_poll;

    // Batch processing
    device.batch = wrapper_batch;
    device.flush = wrapper_flush;

    mSensors.emplace_back(new Light(mEventsQueue));
    mSensors.emplace_back(new Proximity(mEventsQueue));
}

SensorsManager::~SensorsManager() {
    // TODO Auto-generated destructor stub
}

int SensorsManager::activate(int handle, int enabled) {
    auto sensor = Sensor::fromHandle(handle);
    return sensor->activate(enabled);
}

int SensorsManager::setDelay(int handle, int64_t ns) {
    auto sensor = Sensor::fromHandle(handle);
    return sensor->setDelay(ns);
}

int SensorsManager::pollEvents(sensors_event_t* data, int count) {
    mEventsQueue.pop(*data);
    int popped = 1;

    bool more = true;
    while (popped < count && more) {
        sensors_event_t& dst = data[popped];
        more = mEventsQueue.tryPop(dst);
        if (more) {
            popped++;
        }
    }

//    DEBUG("data=%p, popped=%d, timestamp[0]=%lld", data, popped, data[0].timestamp);

    return popped;
}

int SensorsManager::batch(int handle, int flags, int64_t period_ns,
        int64_t timeout) {
    auto sensor = Sensor::fromHandle(handle);
    return sensor->batch(flags, period_ns, timeout);
}

int SensorsManager::flush(int handle) {
    auto sensor = Sensor::fromHandle(handle);
    return sensor->flush();
}

int SensorsManager::getSensorsList(struct sensor_t const** list) {

    if (mSensorsDescriptions.size() == 0) {
        for (auto& sensor : mSensors) {
            mSensorsDescriptions.push_back(sensor->getDescription());
        }
    }

    *list = mSensorsDescriptions.data();
    return mSensorsDescriptions.size();
}

