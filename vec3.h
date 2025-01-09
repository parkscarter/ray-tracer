#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "util.h"

class vec3
{
public:
  double x, y, z;

  // Default constructor
  vec3() : x(0.0), y(0.0), z(0.0) {}

  // Parameterized constructor
  vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  // Initialize the vector
  void init(double x, double y, double z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  // Vector addition: c = a + b
  static vec3 add(const vec3 &a, const vec3 &b)
  {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  // Normalize the vector (in-place)
  void normalize()
  {
    double len = length();
    if (len > 0)
    {
      x /= len;
      y /= len;
      z /= len;
    }
  }

  // Cross product: c = a x b
  static vec3 cross(const vec3 &a, const vec3 &b)
  {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
  }

  // Dot product: a . b
  static double dot(const vec3 &a, const vec3 &b)
  {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  // Subtraction: c = a - b
  static vec3 sub(const vec3 &a, const vec3 &b)
  {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
  }

  // Scale the vector by a scalar: c = v * d
  static vec3 scale(const vec3 &v, double d)
  {
    return vec3(v.x * d, v.y * d, v.z * d);
  }

  // Length squared of the vector
  double length_squared() const
  {
    return x * x + y * y + z * z;
  }

  // Length of the vector
  double length() const
  {
    return std::sqrt(length_squared());
  }

  static vec3 random()
  {
    return vec3(Util::random_double(), Util::random_double(), Util::random_double());
  }

  static vec3 random(double min, double max)
  {
    return vec3(Util::random_double_range(min, max), Util::random_double_range(min, max), Util::random_double_range(min, max));
  }

  static vec3 random_unit_vector()
  {
    while (true)
    {
      vec3 p = vec3::random(-1, 1);
      double lensq = p.length_squared();
      if (1e-160 < lensq && lensq <= 1)
        return p * (1 / sqrt(lensq));
    }
  }

  static vec3 random_on_hemisphere(const vec3 &normal)
  {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
      return on_unit_sphere;
    else
      return on_unit_sphere * -1.0;
  }

  // Unit vector (returns a new vec3)
  static vec3 unit_vector(const vec3 &v)
  {
    double len = v.length();
    if (len > 0)
    {
      return scale(v, 1.0 / len);
    }
    return vec3(0.0, 0.0, 0.0); // Return zero vector if length is 0
  }

  bool near_zero() const
  {
    // Return true if the vector is close to zero in all dimensions.
    auto s = 1e-8;
    return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
  }

  // Print vector to console
  void print() const
  {
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
  }

  static vec3 min(const vec3 &v1, const vec3 &v2)
  {
    return vec3(
        (v1.x < v2.x) ? v1.x : v2.x,
        (v1.y < v2.y) ? v1.y : v2.y,
        (v1.z < v2.z) ? v1.z : v2.z);
  }

  static vec3 max(const vec3 &v1, const vec3 &v2)
  {
    return vec3(
        (v1.x > v2.x) ? v1.x : v2.x,
        (v1.y > v2.y) ? v1.y : v2.y,
        (v1.z > v2.z) ? v1.z : v2.z);
  }

  double &operator[](int i)
  {
    if (i == 0)
      return x;
    else if (i == 1)
      return y;
    else
      return z;
  }

  // Overload the + operator for vector addition
  vec3 operator+(const vec3 &other) const
  {
    return vec3(x + other.x, y + other.y, z + other.z);
  }

  // Overload the - operator for vector subtraction
  vec3 operator-(const vec3 &other) const
  {
    return vec3(x - other.x, y - other.y, z - other.z);
  }

  // Overload the * operator for scalar multiplication
  vec3 operator*(double scalar) const
  {
    return vec3(x * scalar, y * scalar, z * scalar);
  }

  // Overload the dot operator
  // double operator*(const vec3 &other) const
  // {
  //   return dot(*this, other);
  // }

  vec3 operator*(const vec3 &a) const
  {
    return vec3(this->x * a.x, this->y * a.y, this->z * a.z);
  }

  // Overload the cross operator
  vec3 operator^(const vec3 &other) const
  {
    return cross(*this, other);
  }

  // Overload the equality operator
  bool operator==(const vec3 &other) const
  {
    return (x == other.x) && (y == other.y) && (z == other.z);
  }

  // Overload the stream output operator for easy printing
  friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
  {
    os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z;
    return os;
  }
};

typedef vec3 Point;

#endif // VEC3_H