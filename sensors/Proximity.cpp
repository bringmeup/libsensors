/*
 * Proximity.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: nazgee
 */

#include <assert.h>
#include <cutils/properties.h>
#include "Proximity.h"

#define LOG_TAG "sensors::Proximity"
#include "../base/util.h"

#define MAX_RANGE_CM   150.0f

static std::string getFileName() {
    char buffer[PROPERTY_VALUE_MAX];
    property_get("ro.hardware.lightsensor", buffer, "/dupaaaa");
    std::string file = std::string(buffer) + std::string("/in_proximity_raw");
    return file;
}

Proximity::Proximity(events_que_t& queue) : PollingSensor( sensor_t {
        .name               = "vcnl4200 proximity sensor",
        .vendor             = "Vishay",
        .version            = 1,
        .handle             = UNSET_FIELD,
        .type               = SENSOR_TYPE_PROXIMITY,
        .maxRange           = MAX_RANGE_CM,
        .resolution         = UNSET_FIELD,
        .power              = 0.18f,
        .minDelay           = 5000,
        .fifoReservedEventCount = 0,
        .fifoMaxEventCount  = 512,
        .stringType         = SENSOR_STRING_TYPE_PROXIMITY,
        .requiredPermission = 0,
        .maxDelay           = 10000000,
        .flags              = SENSOR_FLAG_ON_CHANGE_MODE,
        .reserved           = { 0 }
    }, queue, getFileName()) {
    mApproximator.addNext(Approximator::sample_t(1, MAX_RANGE_CM));
    mApproximator.addNext(Approximator::sample_t(100, 100));
    mApproximator.addNext(Approximator::sample_t(1000, 50));
    mApproximator.addNext(Approximator::sample_t(2000, 30));
    mApproximator.addNext(Approximator::sample_t(3000, 20));
    mApproximator.addNext(Approximator::sample_t(4094, 10));
    mApproximator.addNext(Approximator::sample_t(4095, 0));
}

Proximity::~Proximity() {
    // TODO Auto-generated destructor stub
}

void Proximity::fillRawValue(sensors_event_t& data, int raw) {
    data.distance = std::min(MAX_RANGE_CM, mApproximator.getApproximation(raw));
}

