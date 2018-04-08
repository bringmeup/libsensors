/*
 * Light.h
 *
 *  Created on: Apr 10, 2018
 *      Author: nazgee
 */

#ifndef SENSORS_LIGHT_H_
#define SENSORS_LIGHT_H_

#include "../base/PollingSensor.h"

class Light: public PollingSensor {
public:
    Light(events_que_t& queue);
    virtual ~Light();
};

#endif /* SENSORS_LIGHT_H_ */
