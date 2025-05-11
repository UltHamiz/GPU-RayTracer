#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

using std::make_shared;
using std::shared_ptr;

// utility file

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// funcs

inline double degree_to_radians(double degree) {
    return degree * pi / 180.0;
}

inline double random_double() { // [0,1] rand double
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// inline double random_double() { // [0,1) rand double
//     return std::rand() / (RAND_MAX + 1.0 );
// }

inline double random_double(double min, double max) { // [min,max) rand double
    return random_double()*(max - min) + min;
}

// headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif