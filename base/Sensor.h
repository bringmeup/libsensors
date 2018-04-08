/*
 * Sensor.h
 *
 *  Created on: Apr 9, 2018
 *      Author: nazgee
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <hardware/sensors.h>
#include "SensorsManager.h"
#include "util.h"

#define UNSET_FIELD -1

class Sensor {
    sensor_t mDescription;
protected:
    events_que_t& mQueue;

public:
    Sensor(sensor_t desc, events_que_t& queue);
    virtual ~Sensor();

    static Sensor* fromHandle(int handle) {
        return reinterpret_cast<Sensor*>(handle);
    }

    virtual sensor_t getDescription();
    int64_t getTimestamp();

    virtual int activate(int enabled);
    virtual int setDelay(int ns);
    virtual int batch(int flags, int64_t period_ns, int64_t timeout);
    virtual int flush();
};

#endif /* SENSOR_H_ */
