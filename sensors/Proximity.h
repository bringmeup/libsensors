/*
 * Proximity.h
 *
 *  Created on: Apr 10, 2018
 *      Author: nazgee
 */
 
 #include "../base/PollingSensor.h"

#ifndef SENSORS_PROXIMITY_H_
#define SENSORS_PROXIMITY_H_

class Proximity: public PollingSensor {
public:
    Proximity(events_que_t& queue);
    virtual ~Proximity();
};

#endif /* SENSORS_PROXIMITY_H_ */
