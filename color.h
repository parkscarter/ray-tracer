#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "util.h"
#include <iostream>
#include <fstream>

class color
{
public:
  vec3 value; // encapsulating color in a vec3

  // Default constructor: sets the color to black
  color() : value(0.0, 0.0, 0.0) {}

  // Parameterized constructor
  color(double x, double y, double z) : value(x, y, z) {}

  color gamma_corrected(double gamma = 2.2) const
  {
    return color(
        std::pow(value.x, 1.0 / gamma),
        std::pow(value.y, 1.0 / gamma),
        std::pow(value.z, 1.0 / gamma));
  }

  static color hsv_to_rgb(double h, double s, double v)
  {
    int i = static_cast<int>(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch (i % 6)
    {
    case 0:
      return color(v, t, p);
    case 1:
      return color(q, v, p);
    case 2:
      return color(p, v, t);
    case 3:
      return color(p, q, v);
    case 4:
      return color(t, p, v);
    case 5:
      return color(v, p, q);
    default:
      return color(1, 0, 1); // fallback magenta
    }
  }

  void write_to_file(FILE *out) const
  {
    double min_intensity = 0.0;
    double max_intensity = 0.999;

    int rbyte = static_cast<int>(256 * Util::clamp(min_intensity, max_intensity, value.x));
    int gbyte = static_cast<int>(256 * Util::clamp(min_intensity, max_intensity, value.y));
    int bbyte = static_cast<int>(256 * Util::clamp(min_intensity, max_intensity, value.z));
    fprintf(out, "%d %d %d\n", rbyte, gbyte, bbyte);
  };
};

#endif // COLOR_H