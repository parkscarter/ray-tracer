#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "color.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class texture
{
public:
  virtual ~texture() = default;

  virtual color value(double u, double v, const vec3 &p) const = 0;
};

class solid_color : public texture
{
public:
  solid_color(const color &albedo) : albedo(albedo) {}

  solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

  color value(double u, double v, const vec3 &p) const override
  {
    return albedo;
  }

private:
  color albedo;
};

class noise_texture : public texture
{
public:
  noise_texture(double scale) : scale(scale) {}

  color value(double u, double v, const vec3 &p) const override
  {
    color result;
    result.value = vec3(1, 1, 1) * noise.turbulance(p, 7);
    // result.value = vec3(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z + 10 * noise.turb(p, 7)));
    return result;
  }

private:
  perlin noise;
  double scale;
};

class checker_texture : public texture
{
public:
  checker_texture(double scale, texture *even, texture *odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

  checker_texture(double scale, const color &c1, const color &c2)
      : checker_texture(scale, new solid_color(c1), new solid_color(c2)) {}

  color value(double u, double v, const vec3 &p) const override
  {
    int xInteger = int(std::floor(inv_scale * p.x));
    int yInteger = int(std::floor(inv_scale * p.y));
    int zInteger = int(std::floor(inv_scale * p.z));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
  }

private:
  double inv_scale;
  texture *even;
  texture *odd;
};

class image_texture : public texture
{
public:
  image_texture(const char *filename) : image(filename) {}

  color value(double u, double v, const vec3 &p) const override
  {
    // std::cout << image.height() << std::endl;
    //  If we have no texture data, then return solid cyan as a debugging aid.
    if (image.height() <= 0)
      return color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = Util::clamp(0, 1, u);
    v = 1.0 - Util::clamp(0, 1, v); // Flip V to image coordinates

    int i = int(u * image.width());
    int j = int(v * image.height());

    auto pixel = image.pixel_data(i, j);

    auto color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

private:
  rtw_image image;
};

#endif