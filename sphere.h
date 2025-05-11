#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h" //can comment out
#include "hittable.h"

// sphere hittable class
class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {
 
        }
        
        // overrides hittable hit() functions
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;
            auto discriminant = h*h - a*c;

            if (discriminant < 0) {
                return false;
            }

            auto sqrtd = std::sqrt(discriminant);
            auto root = (h - sqrtd) / a;
            // checks to get nearest root in the given range (checking both +/- directions of sqrt of discriminant )
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd ) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root; // t
            rec.p = r.at(rec.t); // point hit on hittable surface
            rec.normal = (rec.p - center) / radius; // normal vector

            // surface stuff
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, rec.normal);
            
            return true;
        }    
    

    private:
        point3 center;
        double radius;

};


#endif