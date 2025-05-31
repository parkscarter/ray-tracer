#include "scene_setup.h"
#include "color.h"

bvh_node *setup_scene_1(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config)
{

  // Set camera position and orientation
  cam_config.position = vec3(0.0, 4.0, 12.0);
  cam_config.look_at = vec3(0.0, 0.0, -1.0);
  cam_config.up = vec3(0.0, -1.0, 0.0);
  cam_config.fov = 65.0;
  cam_config.aspect_ratio = 16.0 / 9.0;
  cam_config.background_color = 0;

  std::vector<Hittable *> scene;

  // Create and store solid textures
  auto red_tex = std::make_unique<solid_color>(color(0.8, 0.0, 0.0));
  auto gray_tex = std::make_unique<solid_color>(color(0.3, 0.3, 0.3));
  auto blue_tex = std::make_unique<solid_color>(color(0.3, 0.3, 0.9));
  auto orange_tex = std::make_unique<solid_color>(color(1.0, 0.5, 0.0));
  // auto green_tex = std::make_unique<solid_color>(color(0.5, 1.0, 0.5));
  // auto brown_tex = std::make_unique<solid_color>(color(1.0, 0.6, 0.3));
  // auto white_tex = std::make_unique<solid_color>(color(1.0, 1.0, 1.0));
  // auto black_tex = std::make_unique<solid_color>(color(0.0, 0.0, 0.0));

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
  // auto *white_ptr = white_tex.get();
  auto *red_ptr = red_tex.get();
  auto *blue_ptr = blue_tex.get();
  // auto *green_ptr = green_tex.get();
  // auto *brown_ptr = brown_tex.get();

  // Store in textures vector
  textures.push_back(std::move(red_tex));
  textures.push_back(std::move(gray_tex));
  textures.push_back(std::move(blue_tex));
  textures.push_back(std::move(orange_tex));
  // textures.push_back(std::move(green_tex));
  // textures.push_back(std::move(brown_tex));
  // textures.push_back(std::move(white_tex));
  // textures.push_back(std::move(black_tex));
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
  auto metal_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.0);
  auto metal_2_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.15);
  auto check_mat = std::make_unique<lambertian>(*check_ptr, 0.85);
  auto check2_mat = std::make_unique<lambertian>(*check2_ptr, 0.85);
  // auto white_mat = std::make_unique<lambertian>(*white_ptr, 0.0);
  auto red_mat = std::make_unique<lambertian>(*red_ptr, 0.0);
  auto blue_mat = std::make_unique<lambertian>(*blue_ptr, 0.0);
  // auto green_mat = std::make_unique<lambertian>(*green_ptr, 0.0);

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
  // auto *white_m = white_mat.get();
  auto *red_m = red_mat.get();
  auto *blue_m = blue_mat.get();
  // auto *green_m = green_mat.get();

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
  // materials.push_back(std::move(white_mat));
  materials.push_back(std::move(red_mat));
  materials.push_back(std::move(blue_mat));
  // materials.push_back(std::move(green_mat));

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

  const std::string obj_file = "./obj/teapot.obj";

  try
  {
    color bronze_color(1.0, 0.6, 0.3);
    auto bronze_tex = std::make_unique<solid_color>(bronze_color);
    auto bronze_mat = std::make_unique<metal>(bronze_color, 1.0, 0.05);
    metal *bronze_mat_ptr = bronze_mat.get();

    textures.push_back(std::move(bronze_tex));
    materials.push_back(std::move(bronze_mat));

    // Print bounding box of OBJ
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
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config)
{

  cam_config.position = vec3(0.0, 8.0, 25.0);
  cam_config.look_at = vec3(0.0, 0.0, -1.0);
  cam_config.up = vec3(0.0, -1.0, 0.0);
  cam_config.fov = 65.0;
  cam_config.aspect_ratio = 3.0 / 2.0;
  cam_config.background_color = 2;

  std::vector<Hittable *> scene;

  auto purple_tex = std::make_unique<solid_color>(color(0.2, 0.1, 0.3));
  auto green_tex = std::make_unique<solid_color>(color(0.05, 0.75, 0.05));
  auto red_tex = std::make_unique<solid_color>(color(0.75, 0.05, 0.05));
  auto black_tex = std::make_unique<solid_color>(color(0.0, 0.0, 0.0));
  auto blue_tex = std::make_unique<solid_color>(color(0.2, 0.4, 0.9));
  auto gray_tex = std::make_unique<solid_color>(color(0.2, 0.2, 0.2));
  auto light_tex = std::make_unique<solid_color>(color(1.0, 1.0, 0.9));
  auto bricks_tex = std::make_unique<image_texture>("bricks.jpg");

  auto checkered = std::make_unique<checker_texture>(4, purple_tex.get(), gray_tex.get());

  auto *purple_ptr = purple_tex.get();
  auto *green_ptr = green_tex.get();
  auto *red_ptr = red_tex.get();
  auto *black_ptr = black_tex.get();
  auto *blue_ptr = blue_tex.get();
  auto *gray_ptr = gray_tex.get();
  auto *light_ptr = light_tex.get();
  auto *bricks_ptr = bricks_tex.get();
  auto *check_ptr = checkered.get();

  textures.push_back(std::move(purple_tex));
  textures.push_back(std::move(green_tex));
  textures.push_back(std::move(red_tex));
  textures.push_back(std::move(black_tex));
  textures.push_back(std::move(blue_tex));
  textures.push_back(std::move(gray_tex));
  textures.push_back(std::move(light_tex));
  textures.push_back(std::move(bricks_tex));
  textures.push_back(std::move(checkered));

  auto green_mat = std::make_unique<lambertian>(*green_ptr, 0.0);
  auto red_mat = std::make_unique<lambertian>(*red_ptr, 0.0);
  auto black_mat = std::make_unique<lambertian>(*black_ptr, 0.0);
  auto blue_mat = std::make_unique<lambertian>(*blue_ptr, 0.0);
  auto gray_mat = std::make_unique<lambertian>(*gray_ptr, 0.0);
  auto light_mat = std::make_unique<diffuse_light>(light_ptr);
  auto glass_mat = std::make_unique<dielectric>(1.5, 0.8);
  auto metal_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.0);
  auto metal_mat2 = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.3);
  auto bricks_mat = std::make_unique<lambertian>(*bricks_ptr, 0.0);
  auto check_mat = std::make_unique<lambertian>(*check_ptr, 0.0);

  auto *green_m = green_mat.get();
  auto *red_m = red_mat.get();
  auto *black_m = black_mat.get();
  auto *blue_m = blue_mat.get();
  auto *gray_m = gray_mat.get();
  auto *light_m = light_mat.get();
  auto *glass_m = glass_mat.get();
  auto *metal_m = metal_mat.get();
  auto *metal_m2 = metal_mat2.get();
  auto *bricks_m = bricks_mat.get();
  auto *check_m = check_mat.get();

  materials.push_back(std::move(green_mat));
  materials.push_back(std::move(red_mat));
  materials.push_back(std::move(black_mat));
  materials.push_back(std::move(blue_mat));
  materials.push_back(std::move(gray_mat));
  materials.push_back(std::move(light_mat));
  materials.push_back(std::move(glass_mat));
  materials.push_back(std::move(metal_mat));
  materials.push_back(std::move(metal_mat2));
  materials.push_back(std::move(bricks_mat));
  materials.push_back(std::move(check_mat));

  scene.push_back(new Quad(vec3(-15.0, -3.7, -15.0), vec3(0, 25, 0), vec3(0, 0, 35), green_m));  // Left Wall
  scene.push_back(new Quad(vec3(15.0, -3.7, -15.0), vec3(0, 25, 0), vec3(0, 0, 35), red_m));     // Right Wall
  scene.push_back(new Quad(vec3(-15.0, 21.3, -15.0), vec3(30, 0, 0), vec3(0, 0, 35), gray_m));   // Ceiling
  scene.push_back(new Quad(vec3(-15.0, -3.7, -15.0), vec3(30, 0, 0), vec3(0, 25, 0), bricks_m)); // Back Wall
  scene.push_back(new Quad(vec3(-15.0, -3.7, -15.0), vec3(30, 0, 0), vec3(0, 0, 35), check_m));  // Floor
  scene.push_back(new Sphere(vec3(0.0, 23.3, -3.0), 2.5, light_m));
  scene.push_back(new Sphere(vec3(3.0, 0.0, 8.0), 2.3, glass_m));
  scene.push_back(new Sphere(vec3(3.0, -3.0, 8.0), 0.7, metal_m2));
  scene.push_back(new Sphere(vec3(-8.0, 0.8, -8.0), 4.5, metal_m));

  const std::string obj_file = "./obj/cow.obj";

  try
  {
    color bronze_color(0.9, 0.7, 0.2);
    auto bronze_tex = std::make_unique<solid_color>(bronze_color);
    auto bronze_mat = std::make_unique<metal>(bronze_color, 1.0, 0.05);
    metal *bronze_mat_ptr = bronze_mat.get();

    textures.push_back(std::move(bronze_tex));
    materials.push_back(std::move(bronze_mat));

    // Print bounding box of OBJ
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

bvh_node *setup_scene_3(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config)
{

  cam_config.position = vec3(0.0, 12.0, 35.0);
  cam_config.look_at = vec3(0.0, 0.0, -1.0);
  cam_config.up = vec3(0.0, -1.0, 0.0);
  cam_config.fov = 65.0;
  cam_config.aspect_ratio = 3.0 / 2.0;
  cam_config.background_color = 1;

  std::vector<Hittable *> scene;

  auto gray_tex = std::make_unique<solid_color>(color(0.1, 0.1, 0.1));
  auto lightGray_tex = std::make_unique<solid_color>(color(0.3, 0.3, 0.3));

  auto checkered = std::make_unique<checker_texture>(4, lightGray_tex.get(), gray_tex.get());

  auto *gray_ptr = gray_tex.get();
  auto *lightGray_ptr = lightGray_tex.get();
  auto *check_ptr = checkered.get();

  textures.push_back(std::move(gray_tex));
  textures.push_back(std::move(lightGray_tex));
  textures.push_back(std::move(checkered));

  auto metal_mat = std::make_unique<metal>(color(0.7, 0.7, 0.7), 1.0, 0.0);
  auto check_mat = std::make_unique<lambertian>(*check_ptr, 0.0);

  auto *metal_m = metal_mat.get();
  auto *check_m = check_mat.get();

  materials.push_back(std::move(metal_mat));
  materials.push_back(std::move(check_mat));

  int num_spheres = static_cast<int>(Util::random_double_range(70, 151));

  std::vector<vec3> sphere_centers;
  std::vector<double> sphere_radii;

  int max_attempts = 100;
  int placed = 0;

  while (placed < num_spheres)
  {
    double radius = Util::random_double_range(0.5, 2.8);
    double x = Util::random_double_range(-70.0, 70.0);
    double z = Util::random_double_range(-70.0, 40.0);

    // Skip center region
    if (std::abs(x) < 3.0 && std::abs(z) < 3.0)
      continue;

    vec3 new_center(x, radius, z);

    bool intersects = false;
    for (size_t i = 0; i < sphere_centers.size(); ++i)
    {
      double dist = (new_center - sphere_centers[i]).length();
      if (dist < (radius + sphere_radii[i]))
      {
        intersects = true;
        break;
      }
    }

    if (intersects)
      continue;

    // Random vibrant color using HSV
    double hue = Util::random_double(); // 0 to 1
    double sat = Util::random_double_range(0.8, 1.0);
    double val = Util::random_double_range(0.8, 1.0);
    color col = color::hsv_to_rgb(hue, sat, val);

    auto solid_tex = std::make_unique<solid_color>(col);
    texture *tex_ptr = solid_tex.get();
    textures.push_back(std::move(solid_tex));

    auto lambert_mat = std::make_unique<lambertian>(*tex_ptr, 0.0);
    material *mat_ptr = lambert_mat.get();
    materials.push_back(std::move(lambert_mat));

    scene.push_back(new Sphere(new_center, radius, mat_ptr));

    sphere_centers.push_back(new_center);
    sphere_radii.push_back(radius);

    ++placed;
  }

  scene.push_back(new Sphere(vec3(0.0, 10.0, 0.0), 10.0, metal_m));  // Center
  scene.push_back(new Sphere(vec3(0.0, -2000, 0.0), 2000, check_m)); // Floor

  for (auto *obj : scene)
    obj->bounding_box = obj->getBoundingBox();

  bvh_node *root = new bvh_node(scene.data(), 0, scene.size());

  for (auto &obj : scene)
    obj = nullptr;

  return root;
}
