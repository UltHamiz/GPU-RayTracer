#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h" //can comment out
#include "interval.h"

class material; // tells compiler that the class will/is defined later on

// basically class to store information related to ray hits
class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face; // true if ray is outside
        shared_ptr<material> mat;

        void set_face_normal(const ray& r, const vec3& outward_normal){
            // sets the hit record normal vector
            // NOTE: the param outward_normal is assumed to be unit length
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal; //if facing front don't change, o/w flip to oppose ray
        }
};

// base class for defining things that can be "hit" like spheres
class hittable{
    public:
        virtual ~hittable() = default;

        // virtual makes it so derived class's implementation overrides; = 0 to say derived class requires and implementation
        // upper and lower bounds on t (when to count a hit)
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


#endif