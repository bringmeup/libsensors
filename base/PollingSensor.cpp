/*
 * LightSensor.cpp
 *
 *  Created on: Apr 9, 2018
 *      Author: nazgee
 */

#include "PollingSensor.h"

#include <utils/Log.h>
#include <utils/Timers.h>
#include <cutils/properties.h>
#include <cutils/log.h>
#include <future>

#define LOG_TAG "sensors::PollingSensor"
#include "util.h"


PollingSensor::PollingSensor(sensor_t sensor, events_que_t& queue, std::string filename)
    : Sensor(sensor, queue),
    mThread(std::bind(&PollingSensor::run, this)),
    mPeriod(std::chrono::seconds(2)),
    mInfileName(filename) {

    mInfile = std::ifstream(mInfileName);

    if (!mInfile.is_open()) {
        ALOGE("Failed to open %s", mInfileName.c_str());
    } else {
        DEBUG("Opened %s", mInfileName.c_str());
    }
}

PollingSensor::~PollingSensor() {
    DEBUG("");
}

int PollingSensor::activate(int enabled) {
    std::lock_guard<std::mutex> lk(mMutex);

    DEBUG("this=%p, enabled=%d", this, enabled);

    if (!mInfile.is_open()) {
        ALOGE("Failed to read from sensor file, not activating");
        return -1;
    }

    if (enabled) {
        mThread.resume();
    } else {
        mThread.pause();
    }

    return 0;
}

int PollingSensor::batch(int flags, int64_t period_ns, int64_t timeout) {
    std::lock_guard<std::mutex> lk(mMutex);

    mPeriod = std::chrono::nanoseconds(period_ns);
    return 0;
}

int PollingSensor::setDelay(int ns) {
    std::lock_guard<std::mutex> lk(mMutex);

    mPeriod = std::chrono::nanoseconds(ns);
    return 0;
}

void PollingSensor::run() {
    std::chrono::nanoseconds period;

    {
        std::lock_guard<std::mutex> lk(mMutex);
        period = mPeriod;
    }
    std::this_thread::sleep_for(period);

    mInfile.close();
    mInfile = std::ifstream(mInfileName);

    if (!mInfile) {
        return;
    }

    int value;
    mInfile >> value;
    DEBUG("value=%d", value);

    auto handle = reinterpret_cast<std::int32_t>(this);
    mQueue.push(sensors_event_t {
        .version = sizeof(sensors_event_t),
        .sensor = handle,
        .type = SENSOR_TYPE_LIGHT,
        .timestamp = getTimestamp(),
        .light = static_cast<float>(value),
    });

}

