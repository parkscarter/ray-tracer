#include "scene_setup.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "texture.h"
#include "material.h"
#include "hittable_list.h"
#include "bvh.h"

bvh_node *setup_scene_1(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures)
{
  std::vector<Hittable *> scene;

  // Create and store solid textures
  auto red_tex = std::make_unique<solid_color>(color(0.8, 0.0, 0.0));
  auto gray_tex = std::make_unique<solid_color>(color(0.3, 0.3, 0.3));
  auto blue_tex = std::make_unique<solid_color>(color(0.3, 0.3, 0.9));
  auto orange_tex = std::make_unique<solid_color>(color(1.0, 0.5, 0.0));
  auto green_tex = std::make_unique<solid_color>(color(0.5, 1.0, 0.5));
  auto brown_tex = std::make_unique<solid_color>(color(1.0, 0.6, 0.3));
  auto white_tex = std::make_unique<solid_color>(color(1.0, 1.0, 1.0));
  auto black_tex = std::make_unique<solid_color>(color(0.0, 0.0, 0.0));

  auto perlin_tex = std::make_unique<noise_texture>(1);
  auto light_tex = std::make_unique<solid_color>(color(1.0, 1.0, 0.9));
  auto earth_tex = std::make_unique<image_texture>("earth.jpg");
  auto wall_tex = std::make_unique<image_texture>("wall.jpg");
  auto basketball_tex = std::make_unique<image_texture>("basketball-ball.jpg");

  auto checkered = std::make_unique<checker_texture>(4, red_tex.get(), gray_tex.get());
  auto checkered_2 = std::make_unique<checker_texture>(4, blue_tex.get(), orange_tex.get());

  // Save raw pointers
  auto *earth_ptr = earth_tex.get();
  auto *perlin_ptr = perlin_tex.get();
  auto *ball_ptr = basketball_tex.get();
  auto *wall_ptr = wall_tex.get();
  auto *light_ptr = light_tex.get();
  auto *check_ptr = checkered.get();
  auto *check2_ptr = checkered_2.get();
  auto *white_ptr = white_tex.get();
  auto *red_ptr = red_tex.get();
  auto *blue_ptr = blue_tex.get();
  auto *green_ptr = green_tex.get();
  auto *brown_ptr = brown_tex.get();

  // Store in textures vector
  textures.push_back(std::move(red_tex));
  textures.push_back(std::move(gray_tex));
  textures.push_back(std::move(blue_tex));
  textures.push_back(std::move(orange_tex));
  textures.push_back(std::move(green_tex));
  textures.push_back(std::move(brown_tex));
  textures.push_back(std::move(white_tex));
  textures.push_back(std::move(black_tex));
  textures.push_back(std::move(perlin_tex));
  textures.push_back(std::move(light_tex));
  textures.push_back(std::move(earth_tex));
  textures.push_back(std::move(wall_tex));
  textures.push_back(std::move(basketball_tex));
  textures.push_back(std::move(checkered));
  textures.push_back(std::move(checkered_2));

  // Create and store materials
  auto earth_mat = std::make_unique<lambertian>(*earth_ptr, 1.0);
  auto noise_mat = std::make_unique<lambertian>(*perlin_ptr, 0.85);
  auto basketball_mat = std::make_unique<lambertian>(*ball_ptr, 1.0);
  auto wall_mat = std::make_unique<lambertian>(*wall_ptr, 1.0);
  auto light_mat = std::make_unique<diffuse_light>(light_ptr);
  auto glass_mat = std::make_unique<dielectric>(1.5, 0.8);
  // auto bronze_mat = std::make_unique<metal>(*brown_ptr, 1.0, 0.05);
  auto metal_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.0);
  auto metal_2_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.15);
  auto check_mat = std::make_unique<lambertian>(*check_ptr, 0.85);
  auto check2_mat = std::make_unique<lambertian>(*check2_ptr, 0.85);
  auto white_mat = std::make_unique<lambertian>(*white_ptr, 0.0);
  auto red_mat = std::make_unique<lambertian>(*red_ptr, 0.0);
  auto blue_mat = std::make_unique<lambertian>(*blue_ptr, 0.0);
  auto green_mat = std::make_unique<lambertian>(*green_ptr, 0.0);

  // Save raw pointers
  auto *earth_m = earth_mat.get();
  auto *noise_m = noise_mat.get();
  auto *ball_m = basketball_mat.get();
  auto *wall_m = wall_mat.get();
  auto *light_m = light_mat.get();
  auto *glass_m = glass_mat.get();
  auto *metal_m = metal_mat.get();
  auto *metal2_m = metal_2_mat.get();
  auto *check_m = check_mat.get();
  auto *check2_m = check2_mat.get();
  auto *white_m = white_mat.get();
  auto *red_m = red_mat.get();
  auto *blue_m = blue_mat.get();
  auto *green_m = green_mat.get();

  // Store materials
  materials.push_back(std::move(earth_mat));
  materials.push_back(std::move(noise_mat));
  materials.push_back(std::move(basketball_mat));
  materials.push_back(std::move(wall_mat));
  materials.push_back(std::move(light_mat));
  materials.push_back(std::move(glass_mat));
  materials.push_back(std::move(metal_mat));
  materials.push_back(std::move(metal_2_mat));
  materials.push_back(std::move(check_mat));
  materials.push_back(std::move(check2_mat));
  materials.push_back(std::move(white_mat));
  materials.push_back(std::move(red_mat));
  materials.push_back(std::move(blue_mat));
  materials.push_back(std::move(green_mat));

  // Add hittables
  scene.push_back(new Sphere(vec3(-2.5, 5.0, -12.0), 5, earth_m));
  scene.push_back(new Sphere(vec3(0.0, 0.8, 6.0), 0.8, ball_m));
  scene.push_back(new Sphere(vec3(6.0, 3.0, -8.0), 3, noise_m));
  scene.push_back(new Sphere(vec3(4.5, 1.8, 4.0), 1.8, glass_m));
  scene.push_back(new Sphere(vec3(-9.0, 3.5, -4.0), 3.5, metal_m));
  scene.push_back(new Sphere(vec3(-3.5, 1.2, 5.0), 1.2, metal2_m));
  scene.push_back(new Sphere(vec3(-8.0, 4.5, 2.0), 1.5, light_m));
  scene.push_back(new Sphere(vec3(-8.0, 1.5, 2.0), 1.5, check2_m));
  scene.push_back(new Sphere(vec3(12.0, 3.5, -4.0), 3.5, wall_m));
  scene.push_back(new Sphere(vec3(0.0, -1000.0, 0.0), 1000, check_m));
  scene.push_back(new Sphere(vec3(10.0, 6.0, 20.0), 1.5, light_m));

  scene.push_back(new Quad(vec3(-8.0, 12.0, 8.0), vec3(16, 0, 0), vec3(0, 0, 16), light_m));
  scene.push_back(new Quad(vec3(-8.0, 12.0, -12.0), vec3(16, 0, 0), vec3(0, 0, 16), light_m));
  scene.push_back(new Quad(vec3(-8.0, 12.0, -32.0), vec3(16, 0, 0), vec3(0, 0, 16), light_m));

  const std::string obj_file = "./obj/bimba.obj"; // <== Hardcoded path

  try
  {
    color bronze_color(1.0, 0.6, 0.3);
    auto bronze_tex = std::make_unique<solid_color>(bronze_color);
    auto bronze_mat = std::make_unique<metal>(bronze_color, 1.0, 0.05);
    metal *bronze_mat_ptr = bronze_mat.get();

    // Store material and texture
    textures.push_back(std::move(bronze_tex));
    materials.push_back(std::move(bronze_mat));

    // Optional: Print bounding box of OBJ
    Util::print_obj_bounding_box(obj_file);

    HittableList obj = HittableList::load_triangles_from_obj(obj_file, bronze_mat_ptr);
    scene.push_back(new HittableList(std::move(obj)));
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error loading OBJ file: " << e.what() << std::endl;
  }

  for (auto *obj : scene)
    obj->bounding_box = obj->getBoundingBox();

  bvh_node *root = new bvh_node(scene.data(), 0, scene.size());

  for (auto &obj : scene)
    obj = nullptr;

  return root;
}

bvh_node *setup_scene_2(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures)
{
  std::vector<Hittable *> scene;

  for (auto *obj : scene)
    obj->bounding_box = obj->getBoundingBox();

  bvh_node *root = new bvh_node(scene.data(), 0, scene.size());

  for (auto &obj : scene)
    obj = nullptr;

  return root;
}
