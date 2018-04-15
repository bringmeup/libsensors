/*
 * Sensor.cpp
 *
 *  Created on: Apr 9, 2018
 *      Author: nazgee
 */

#include <cstdint>
#include "Sensor.h"

#define LOG_TAG "sensors::Sensor"
#include "util.h"

Sensor::Sensor(sensor_t desc, events_que_t& queue) :
    mDescription(desc), mQueue(queue) {
    DEBUG("this=%p", this);
    auto handle = reinterpret_cast<std::uintptr_t>(this);
    mDescription.handle = handle;
}

Sensor::~Sensor() {
    DEBUG("");
}

sensor_t Sensor::getDescription() {
    DEBUG("this=%p", this);
    return mDescription;
}

int64_t Sensor::getTimestamp() {
    struct timespec t;
    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return int64_t(t.tv_sec)*1000000000LL + t.tv_nsec;
}

int Sensor::activate(int enabled) {
    INFO("this=%p, enabled=%d", this, enabled);
    return 0;
}

int Sensor::setDelay(int ns) {
    INFO("this=%p, ns=%d", this, ns);
    return 0;
}

int Sensor::batch(int flags, int64_t period_ns, int64_t timeout) {
    INFO("this=%p, flags=%d, period_ns=%lld, timeout=%lld", this, flags, period_ns, timeout);
    return 0;
}

int Sensor::flush() {
    INFO("this=%p", this);
    return 0;
}

