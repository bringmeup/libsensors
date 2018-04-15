/*
 * Approximator.cpp
 *
 *  Created on: Apr 15, 2018
 *      Author: nazgee
 */

#include "Approximator.h"
#define LOG_TAG "sensors::Approximator"
#include "util.h"

Approximator::Approximator() : mHasPoints(false) {
}

Approximator::~Approximator() {
}

void Approximator::addNext(sample_t point) {
    if (!mHasPoints) {
        mPreviousPoint = point;
        mHasPoints = true;
        return;
    }

    auto x2 = std::get<0>(point);
    auto y2 = std::get<1>(point);
    auto x1 = std::get<0>(mPreviousPoint);
    auto y1 = std::get<1>(mPreviousPoint);

    float dx = x2 -x1;
    float dy = y2 - y1;
    float a = dy/dx;
    float b = y1 - (a * x1);

    DEBUG("p1=(%d, %.2f) p2=(%d, %.2f) | dx=%.2f dy=%.2f f(x)=a=%.2fx+%.2f", x1, y1, x2, y2, dx, dy, a, b);

    mRangeToLine[Range(x1, x2)] = Approximator::line_coeff_t(a, b);

    mPreviousPoint = point;
}

float Approximator::getApproximation(int x) {
    float sample = (float)x;

    Approximator::line_coeff_t coeff = getLineCoeff(x);
    return (coeff.first * sample + coeff.second);
}


Approximator::line_coeff_t Approximator::getLineCoeff(int item) {
    auto it = mRangeToLine.lower_bound(Range(item));
    bool found = it != mRangeToLine.end();

    if (found) {
        DEBUG("found=%d it=%x [%d-%d] item=%d", found, &it->first, it->first.low(), it->first.high(), item);
    } else {
        DEBUG("found=%d", found);
    }

    if (found && (it->first.low() <= item)) {
        DEBUG("coeff for %d is a=%f, b=%f", item, it->second.first, it->second.first);
        return it->second;
    } else {
        DEBUG("no coeff for %d", item);
        return std::make_pair(0.0f, 0.0f); // No mapping ?
    }
}
