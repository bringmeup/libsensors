/*
 * Proximity.h
 *
 *  Created on: Apr 10, 2018
 *      Author: nazgee
 */
 
 #ifndef SENSORS_PROXIMITY_H_
#define SENSORS_PROXIMITY_H_

#include <map>
#include "../base/PollingSensor.h"
#include "../base/Approximator.h"


class Proximity: public PollingSensor {
    Approximator mApproximator;

public:
    Proximity(events_que_t& queue);
    virtual ~Proximity();
    virtual void fillRawValue(sensors_event_t& data, int raw);
private:
    void insertRange(int raw_l, float cm_l, int raw_h, float cm_h);
};

#endif /* SENSORS_PROXIMITY_H_ */
