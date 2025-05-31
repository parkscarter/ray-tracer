#include "camera.h"

Camera::Camera(int width, double a_ratio, vec3 center, vec3 look_at, vec3 up, double fov, int samples, int background_color, bvh_node *scene_root)
    : aspect_ratio(a_ratio),
      image_width(width),
      center(center),
      fov(fov),
      background_mode(background_color),
      scene_root(scene_root),
      samples_per_pixel(samples)
{
  // Calculate image height
  image_height = static_cast<int>(width / a_ratio);

  // Calculate camera orientation
  forward = vec3::unit_vector(vec3::sub(look_at, center)); // Normalize forward
  vec3 up_normalized = vec3::unit_vector(up);
  right = vec3::unit_vector(vec3::cross(up_normalized, forward));
  up_normalized = vec3::cross(forward, right);
  up_normalized = vec3::unit_vector(up_normalized);
  up = up_normalized;

  // Calculate the viewport based on field of view
  double focal_length = 1.0;                                    // Typically, the focal length is 1 unit
  double viewport_height = 2.0 * tan(fov * 0.5 * M_PI / 180.0); // Convert FOV to radians
  double viewport_width = viewport_height * a_ratio;

  // Viewport u (horizontal) and v (vertical) directions
  vec3 viewport_u = vec3::scale(right, viewport_width);
  vec3 viewport_v = vec3::scale(up_normalized, viewport_height);

  // Difference in position between adjacent pixels
  pixel_delta_u = vec3::scale(viewport_u, 1.0 / width);
  pixel_delta_v = vec3::scale(viewport_v, 1.0 / image_height);

  // Upper left corner of the viewport (where the camera is looking from)
  vec3 d_u = vec3::scale(viewport_u, 0.5);
  vec3 d_v = vec3::scale(viewport_v, 0.5);
  vec3 focal_length_vector(0.0, 0.0, focal_length);

  vec3 vp_upper_left = vec3::sub(center, focal_length_vector);
  vp_upper_left = vec3::sub(vp_upper_left, d_u);
  vp_upper_left = vec3::sub(vp_upper_left, d_v);

  // Pixel location for the first pixel (00)
  pixel_00_loc = vec3::add(pixel_delta_u, pixel_delta_v);
  pixel_00_loc = vec3::scale(pixel_00_loc, 1 / 2);
  pixel_00_loc = vec3::add(pixel_00_loc, vp_upper_left);
}

color Camera::ray_color(const ray &r, int depth) const
{
  if (depth <= 0)
    return color(0.0, 0.0, 0.0); // Stop if depth is 0 or below

  double t_min = 0.01, t_max = std::numeric_limits<double>::infinity();
  hit_record h;

  if (!scene_root->hit(r, t_min, t_max, h))
  {
    if (background_mode == 1)
    {
      color background;
      vec3 unit_direction = vec3::unit_vector(r.direction);
      double a = 0.5 * (unit_direction.y + 1.0);
      color white(1.0, 1.0, 1.0);
      color light_blue(0.1, 0.5, 1.0); // Light blue color
      background.value = (white.value * (1.0 - a)) + (light_blue.value * a);
      return background;
    }
    else if (background_mode == 2)
    {
      color dark(0.005, 0.005, 0.005);
      return dark;
    }
    else
    {
      color black(0.0, 0.0, 0.0);
      return black;
    }
  }

  ray scattered;
  color attenuation, result, color_from_emission;

  color_from_emission = h.mat->emitted(h.u, h.v, h.p);

  if (!h.mat->scatter(r, h, attenuation, scattered))
  {
    return color_from_emission;
  }

  color color_from_scatter;
  color_from_scatter.value = attenuation.value * ray_color(scattered, depth - 1).value;
  result.value = color_from_emission.value + color_from_scatter.value;
  return result.gamma_corrected();
}

// This function grabs a sample ray where (i, j) is a position on the viewport
ray Camera::get_ray(int i, int j) const
{
  vec3 offset = sample_square(); // Offset will be a point within a ([-0.5 - 0.5], [-0.5, 0.5]) range
  vec3 sample, dif_x, dif_y;
  dif_x = vec3::scale(pixel_delta_u, i + offset.x);
  dif_y = vec3::scale(pixel_delta_v, j + offset.y);
  sample = vec3::add(pixel_00_loc, dif_x);
  sample = vec3::add(sample, dif_y);

  Point ray_origin = center;
  vec3 direction = vec3::sub(sample, ray_origin);
  double ray_time = Util::random_double();

  return ray(ray_origin, direction, ray_time);
}

// render iterates across the viewport, getting rays, and their color, then writing that color
// to the output image.
void Camera::render() const
{
  std::vector<std::vector<color>> image(image_height, std::vector<color>(image_width));
  int thread_count = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  std::atomic<int> lines_done(0);

  // Progress monitor thread
  std::thread progress_thread([&]()
                              {
    int last_reported = -1;
    while (lines_done < image_height) {
      int current = lines_done.load();
      if (current != last_reported) {
        std::cout << "Lines completed: " << current << " / " << image_height << "\r" << std::flush;
        last_reported = current;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    }
    std::cout << "Lines completed: " << image_height << " / " << image_height << "\n"; });

  // Worker threads
  auto render_rows = [&](int start_row, int end_row)
  {
    for (int j = start_row; j < end_row; ++j)
    {
      for (int i = 0; i < image_width; ++i)
      {
        color pixel_color;
        for (int sample = 0; sample < samples_per_pixel; ++sample)
        {
          ray r = get_ray(i, j);
          color color_sample = ray_color(r, 50);
          pixel_color.value = vec3::add(pixel_color.value, color_sample.value);
        }
        pixel_color.value = vec3::scale(pixel_color.value, 1.0 / samples_per_pixel);
        image[j][i] = pixel_color;
      }
      ++lines_done;
    }
  };

  int rows_per_thread = image_height / thread_count;
  for (int t = 0; t < thread_count; ++t)
  {
    int start = t * rows_per_thread;
    int end = (t == thread_count - 1) ? image_height : start + rows_per_thread;
    threads.emplace_back(render_rows, start, end);
  }

  // Join worker threads
  for (auto &t : threads)
  {
    t.join();
  }

  // Wait for progress thread
  progress_thread.join();

  // Write image to file
  FILE *image_file = fopen("output.ppm", "w");
  if (!image_file)
  {
    std::cerr << "Error: could not open output file.\n";
    return;
  }
  fprintf(image_file, "P3\n%d %d\n255\n", image_width, image_height);
  for (int j = 0; j < image_height; ++j)
  {
    for (int i = 0; i < image_width; ++i)
    {
      image[j][i].write_to_file(image_file);
    }
  }
  fclose(image_file);
  std::cerr << "\nDone.\n";
}

vec3 Camera::sample_square() const
{
  double a = Util::random_double() - 0.5;
  double b = Util::random_double() - 0.5;
  return vec3(a, b, 0.0);
}