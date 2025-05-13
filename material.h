#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual ~material() = default;

        // Produce a scattered ray (or say it absorbed the incident ray).
        // If scattered, say how much the ray should be attenuated (reduction)
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

// Lambertian (diffuse) reflectance can either always scatter and attenuate light according to its reflectance R
// for diffused matterial, random scattering of outwawrd vector (ch9)
class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scattered_direction = rec.normal + random_unit_vector(); //lambertian scattering direction 
            
            if (scattered_direction.near_zero()) {
                scattered_direction = rec.normal;
            }

            scattered = ray(rec.p, scattered_direction);
            attenuation = albedo;
            return true;
        }

    private:
        // Albedo is a precise technical term in some disciplines, but in all cases it is used to define some form of fractional reflectance
        // color of base material
        color albedo;
};


class metal : public material {
    public:
        metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}
        
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz; 
};

#endif