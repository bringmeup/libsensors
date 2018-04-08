/*
 * Light.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: nazgee
 */

#include <cutils/properties.h>
#include "Light.h"

static std::string getFileName() {
    char buffer[PROPERTY_VALUE_MAX];
    property_get("ro.hardware.lightsensor", buffer, "/dupaaaa");
    std::string file = std::string(buffer) + std::string("/in_illuminance_raw");
    return file;
}

Light::Light(events_que_t& queue) : PollingSensor( sensor_t {
        .name               = "vcnl4200 light sensor",
        .vendor             = "Vishay",
        .version            = 1,
        .handle             = UNSET_FIELD,
        .type               = SENSOR_TYPE_LIGHT,
        .maxRange           = UNSET_FIELD,
        .resolution         = UNSET_FIELD,
        .power              = 0.18f,
        .minDelay           = 5000,
        .fifoReservedEventCount = 0,
        .fifoMaxEventCount  = 512,
        .stringType         = SENSOR_STRING_TYPE_LIGHT,
        .requiredPermission = 0,
        .maxDelay           = 10000000,
        .flags              = SENSOR_FLAG_ON_CHANGE_MODE,
        .reserved           = { 0 }
    }, queue, getFileName()) {

}

Light::~Light() {
    // TODO Auto-generated destructor stub
}

