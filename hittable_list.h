#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h" //can comment out

#include <memory>
#include <vector>

using std::make_shared; //can comment out
using std::shared_ptr; //can comment out

// read up on shared_ptrs, allows for reuse of some obj/ptr and handles clean memory managements when all instances of this 
// shared ptr being used are done (like a counter or semaphore)

// class that extends hittable; lets you add multiple objects and has hit() return nearest object that is hit
class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;
        
        hittable_list() {
        }
        hittable_list(shared_ptr<hittable> object) {
            add(object);
        }

        void clear() {
            objects.clear();
        }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        // gets closest (to ray) hit object out of all added objects
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto& object: objects) {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};



#endif