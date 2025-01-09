#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vec3.h"

class AABB
{
public:
  vec3 min; // Minimum corner
  vec3 max; // Maximum corner

  AABB() = default;

  // Constructor to create an AABB from min and max corners
  AABB(const vec3 &min, const vec3 &max) : min(min), max(max) {}

  static AABB combine(const AABB &box1, const AABB &box2)
  {
    vec3 new_min = vec3::min(box1.min, box2.min);
    vec3 new_max = vec3::max(box1.max, box2.max);
    return AABB(new_min, new_max);
  }

  // Check if a ray intersects this AABB

  bool hit(const ray &r, double &t_min, double &t_max) const
  {
    // std::cout << "bbox intersection CHECK" << std::endl;
    const vec3 orig = r.origin;
    const vec3 dir = r.direction;

    // Iterate over each axis (x, y, z)
    for (int axis = 0; axis < 3; ++axis)
    {
      // Calculate the inverse of the ray's direction component for the current axis
      double inv_d = 1.0 / (axis == 0 ? dir.x : (axis == 1 ? dir.y : dir.z));

      // Calculate the intersection times along the ray's path
      double t0 = ((axis == 0 ? min.x : (axis == 1 ? min.y : min.z)) - (axis == 0 ? orig.x : (axis == 1 ? orig.y : orig.z))) * inv_d;
      double t1 = ((axis == 0 ? max.x : (axis == 1 ? max.y : max.z)) - (axis == 0 ? orig.x : (axis == 1 ? orig.y : orig.z))) * inv_d;
      // If the direction is negative, swap t0 and t1
      if (inv_d < 0.0)
        std::swap(t0, t1);

      // Update the ray's min and max values for this axis
      t_min = std::max(t0, t_min);
      t_max = std::min(t1, t_max);

      // If the ray doesn't intersect the box, return false
      if (t_max <= t_min)
      {
        return false;
      }
    }
    //  If we get here, the ray intersects the AABB
    return true;
  }
};

#endif // AABB_