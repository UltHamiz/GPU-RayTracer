#ifndef CAMERA_H
#define CAMERA_H

// Construct and dispatch rays into the world.
// Use the results of these rays to construct the rendered image.

#include "hittable.h"

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10; // number of samples to pull per pixel for antialiasing
        int max_depth = 10;


        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for(int i = 0; i < image_height; i++){

                // progress indicator
                std::clog << "\rScanlines Remaining:" << (image_height - i) << ' ' << std::flush;
                for(int j = 0; j < image_width; j++){

                    // auto pixel_center = pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
                    // auto ray_direction = pixel_center - center;

                    // ray r(center, ray_direction);

                    // color pixel_color = ray_color(r,world);
                    // write_color(std::cout, pixel_color);
                    
                    // samples random rays around given point, adding that sample to this pixel; averaging value
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(j,i); // antialiasing
                        pixel_color += ray_color(r,max_depth, world);

                    }
                    write_color(std::cout, pixel_sample_scale * pixel_color);

                }
            }
            std::clog << "\rDone.             \n";
        }
    
    private:
        int image_height;
        point3 center;  // camera center
        point3 pixel00_loc; // 0,0 pixel
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        double pixel_sample_scale; // color scale factor for sum of pixel samples (?)


        void initialize() {
            
            image_height = int(image_width/aspect_ratio);
            if (image_height < 1) {
                image_height = 1;
            }

            pixel_sample_scale = 1.0 / samples_per_pixel;


            // camera and viewport stuff
            // try this with a different viewport height, it shouldn't matter -> scales stuff out to same value
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height); //isnt this just the aspect ratio? -> ideal ratio not necessacrily exact
            auto focal_length = 1.0;
            center = point3(0,0,0);


            // horizontal and vertical vectors along viewport
            auto viewport_u = vec3(viewport_width,0,0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // horizontal and vertical pixel deltas
            pixel_delta_u = viewport_u/ image_width;
            pixel_delta_v = viewport_v/image_height;

            // (offset) location of upper left pixel
            auto viewport_upper_left = center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 

        }

        ray get_ray(int i, int j) const {
            // gets a ray (from camera center) pointing to a random point around pixel i,j
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // returns the vector to random point in [-.5,-.5]-[+.5,+.5] unit square -> pixel unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        color ray_color(const ray& r,int depth, const hittable& world) const {

            // recursed too much
            if (depth <= 0) {
                return color(0,0,0); //return black if too much recursion
            }


            hit_record rec;
            // if (world.hit(r, interval(0, infinity), rec)) {
            //     return 0.5 * (rec.normal + color(1,1,1));
            // }

            if (world.hit(r, interval(0.001, infinity), rec)) { // min to prevent shadow acne -> collision due to slight off error (origin of bounced vector slight below surface -> double bounce)
                // adding surface normal to random reflect vector to create lambertian reflection (more centered around surface normal)
                vec3 direction = rec.normal +  random_unit_vector(); //random for diffused/rough material
                return .5 * ray_color(ray(rec.p, direction), depth - 1,world); // ray reflects and check if that reflected ray hits something
                // basically takes on % of reflected rays color -> recursive rn
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 *(unit_direction.y() + 1.0);
            // lerp
            return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0); //remember color is a scale from 0-1 for color obj
        }
};


#endif