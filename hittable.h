#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "color.h"
#include "aabb.h"
// #include "material.h"

class Hittable;
class material;

class hit_record
{
public:
    vec3 p;          // Point of intersection
    vec3 normal;     // Normal at the intersection point
    double t;        // Distance along the ray where the hit occurs
    bool front_face; // bool indicating if hit the front face
    double u;        // u and v are texture coordinates
    double v;
    material *mat; // copy of material the ray hit

    // double reflectivity;

    // Default constructor (can be left empty if not needed)
    hit_record() : t(INFINITY) {}

    // Constructor with all the necessary details
    hit_record(const vec3 &p, const vec3 &normal, double t, material *mat)
        : p(p), normal(normal), t(t), mat(mat) {}

    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = vec3::dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : (outward_normal * -1.0);
    }
};

class Hittable
{
public:
    // double reflectivity; // Reflectivity of the object
    AABB bounding_box; // Bounding box for the object

    // Hittable(double reflectivity = 0.0) : reflectivity(reflectivity) {}

    virtual ~Hittable() = default; // Virtual destructor for cleanup

    // Pure virtual method that will be implemented by derived classes
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

    // Pure virtual method to get the bounding box of the object
    virtual AABB getBoundingBox() const = 0;
};

#endif // HITTABLE_H