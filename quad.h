#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class Quad : public Hittable
{
public:
  Quad(const vec3 &Q, const vec3 &u, const vec3 &v, material *mat)
      : Q(Q), u(u), v(v), mat(mat)
  {
    vec3 n = u ^ v;
    normal = vec3::unit_vector(n);
    D = vec3::dot(normal, Q);
    w = n * (1 / vec3::dot(n, n));

    set_bounding_box();
  }

  virtual void set_bounding_box()
  {
    // Compute the bounding box of all four vertices.
    AABB bbox_diagonal1 = AABB(Q, Q + u + v);
    AABB bbox_diagonal2 = AABB(Q + u, Q + v);

    bbox = AABB::combine(bbox_diagonal1, bbox_diagonal2);

    vec3 boxSize = bbox.max - bbox.min;
    const float MIN_SIZE = 0.01f;

    // Ensure that no dimension of the AABB has a size less than MIN_SIZE
    if (boxSize.x < MIN_SIZE)
    {
      bbox.max.x = bbox.min.x + MIN_SIZE;
    }
    if (boxSize.y < MIN_SIZE)
    {
      bbox.max.y = bbox.min.y + MIN_SIZE;
    }
    if (boxSize.z < MIN_SIZE)
    {
      bbox.max.z = bbox.min.z + MIN_SIZE;
    }
  }

  AABB getBoundingBox() const override { return bbox; }

  bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
  {
    auto denom = vec3::dot(normal, r.direction);
    ;
    // No hit if the ray is parallel to the plane.
    if (std::fabs(denom) < 1e-8)
      return false;
    ;
    // Return false if the hit point parameter t is outside the ray interval.
    double t = (D - vec3::dot(normal, r.origin)) / denom;
    if (t < t_min || t > t_max)
      return false;
    ;
    vec3 intersection = r.at(t);

    vec3 planar_hitpt_vector = intersection - Q;
    auto alpha = vec3::dot(w, vec3::cross(planar_hitpt_vector, v));
    auto beta = vec3::dot(w, vec3::cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta, rec))
      return false;
    rec.t = t;
    rec.p = intersection;
    rec.mat = mat;
    rec.set_face_normal(r, normal);

    return true;
  }

  virtual bool is_interior(double a, double b, hit_record &rec) const
  {
    // Given the hit point in plane coordinates, return false if it is outside the
    // primitive, otherwise set the hit record UV coordinates and return true.

    if (a < 0 || a > 1 || b < 0 || b > 1)
      return false;

    rec.u = a;
    rec.v = b;
    return true;
  }

private:
  vec3 Q;
  vec3 u, v;
  vec3 w;
  material *mat;
  AABB bbox;
  vec3 normal;
  double D;
};

#endif