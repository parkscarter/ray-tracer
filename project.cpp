#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <tuple>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "rtw_stb_image.h"

#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "util.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"
#include "hittable.h"
#include "camera.h"
#include "hittable_list.h"
#include "aabb.h"
#include "bvh.h"
#include "material.h"
#include "scene_setup.h"

// command to compile:
//  g++ -std=c++14 project.cpp camera.cpp scene_setup.cpp rtw_stb_image.cpp -o ray-tracer

#define IW 480 // image width 240, 480, 960, 1920, 3840
int main(int argc, char *argv[])
{
  // Define Camera
  vec3 camera_position(0.0, 4.0, 12.0);
  vec3 look_at(0.0, 0.0, -1.0); // Looking towards the negative Z axis
  vec3 up(0.0, -1.0, 0.0);      // this should be negative 1
  double fov = 65.0;
  int samples = 200;

  // Set image width and height
  int image_width = IW;
  double aspectRatio = 16.0 / 9.0;

  std::vector<Hittable *> scene;
  std::vector<std::unique_ptr<material>> materials;
  std::vector<std::unique_ptr<texture>> textures;

  bvh_node *root = setup_scene(materials, textures);

  Camera c(image_width, aspectRatio, camera_position, look_at, up, fov, samples, root);
  c.render();

  return 0;
}