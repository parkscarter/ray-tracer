#ifndef RAY_H
#define RAY_H

#include "vec3.h" // Assuming the vec3 class is already defined

class ray
{
public:
  vec3 origin;    // The starting point of the ray
  vec3 direction; // The direction vector of the ray
  double tm;

  // Default constructor
  ray() : origin(vec3(0.0, 0.0, 0.0)), direction(vec3(0.0, 0.0, 0.0)), tm(0) {}

  // Parameterized constructor
  ray(const vec3 &origin, const vec3 &direction, double time)
      : origin(origin), direction(direction), tm(time) {}

  ray(const vec3 &origin, const vec3 &direction)
      : origin(origin), direction(direction), tm(0) {}

  double time() const { return tm; }

  // Function to return the position at a given time t
  vec3 at(double t) const
  {
    return origin + direction * t;
  }

  // Static function to reflect vector v against the normal vector n
  static vec3 reflect(const vec3 &v, const vec3 &n)
  {
    double temp = 2 * vec3::dot(v, n);
    vec3 result = vec3::scale(n, temp);
    result = vec3::sub(v, result);
    return result;
  }

  static vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
  {
    // double cos_theta = std::fmin((uv * -1.0) * n, 1.0);
    double cos_theta = std::fmin(vec3::dot((uv * -1.0), n), 1.0);
    vec3 r_out_perp = (uv + (n * cos_theta)) * etai_over_etat;
    vec3 r_out_parallel = n * -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()));
    return r_out_perp + r_out_parallel;
  }
};

#endif // RAY_H

// #ifndef RAY_H
// #define RAY_H

// #include "vec3.h" // Assuming the vec3 class is already defined

// class ray
// {
// public:
//   vec3 origin;    // The starting point of the ray
//   vec3 direction; // The direction vector of the ray

//   // Default constructor
//   ray() : origin(vec3(0.0, 0.0, 0.0)), direction(vec3(0.0, 0.0, 0.0)) {}

//   // Parameterized constructor
//   ray(const vec3 &origin, const vec3 &direction)
//       : origin(origin), direction(direction) {}

//   // Function to return the position at a given time t
//   vec3 at(double t) const
//   {
//     return origin + direction * t;
//   }

//   // Static function to reflect vector v against the normal vector n
//   static vec3 reflect(const vec3 &v, const vec3 &n)
//   {
//     double temp = 2 * vec3::dot(v, n);
//     vec3 result = vec3::scale(n, temp);
//     result = vec3::sub(v, result);
//     return result;
//   }

//   static vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
//   {
//     // double cos_theta = std::fmin((uv * -1.0) * n, 1.0);
//     double cos_theta = std::fmin(vec3::dot((uv * -1.0), n), 1.0);
//     vec3 r_out_perp = (uv + (n * cos_theta)) * etai_over_etat;
//     vec3 r_out_parallel = n * -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared()));
//     return r_out_perp + r_out_parallel;
//   }
// };

// #endif // RAY_H