#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// ray class with point3 and vec3 object ->  P(t) = A + t*b - > A point and b vector
class ray {
    public:

        // P(t) = A + t*b - > A and b
        // A is the origin (point) ; b is direction (vector)
        ray() {

        }

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {
            
        }

        // first constant is for return type (vec3) ; second is for the function itself
        const point3& origin() const {
            return orig;
        }

        const vec3& direction() const{
            return dir;
        }

        // get the position of the ray at t (~ time)    
        point3 at(double t) const {
            return orig + (dir *t);
        }

    // any point in making this private?
    private:
        point3 orig;
        vec3 dir;
};


#endif