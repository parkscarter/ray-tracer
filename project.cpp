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

// command to compile:
//  g++ -std=c++11 project.cpp camera.cpp rtw_stb_image.cpp -o ray-tracer

#define IW 3840 // image width 240, 480, 960, 1920, 3840
int main(int argc, char *argv[])
{
  // Define Camera
  vec3 camera_position(0.0, 4.0, 12.0);
  vec3 look_at(0.0, 0.0, -1.0); // Looking towards the negative Z axis
  vec3 up(0.0, -1.0, 0.0);      // this should be negative 1
  double fov = 65.0;
  int samples = 120;

  // Set image width and height
  int image_width = IW;
  double aspectRatio = 16.0 / 9.0;
  std::vector<Hittable *> scene;

  // define all commonly used colors
  color red(0.8, 0.0, 0.0);
  color blue(0.3, 0.3, 0.9);
  color green(0.5, 1.0, 0.5);
  color grey(0.7, 0.7, 0.7);
  color gray(0.3, 0.3, 0.3);
  color brown(1.0, 0.6, 0.3);
  color orange(1.0, 0.5, 0.0);
  color white(1.0, 1.0, 1.0);
  color black(0.0, 0.0, 0.0);

  // and textures...
  solid_color green_tex(green);
  solid_color brown_tex(brown);
  solid_color blue_tex(blue);
  solid_color orange_tex(orange);
  solid_color white_tex(white);
  solid_color red_tex(red);
  solid_color black_tex(black);
  solid_color gray_tex(gray);

  noise_texture perlin_texture(1);
  solid_color light_texture = solid_color(color(1.0, 1.0, 0.9));
  image_texture earth("earth.jpg");
  image_texture wall("wall.jpg");
  image_texture basketball("basketball-ball.jpg");

  checker_texture checkered(4, &red_tex, &gray_tex);
  checker_texture checkered_2(4, &blue_tex, &orange_tex);

  // materials
  dielectric *glass_mat = new dielectric(1.5, 0.8);
  lambertian *noise_mat = new lambertian(perlin_texture, 0.85);
  metal *bronze_mat = new metal(brown, 1.0, 0.05);
  metal *metal_mat = new metal(grey, 1.0, 0.0);
  metal *metal_2_mat = new metal(grey, 1.0, 0.15);
  lambertian *checkered_mat = new lambertian(checkered, 0.85);
  lambertian *checkered_2_mat = new lambertian(checkered_2, 0.85);
  diffuse_light *light_mat = new diffuse_light(&light_texture);
  lambertian *earth_mat = new lambertian(earth, 1.0);
  lambertian *wall_mat = new lambertian(wall, 1.0);
  lambertian *basketball_mat = new lambertian(basketball, 1.0);

  lambertian *white_mat = new lambertian(white_tex, 0.0);
  lambertian *red_mat = new lambertian(red_tex, 0.0);
  lambertian *blue_mat = new lambertian(blue_tex, 0.0);
  lambertian *green_mat = new lambertian(green_tex, 0.0);

  // Set scene by adding objects
  // Spheres are defined by (x, y, z), radius, material
  scene.push_back(new Sphere(vec3(-2.5, 5.0, -12.0), 5, earth_mat));
  scene.push_back(new Sphere(vec3(0.0, 0.8, 6.0), 0.8, basketball_mat));
  scene.push_back(new Sphere(vec3(6.0, 3.0, -8.0), 3, noise_mat));
  scene.push_back(new Sphere(vec3(4.5, 1.8, 4.0), 1.8, glass_mat));
  scene.push_back(new Sphere(vec3(-9.0, 3.5, -4.0), 3.5, metal_mat));
  scene.push_back(new Sphere(vec3(-3.5, 1.2, 5.0), 1.2, metal_2_mat));
  scene.push_back(new Sphere(vec3(-8.0, 4.5, 2.0), 1.5, light_mat));
  scene.push_back(new Sphere(vec3(-8.0, 1.5, 2.0), 1.5, checkered_2_mat));
  scene.push_back(new Sphere(vec3(12.0, 3.5, -4.0), 3.5, wall_mat));
  scene.push_back(new Sphere(vec3(0.0, -1000.0, 0.0), 1000, checkered_mat));
  scene.push_back(new Sphere(vec3(10.0, 6, 20.0), 1.5, light_mat));

  // Quads for ceiling lights
  scene.push_back(new Quad(vec3(-8.0, 12.0, 8.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  scene.push_back(new Quad(vec3(-8.0, 12.0, -12.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  scene.push_back(new Quad(vec3(-8.0, 12.0, -32.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));

  //     Load OBJ file (if provided) and create HittableList

  if (argc == 2)
  {
    double objectReflectivity = 0;
    HittableList obj;
    std::ostringstream oss;
    oss << "./obj/" << argv[1];

    // Convert ostringstream to string
    std::string filepath = oss.str();

    Util::print_obj_bounding_box(filepath);

    try
    {
      obj = HittableList::load_triangles_from_obj(filepath, bronze_mat);
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error loading OBJ file: " << e.what() << std::endl;
      return 1;
    }

    scene.push_back(new HittableList(std::move(obj)));
  }
  else if (argc > 2)
  {
    std::cerr << "Plese provide only one obj file for now!" << std::endl;
    return 1;
  }

  for (const auto &hittable : scene)
  {
    hittable->bounding_box = hittable->getBoundingBox();
  }

  // Build a BVH tree from the scene vector
  bvh_node *root = new bvh_node(scene.data(), 0, scene.size());

  // Cleanup the raw pointers in scene (BVH will manage them)
  for (auto &hittable : scene)
  {
    // Don't delete objects in the vector, because they're managed by the BVH now
    hittable = nullptr;
  }

  Camera c(image_width, aspectRatio, camera_position, look_at, up, fov, samples, root);
  c.render();

  return 0;
}