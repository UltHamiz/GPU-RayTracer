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

// refractive material class (glass, water; light enters the material)
class dielectric : public material {   
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {

        }

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());

            // total internal reflection
            double cost_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cost_theta*cost_theta);

            bool cannot_refract = ri * sin_theta > 1.0; // o/w must reflect
            vec3 direction;
            
            if (cannot_refract || reflectance(cost_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction);
            return true;
        }


    private:
        // ratio of the material's refractive index over the refractive index of the enclosing media
        double refraction_index;


        static double reflectance(double cosine, double refraction_index) {
            // schlick approximatino
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 *= r0;
            return r0 + (1 - r0)*std::pow((1- cosine),5);
        }
};

#endif