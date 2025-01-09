#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "color.h"
#include "texture.h"

class material
{
public:
  virtual ~material() = default;

  double reflectivity;

  virtual color emitted(double u, double v, const vec3 &p) const
  {
    return color(0, 0, 0);
  }

  virtual bool scatter(
      const ray &r_in, hit_record &rec, color &attenuation, ray &scattered) const
  {
    return false;
  }
};

class lambertian : public material
{
public:
  lambertian(texture &tex, double reflectivity) : tex(&tex), reflectivity(reflectivity) {}

  double reflectivity;

  bool scatter(const ray &r_in, hit_record &rec, color &attenuation, ray &scattered)
      const override
  {
    vec3 scatter_direction = rec.normal + vec3::random_unit_vector();

    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;
    scattered = ray(rec.p, scatter_direction, r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);
    return true;
  }

private:
  texture *tex;
};

class metal : public material
{
public:
  metal(const color &albedo, double reflectivity, double fuzz) : albedo(albedo), reflectivity(reflectivity), fuzz(fuzz) {}

  double reflectivity;
  double fuzz;

  bool scatter(const ray &r_in, hit_record &rec, color &attenuation, ray &scattered)
      const override
  {
    vec3 reflected = ray::reflect(r_in.direction, rec.normal);
    reflected = vec3::unit_vector(reflected) + (vec3::random_unit_vector() * fuzz);
    scattered = ray(rec.p, reflected, r_in.time());
    attenuation = albedo;
    rec.mat->reflectivity = reflectivity;

    return true;
  }

private:
  color albedo;
};

class diffuse_light : public material
{
public:
  diffuse_light(texture *tex) : tex(tex) {}
  // diffuse_light(const color &emit) : tex (solid_color(emit)) {}

  color emitted(double u, double v, const vec3 &p) const override
  {
    return tex->value(u, v, p);
  }

private:
  texture *tex;
};

class dielectric : public material
{
public:
  dielectric(double refraction_index, double reflectivity) : refraction_index(refraction_index), reflectivity(reflectivity) {}

  double reflectivity;

  bool scatter(const ray &r_in, hit_record &rec, color &attenuation, ray &scattered)
      const override
  {
    attenuation = color(1.0, 1.0, 1.0);

    double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;
    vec3 unit_direction = vec3::unit_vector(r_in.direction);

    vec3 refracted = ray::refract(unit_direction, rec.normal, ri);

    scattered = ray(rec.p, refracted, r_in.time());
    return true;
  }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive index over
  // the refractive index of the enclosing media
  double refraction_index;
};

#endif