#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
// #include <iostream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // // scale from 0-1 to 0-255
    // int rbyte = int(255.999 * r);
    // int gbyte = int(255.999 * g);
    // int bbyte = int(255.999 * b);

    // we are doing this now to avoid r,g,b being more than one (which may be possible b/c of antialiasing sums)
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));


    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


#endif