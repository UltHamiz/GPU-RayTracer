#ifndef CAMERA_H
#define CAMERA_H

// Construct and dispatch rays into the world.
// Use the results of these rays to construct the rendered image.

#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10; // number of samples to pull per pixel for antialiasing
        int max_depth = 10; //number of bounces for a ray

        // more camera features
        double vfov = 90; //vertical (fov) viewing angle
        point3 lookfrom = point3(0,0,0); //camera center
        point3 lookat = point3(0,0,-1); // point camera is looking at
        vec3 vup = vec3(0,1,0); // camera relative up direction

        // camera features for defocus blur (lens disk)
        double defocus_angle = 0; //cone angle
        double focus_dist = 10; //distance from camera center to focus plan (also viewport plan)

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

        vec3 u, v, w; // basis vectors for camera position

        // horizontal and vertical radii for disk
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;


        void initialize() {
            
            image_height = int(image_width/aspect_ratio);
            if (image_height < 1) {
                image_height = 1;
            }

            pixel_sample_scale = 1.0 / samples_per_pixel;

            center = lookfrom;


            // camera and viewport stuff
            // try this with a different viewport height, it shouldn't matter -> scales stuff out to same value
            // also fov stuff

            // auto focal_length = (lookfrom - lookat).length();
            auto theta = degree_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2.0 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height); //isnt this just the aspect ratio? -> ideal ratio not necessacrily exact
            // center = point3(0,0,0);

            // u,w,v basis vector
            w = unit_vector(lookfrom - lookat); //-w is where camera is looking towards
            u = unit_vector(cross(vup,w)); // width
            v = cross(w,u); // height


            // horizontal and vertical vectors along viewport
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;

            // horizontal and vertical pixel deltas
            pixel_delta_u = viewport_u/ image_width;
            pixel_delta_v = viewport_v/image_height;

            // (offset) location of upper left pixel
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 


            // defocus blur disk vectors
            auto defocus_radius = focus_dist * std::tan(degree_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

        }

        ray get_ray(int i, int j) const {
            // gets a ray (from rand point on disk lens plane) pointing to a random point around pixel i,j
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0 ) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // returns the vector to random point in [-.5,-.5]-[+.5,+.5] unit square -> pixel unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            // returns a random point on the lens disk
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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
                // vec3 direction = rec.normal +  random_unit_vector(); //random for diffused/rough material
                // return .5 * ray_color(ray(rec.p, direction), depth - 1,world); // ray reflects and check if that reflected ray hits something
                // basically takes on % (attenuation) of reflected rays color -> recursive rn
                
                // different types of scattering and attenuation per material   
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth -1, world); // depth!!!
                }
                return color(0,0,0); // should come here when fuzzyness of vector results in vector not in direction of normal

            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 *(unit_direction.y() + 1.0);
            // lerp
            return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0); //remember color is a scale from 0-1 for color obj
        }
};


#endif