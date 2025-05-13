#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

// #include <iostream>

// Make a Sphere -> check if hit -> return the location of the hit (actually t of ray to hit)
// double hit_sphere(const point3& center, double radius, const ray& r) {
//     vec3 oc = center - r.origin();
//     // all values from solving for t from equation for sphere
//     // auto a = dot(r.direction(), r.direction());
//     // auto b = -2.0 * dot(r.direction(), oc);
//     // auto c = dot(oc, oc) - radius*radius;
//     // auto discriminant = b*b - 4*a*c;
//     auto a = r.direction().length_squared();
//     auto h = dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius*radius;
//     auto discriminant = h*h - a*c;



//     if (discriminant < 0){
//         return -1.0;
//     } else {
//         return (h - std::sqrt(discriminant)) / a;
//     }

//     return (discriminant >= 0); // discriminant >= 0 -> hits (at least) once on sphere (solution)
// }


// return color of a ray
// color ray_color(const ray& r, const hittable& world){
//     //sphere check -> change color if hit
//     // auto t = hit_sphere(point3(0,0,-1), 0.5, r);
//     // if (t > 0.0){
//     //     vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // ray position at t - origin
//     //     return 0.5 * color(N.x() + 1,N.y() + 1, N.z() + 1); //scale from -1 to 1 -> 0 to 1
//     // }

//     hit_record rec;
//     if (world.hit(r, interval(0, infinity), rec)) {
//         return 0.5 * (rec.normal + color(1,1,1));
//     }


//     vec3 unit_direction = unit_vector(r.direction());
//     auto a = 0.5 *(unit_direction.y() + 1.0);
//     // lerp
//     return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0); //remember color is a scale from 0-1 for color obj
// }

// int main() {

    // rendered image scale/size
    
    // auto aspect_ratio = 16.0/9.0;
    // int image_width = 400;

    // int image_height = int(image_width/aspect_ratio);
    // if (image_height < 1) {
    //     image_height = 1;
    // }

    // // world building
    // hittable_list world;
    // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));


    // // camera and viewport stuff
    // // try this with a different viewport height, it shouldn't matter -> scales stuff out to same value
    // auto viewport_height = 2.0;
    // auto viewport_width = viewport_height * (double(image_width)/image_height); //isnt this just the aspect ratio? -> ideal ratio not necessacrily exact
    // auto focal_length = 1.0;
    // auto camera_center = point3(0,0,0);


    // // horizontal and vertical vectors along viewport
    // auto viewport_u = vec3(viewport_width,0,0);
    // auto viewport_v = vec3(0, -viewport_height, 0);

    // // horizontal and vertical pixel deltas
    // auto pixel_delta_u = viewport_u/ image_width;
    // auto pixel_delta_v = viewport_v/image_height;

    // // (offset) location of upper left pixel
    // auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    // auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 


    // // rendering image


    // std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // for(int i = 0; i < image_height; i++){

    //     // progress indicator
    //     std::clog << "\rScanlines Remaining:" << (image_height - i) << ' ' << std::flush;
    //     for(int j = 0; j < image_width; j++){

    //         // old image stuff
    //         // auto r = double(j) / (image_width - 1);
    //         // auto g = double(i) / (image_height - 1);
    //         // auto b = 0.0;

    //         // // Scale from 0-1 to 0-255
    //         // int ir = int(255.999 * r);
    //         // int ig = int(255.999 * g);
    //         // int ib = int(255.999 * b);

    //         // std::cout << ir << ' ' << ig << ' ' << ib << '\n';

    //         // using pixel color to replace old image stuff
    //         // color pixel_color = color(double(j)/(image_width - 1), double(i)/(image_height - 1), 0.0);
    //         // write_color(std::cout, pixel_color);

    //         // new test
    //         auto pixel_center = pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
    //         // std::cout << pixel_center << std::endl;
    //         auto ray_direction = pixel_center - camera_center;

    //         ray r(camera_center, ray_direction);

    //         color pixel_color = ray_color(r,world);
    //         write_color(std::cout, pixel_color);

    //     }
    // }

    // std::clog << "\rDone.             \n";
// }


int main() {

    // world building
    hittable_list world;

    // define material types

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<metal>(color(0.8,0.8,0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_right));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_left));




    // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}