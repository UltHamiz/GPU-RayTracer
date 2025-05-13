#ifndef VEC3_H
#define VEC3_H

// #include <cmath>
// #include <iostream>

// basic vector class (also used for points and color)
class vec3 {
    public:
        double e[3];

        // constructors
        vec3() : e{0,0,0} {

        }
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {

        }

        double x() const {
            return e[0];
        }
        double y() const {
            return e[1];
        }
        double z() const {
            return e[2];
        }

        vec3 operator-() const {
            return vec3(-e[0], -e[1], -e[2]);
        } 

        double operator[](int i) const {
            return e[i];
        }

        double & operator[](int i) {
            return e[i];
        }

        vec3& operator+=(const vec3& v){
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        // basically uses the *= overloaded op
        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return (e[0]*e[0]) + (e[1]*e[1]) + (e[2]*e[2]); 
        }

        bool near_zero() const {
            // vector near zero across x,y,z
            auto lowerbound = 1e-8;
            return (std::fabs(e[0]) < lowerbound) && (std::fabs(e[1]) < lowerbound) && (std::fabs(e[2]) < lowerbound); // bad bug before, paren()
        }


        // random vector generation (used for reflections of rays; diffused materials)

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }
        
        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min,max), random_double(min,max));
        }

};

// alias for vec3 class
using point3 = vec3;

// inline function -> using this bc essentially faster, only expanded on its call


inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v){
    return (u.e[0] * v.e[0]) + (u.e[1] * v.e[1]) +  (u.e[2] * v.e[2]); 
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v){
    return v / v.length();
}


// think about differences between defining function in class (in the header), defining it outside class (same/separate file)
// and defining it inline

// implementation of rejection algorithm (for vector on the surface of a hemisphere)
// this is slow; how would an analytical version of this perform?
inline vec3 random_unit_vector() {
    while(true) {
        auto p = vec3::random(-1,1); // bug, was random(), generating vectors in particular direction ("up") -> "too smooth"
        auto lengthsquared = p.length_squared();
        if (lengthsquared <= 1 && lengthsquared > 1e-160){ // lower bound to avoid underflow of zero for lensq operation
            return p / sqrt(lengthsquared);
        }
    }
}

// generates a random unit vector; checks if that random unit vector's dot product with a surface normal is correct
// i.e. that the generates vector is in the correct hemisphere/reflecting
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 unit_vec_sphere = random_unit_vector();
    if (dot(unit_vec_sphere, normal) > 0.0) { //in direction of norm/surface vector (what we want)
        return unit_vec_sphere;
    } else {
        return -unit_vec_sphere;
    }
}

// "perfect reflection vector"
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n; // "b vector"
}

#endif