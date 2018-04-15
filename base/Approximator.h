/*
 * Approximator.h
 *
 *  Created on: Apr 15, 2018
 *      Author: nazgee
 */

#ifndef BASE_APPROXIMATOR_H_
#define BASE_APPROXIMATOR_H_

#include <utility>
#include <map>

class Range {
public:
    explicit Range(int point) : mLow(point), mHigh(point) {
    }
    Range(int low, int high) : mLow(low), mHigh(high) {
    }

    bool operator<(const Range& rhs) const {
        if (mLow < rhs.mLow && mHigh < rhs.mHigh) {
            return true;
        }
        return false;
    }

    int low() const {
        return mLow;
    }

    int high() const {
        return mHigh;
    }

private:
    int mLow;
    int mHigh;
};

class Approximator {
public:
    typedef std::pair<int, float> sample_t;
    typedef std::pair<float, float> line_coeff_t;
    typedef std::map<Range, line_coeff_t> ranged_map_t;
protected:
    ranged_map_t mRangeToLine;
    bool mHasPoints;
    sample_t mPreviousPoint;
public:
    Approximator();
    virtual ~Approximator();
    void addNext(sample_t sample);
    float getApproximation(int x);
private:
    line_coeff_t getLineCoeff(int item);
};

#endif /* BASE_APPROXIMATOR_H_ */
