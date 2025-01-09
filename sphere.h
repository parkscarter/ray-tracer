// #ifndef SPHERE_H
// #define SPHERE_H

// #include "hittable.h"
// #include "ray.h"
// #include "vec3.h"
// #include <cmath>

// class Sphere : public Hittable
// {
// public:
//     ray center;    // Center of the sphere
//     double radius; // Radius of the sphere
//     material *mat;

//     Constructor to initialize the sphere with a center, radius, and reflectivity
//     Sphere(const vec3 &center, double radius, material *mat)
//         : center(center, vec3(0, 0, 0)), radius(radius), mat(mat) {}

//     Sphere(const vec3 &center, const vec3 &center2, double radius, material *mat)
//         : center(center, center2 - center), radius(radius), mat(mat) {}

//     Override the hit() method from Hittable
//     bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
//     {
//         vec3 current_center = center.at(r.time());
//         vec3 oc = r.origin - current_center;
//         vec3 oc = current_center - r.origin;
//          std::cout << r.time() << std::endl;
//            std::cout << oc << std::endl;

//         double a = r.direction.length_squared();
//         double h = vec3::dot(r.direction, oc);
//         double c = oc.length_squared() - (radius * radius);

//         double discriminant = h * h - a * c;
//         if (discriminant < 0)
//             return false;

//         double sqrt_disc = std::sqrt(discriminant);
//         double t = (-h - sqrt_disc) / a; // Nearest root

//         if (t < t_min || t > t_max)
//         {
//             t = (-h + sqrt_disc) / a; // Farthest root
//             if (t < t_min || t > t_max)
//                 return false;
//         }

//         rec.t = t;
//         rec.p = r.at(t);
//         vec3 outward_normal = (rec.p - current_center) * (1.0 / radius);

//         std::cout << outward_normal << std::endl;
//         get_sphere_uv(outward_normal, rec.u, rec.v);
//         rec.normal = outward_normal;
//         rec.set_face_normal(r, outward_normal);
//         rec.mat = mat;

//         return true;
//     }

//     AABB getBoundingBox() const override
//     {
//         vec3 min = center.direction - vec3(radius, radius, radius);
//         vec3 max = center.direction + vec3(radius, radius, radius);
//         return AABB(min, max);
//     }

//     static void get_sphere_uv(const vec3 &p, double &u, double &v)
//     {
//         double pi = 3.14159;

//         double theta = std::acos(-p.y);
//         double phi = std::atan2(-p.z, p.x) + pi;

//         u = phi / (2 * pi);
//         v = theta / pi;
//     }
// };

// #endif // SPHERE_H

// ***
//
// If your're reading this, you've found that I have two versions of the sphere class.
// The below version was used thoughout the bulk of the project, and the above version
// was used to render moving spheres. For some reason, intersections are not properly
// recorded with off-center spheres in the above version. This bug pained me for the
// entire week before the project was due but I could not find a viable solution. You can
// see remmnants of my debugging sessions above.
//
// ***

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <cmath>

class Sphere : public Hittable
{
public:
    vec3 center;   // Center of the sphere
    double radius; // Radius of the sphere
    material *mat;

    // Constructor to initialize the sphere with a center, radius, and reflectivity
    Sphere(const vec3 &center, double radius, material *mat)
        : center(center), radius(radius), mat(mat) {}
    // Override the hit() method from Hittable
    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
    {
        vec3 oc = r.origin - center;
        // std::cout << oc << std::endl;
        //   oc was -1, 2, 9
        double a = r.direction.length_squared();
        double h = vec3::dot(r.direction, oc);
        double c = oc.length_squared() - radius * radius;

        double discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        double sqrt_disc = std::sqrt(discriminant);
        double t = (-h - sqrt_disc) / a; // Nearest root

        if (t < t_min || t > t_max)
        {
            t = (-h + sqrt_disc) / a; // Farthest root
            if (t < t_min || t > t_max)
                return false;
        }
        // vec3 outward_normal = (rec.p - center) * (1.0 / radius);

        rec.t = t;
        rec.p = r.at(t);
        vec3 outward_normal = (rec.p - center) * (1.0 / radius);
        // std::cout << outward_normal << std::endl;
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.normal = outward_normal;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    AABB getBoundingBox() const override
    {
        vec3 min = center - vec3(radius, radius, radius);
        vec3 max = center + vec3(radius, radius, radius);
        return AABB(min, max);
    }

    static void get_sphere_uv(const vec3 &p, double &u, double &v)
    {
        double pi = 3.14159;

        double theta = std::acos(-p.y);
        double phi = std::atan2(-p.z, p.x) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

#endif // SPHERE_H