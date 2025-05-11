#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

class interval {
    public:
        double min;
        double max;


        interval() : min(+infinity), max(-infinity) {

        }

        interval(double min, double max) : min(min), max(max) {

        }

        double size() const {
            return max - min;
        }

        bool contains(double x) const { // [min, max]
            return min <= x && x <= max;
        }

        bool surrounds(double x) const { // (min,max)
            return min < x && x < max;
        }

        double clamp(double x)  const { // makes sure returned value is in interval if x isn't
            if (x < min) {
                return min;
            }
            if (x > max) {
                return max;
            }
            return x;
        }

        static const interval empty;
        static const interval universe;

};

const interval interval::empty = interval(+infinity,-infinity);
const interval interval::universe = interval(-infinity, + infinity);



#endif