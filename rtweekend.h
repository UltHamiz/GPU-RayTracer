#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

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

// headers

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif