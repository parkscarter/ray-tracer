#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "triangle.h"
#include "bvh.h"
// #include "rtw_stb_image.h"
#include "material.h"
#include <vector>
#include <stdio.h>
#include <limits>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

#define MAX_BOUNCES 20

class Camera
{
public:
  Camera(int width, double a_ratio, vec3 center, vec3 look_at, vec3 up, double fov, int samples, int background_color, bvh_node *scene_root);

  color ray_color(const ray &r, int depth = MAX_BOUNCES) const;
  ray get_ray(int i, int j) const;
  void render() const;

private:
  double aspect_ratio;
  int image_width;
  int image_height;

  int background_mode;

  Point center; // Camera center
  vec3 forward; // Direction the camera is facing (viewing direction)
  vec3 right;
  vec3 up;
  double fov; // Field of view in degrees

  std::vector<Hittable *> hittables;
  // int num_hittables; // Number of objects in the scene

  Point pixel_00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  int samples_per_pixel; // Number of samples taken for antialiasing

  bvh_node *scene_root; // BVH object for entire scene

  // Private helper methods
  vec3 sample_square() const;
};

#endif // CAMERA_H