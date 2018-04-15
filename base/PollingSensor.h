/*
 * LightSensor.h
 *
 *  Created on: Apr 9, 2018
 *      Author: nazgee
 */

#ifndef POLLINGSENSOR_H_
#define POLLINGSENSOR_H_

#include <fstream>
#include <thread>
#include "Sensor.h"

class PollingSensor: public Sensor {
    std::mutex mMutex;
    SafeThread mThread;
    std::chrono::nanoseconds mPeriod;
    std::ifstream mInfile;
    std::string mInfileName;
    sensors_event_t mSensorData;
public:
    PollingSensor(sensor_t sensor, events_que_t& queue, std::string filename);
    virtual ~PollingSensor();

    virtual int activate(int enabled);
    virtual int batch(int flags, int64_t period_ns, int64_t timeout);
    virtual int setDelay(int ns);

    void run();
protected:
    virtual void fillRawValue(sensors_event_t& data, int raw);
};

#endif /* POLLINGSENSOR_H_ */
