#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>
#include <fstream>
#include <sstream>

class Triangle : public Hittable
{
public:
  // Vertices of the triangle
  vec3 a, b, c;
  material *mat;
  // double reflectivity;

  // Constructor to initialize the triangle with vertices and reflectivity
  Triangle(const vec3 &v1, const vec3 &v2, const vec3 &v3, material *mat)
      : a(v1), b(v2), c(v3), mat(mat) {}

  // Function to compute the normal vector of the triangle's surface
  vec3 normal() const
  {
    vec3 ab = vec3::sub(b, a);                      // ab = b - a
    vec3 ac = vec3::sub(c, a);                      // ac = c - a
    vec3 cross_product = vec3::cross(ab, ac);       // cross(ab, ac)
    vec3 normal = vec3::unit_vector(cross_product); // Normalize the cross product to get the normal
    return normal;
  }

  bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
  {
    vec3 e1 = b - a;
    vec3 e2 = c - a;
    vec3 h = vec3::cross(r.direction, e2);
    double z = vec3::dot(e1, h);

    if (z > -1e-8 && z < 1e-8)
    {
      return false; // Ray is parallel to the triangle
    }

    double f = 1.0 / z;
    vec3 s = r.origin - a;
    double u = f * vec3::dot(s, h);

    if (u < 0.0 || u > 1.0)
      return false;

    vec3 q = vec3::cross(s, e1);
    double v = f * vec3::dot(r.direction, q);

    if (v < 0.0 || u + v > 1.0)
      return false;

    double t = f * vec3::dot(e2, q);
    if (t < t_min || t > t_max)
      return false;

    // If we are here, there was a valid intersection

    rec.t = t;
    rec.p = r.at(t);
    rec.normal = normal(); // Use the triangle's normal
    rec.set_face_normal(r, normal());
    rec.mat = mat;
    return true;
  }

  AABB getBoundingBox() const override
  {
    // Find the minimum and maximum corners of the triangle
    vec3 min = vec3::min(vec3::min(a, b), c);
    vec3 max = vec3::max(vec3::max(a, b), c);

    // Set a minimum box dimension of 0.01
    vec3 boxSize = max - min;
    const float MIN_SIZE = 0.01f;

    // Ensure that no dimension of the AABB has a size less than MIN_SIZE
    if (boxSize.x < MIN_SIZE)
    {
      max.x = min.x + MIN_SIZE;
    }
    if (boxSize.y < MIN_SIZE)
    {
      max.y = min.y + MIN_SIZE;
    }
    if (boxSize.z < MIN_SIZE)
    {
      max.z = min.z + MIN_SIZE;
    }

    // Return the adjusted AABB
    return AABB(min, max);
  }

  // Hittable *clone() const override
  // {
  //   return new Triangle(a, b, c, reflectivity); // Return a new Sphere copy
  // }
};

#endif // TRIANGLE_H