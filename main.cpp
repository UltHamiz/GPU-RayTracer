#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <chrono>
#include <thread>
#include <string>

// int main() {

//     // world building
//     hittable_list world;

//     // define material types

//     auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
//     auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
//     // auto material_left = make_shared<dielectric>(1.0/1.33); //air bubble
//     auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);

//     auto material_left   = make_shared<dielectric>(1.50);
//     auto material_bubble = make_shared<dielectric>(1.00 / 1.50);    
    
//     world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
//     world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
//     world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_right));
//     world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_left));
//     world.add(make_shared<sphere>(point3(1.0,    0.0, -1.0),   0.4, material_bubble));




//     // world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
//     // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

//     camera cam;

//     cam.aspect_ratio = 16.0/9.0;
//     cam.image_width = 400;
//     cam.samples_per_pixel = 100;
//     cam.max_depth = 50;

//     cam.vfov = 20; // vertical fov
//     cam.lookfrom = point3(-2,2,1); //cam center
//     cam.lookat = point3(0,0,-1); // cam looks at
//     cam.vup = vec3(0,1,0); // cam rotation

//     // defocus blur
//     cam.defocus_angle = 10.0;
//     cam.focus_dist = 3.4;

//     auto start = std::chrono::high_resolution_clock::now();

//     cam.render(world);

//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> diff = end - start;
//     std::clog << "Time:" << diff.count() << " seconds" << std::endl;
// }



int main(int argc, char* argv[]) {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 50;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    auto start = std::chrono::high_resolution_clock::now();

    // cam.render(world, argv[2]);

    // for (int i = 0; i < atoi(argv[1]); i++){
    //     cam.render(world, argv[i + 2]);
    // }

    std::vector<std::thread> mythreads;

    for(int i = 0; i < atoi(argv[1]); i++) {
        // std::cout << argv[2] + std::to_string(i) + ".ppm" << std::endl;
        mythreads.push_back(std::thread([&cam, &world, &argv, i]() {cam.render(world, argv[2] + std::to_string(i) + ".ppm"  );}));
    }

    for (auto& t : mythreads) {
        t.join();
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::clog << "Time:" << diff.count() << " seconds" << std::endl;
}