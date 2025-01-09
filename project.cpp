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
//  g++ -std=c++11 project.cpp camera.cpp rtw_stb_image.cpp -o project
//  ***
//  As I was doing my final render I found that because there were objects surrounding the camera,
//  each ray would hit the bvh. The renders began taking longer than usual, then I remembered I
//  added each triangle from the hittable_list to the bvh separately, meaning that each ray had to
//  be calculated log(n) times where n was the number of triangles. This didn't show up in earlier
//  testing because some rays would miss the bvh. I tried for a quick solution but no luck

#define IW 480 // image width 240, 480, 960, 1920, 3840
int main(int argc, char *argv[])
{
  std::vector<Hittable *> hittables;

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

  hittables.push_back(new Sphere(vec3(-2.5, 5.0, -12.0), 5, earth_mat));
  hittables.push_back(new Sphere(vec3(0.0, 0.8, 6.0), 0.8, basketball_mat));
  hittables.push_back(new Sphere(vec3(6.0, 3.0, -8.0), 3, noise_mat));
  hittables.push_back(new Sphere(vec3(4.5, 1.8, 4.0), 1.8, glass_mat));
  hittables.push_back(new Sphere(vec3(-9.0, 3.5, -4.0), 3.5, metal_mat));
  hittables.push_back(new Sphere(vec3(-3.5, 1.2, 5.0), 1.2, metal_2_mat));
  hittables.push_back(new Sphere(vec3(-8.0, 4.5, 2.0), 1.5, light_mat));
  hittables.push_back(new Sphere(vec3(-8.0, 1.5, 2.0), 1.5, checkered_2_mat));
  hittables.push_back(new Sphere(vec3(12.0, 3.5, -4.0), 3.5, wall_mat));

  hittables.push_back(new Sphere(vec3(0.0, -1000.0, 0.0), 1000, checkered_mat));

  hittables.push_back(new Sphere(vec3(10.0, 6, 20.0), 1.5, light_mat));

  // vec3 centerA = vec3(3.0, 1.5, 0.0);
  // vec3 centerB = vec3(3.0, 2.5, 0.0);
  // hittables.push_back(new Sphere(centerA, centerB, 2, green_mat));

  hittables.push_back(new Quad(vec3(-8.0, 12.0, 8.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  hittables.push_back(new Quad(vec3(-8.0, 12.0, -12.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  // hittables.push_back(new Quad(vec3(-28.0, 12.0, -12.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  // hittables.push_back(new Quad(vec3(12.0, 12.0, -12.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
  hittables.push_back(new Quad(vec3(-8.0, 12.0, -32.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));

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

    for (Hittable *h : obj.objects)
    {
      hittables.push_back(h);
    }
  }
  else if (argc > 2)
  {
    std::cerr << "Plese provide only one obj file for now!" << std::endl;
    return 1;
  }

  for (const auto &hittable : hittables)
  {
    hittable->bounding_box = hittable->getBoundingBox();
  }

  // Build a BVH tree from the hittables vector
  bvh_node *root = new bvh_node(hittables.data(), 0, hittables.size());

  // Cleanup the raw pointers in hittables (BVH will manage them)
  for (auto &hittable : hittables)
  {
    // Don't delete objects in the vector, because they're managed by the BVH now
    hittable = nullptr;
  }

  vec3 camera_position(0.0, 4.0, 12.0);
  vec3 look_at(0.0, 0.0, -1.0); // Looking towards the negative Z axis
  vec3 up(0.0, -1.0, 0.0);      // this should be negative 1
  double fov = 65.0;
  int samples = 120;

  // Set image width and height
  int image_width = IW;
  double aspectRatio = 16.0 / 9.0;
  Camera c(image_width, aspectRatio, camera_position, look_at, up, fov, samples, root);
  c.render();

  return 0;
}