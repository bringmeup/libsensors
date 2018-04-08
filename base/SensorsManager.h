/*
 * SensorsManager.h
 *
 *  Created on: Apr 8, 2018
 *      Author: nazgee
 */

#ifndef SENSORSMANAGER_H_
#define SENSORSMANAGER_H_

#include <poll.h>
#include <hardware/hardware.h>
#include <hardware/sensors.h>
#include <vector>
#include <memory>
#include "ConcurrentQueue.h"

typedef ConcurrentQueue<sensors_event_t> events_que_t;

class Sensor;

struct SensorsManager {

    sensors_poll_device_1_t device; // must be first

    std::vector<sensor_t> mSensorsDescriptions;
    std::vector<std::unique_ptr<Sensor>> mSensors;
    events_que_t mEventsQueue;

    SensorsManager(const struct hw_module_t *module);
    ~SensorsManager();

    int activate(int handle, int enabled);
    int setDelay(int handle, int64_t ns);
    int pollEvents(sensors_event_t* data, int count);
    int batch(int handle, int flags, int64_t period_ns, int64_t timeout);
    int flush(int handle);

    int getSensorsList(struct sensor_t const** list);
};

#endif /* SENSORSMANAGER_H_ */
