#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>
#include <sstream>

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


// Usage : ./main.exe [num_threads] [output_location] [final_output]
// Compile : cl /std:c++17 /O2 /GL /DNDEBUG /EHsc .\main.cpp /link /LTCG

int main(int argc, char* argv[]) {

    // environemnt setup
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

    // camera settings
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 400;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    auto start = std::chrono::high_resolution_clock::now();

    // cam.render(world, argv[2]);

    // multi-threading -> renders n different images of scene
    std::vector<std::thread> mythreads;

    for(int i = 0; i < atoi(argv[1]); i++) {
        // std::cout << argv[2] + std::to_string(i) + ".ppm" << std::endl;
        mythreads.push_back(std::thread([&cam, &world, &argv, i]() {cam.render(world, argv[2] + std::to_string(i) + ".ppm"  );}));
    }

    for (auto& t : mythreads) {
        t.join();
    }


    // average images
    std::string path = argv[2];
    std::vector<std::vector<int>> data;
    std::string dimensions;
    int file_no = 0;
    // iterate every ppm file file location
    for (const auto& e : std::filesystem::directory_iterator(path)) {
        if (e.is_regular_file()) {
            std::string file_p = e.path().filename().string();
            if (file_p.substr(file_p.length() - 4) == ".ppm") { // file type check
                std::cout << file_p << std::endl;
                
                // open ifstream
                std::ifstream file(path + file_p);
                // std::cout << path + file_p << std::endl;
                
                if (!file.is_open()) {
                    std::cout << "file open error" << std::endl;
                    return 1;
                }
                
                // skip first 3 lines of ppm
                std::string dummy;
                for (int i = 0; i < 3 && std::getline(file, dummy); ++i) {
                    if (i == 1) {
                        // grab dimensions from image
                        dimensions = dummy;
                    }
                }


                std::string line;
                int count = 0;
                std::vector<std::vector<int>> values;
                // read in all data from a file into 2d vector
                while (std::getline(file, line)) {
                    std::istringstream iss(line);
                    int r, g, b;
                    std::vector<int> val;
                    if (iss >> r >> g >> b) {
                        val.push_back(r);
                        val.push_back(g);
                        val.push_back(b);
                    }
                    values.push_back(val);
                    // count += 1;
                }

                // enter values into main data vector
                if (data.empty()) {
                    data = values;
                } else {
                    for (size_t j = 0; j < data.size(); j++) {
                        for (size_t k = 0; k < data[j].size(); k++){
                            data[j][k] += values[j][k];
                        }
                    }
                }

                // print out values
                // for(auto q: values) {
                //     std::cout << q[0] << " " << q[1] << " " << q[2] << std::endl;
                // }


                file.close();
                file_no += 1; 
            }
        }
    }

    // avg data values by num of files/renders
    for(auto& q1: data) {
        // std::cout << q[0] << " " << q[1] << " " << q[2] << std::endl;
        for (auto& val : q1) {
            val /= file_no;
        }
    }
    // std::cout << data.size() << std::endl;


    std::string output_file_path = argv[3];
    std::ofstream final_out(output_file_path);
    if (!final_out.is_open()) {
        std::cout << "cant open" << std::endl;
        return 1;
    }
    final_out << "P3\n" << dimensions << "\n255\n";

    for(auto& q: data) {
        final_out << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';
    }


    final_out.close();


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::clog << "Time:" << diff.count() << " seconds" << std::endl;
}