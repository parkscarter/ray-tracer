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

#define IW 960 // image width 240, 480, 960, 1920, 3840
int main(int argc, char *argv[])
{
  // Define camera quality: Higher => Nicer image but longer run-time
  int samples = 100;
  int image_width = IW;

  CameraConfig cam_config;

  std::vector<Hittable *> scene;
  std::vector<std::unique_ptr<material>> materials;
  std::vector<std::unique_ptr<texture>> textures;

  int scene_number = 1; // Default to 1
  if (argc >= 2)
  {
    try
    {
      scene_number = std::stoi(argv[1]);
    }
    catch (...)
    {
      std::cerr << "Invalid scene number. Using default scene 1." << std::endl;
    }
  }

  bvh_node *root = nullptr;

  switch (scene_number)
  {
  case 1:
    root = setup_scene_1(materials, textures, cam_config);
    break;
  case 2:
    root = setup_scene_2(materials, textures, cam_config);
    break;
  case 3:
    root = setup_scene_3(materials, textures, cam_config);
    break;
  default:
    std::cerr << "Unknown scene number: " << scene_number << ". Using default scene 1." << std::endl;
    root = setup_scene_1(materials, textures, cam_config);
    break;
  }

  Camera c(image_width, cam_config.aspect_ratio, cam_config.position, cam_config.look_at,
           cam_config.up, cam_config.fov, samples, cam_config.background_color, root);
  c.render();

  return 0;
}