#ifndef SCENE_SETUP_H
#define SCENE_SETUP_H

#include <vector>
#include <memory>
#include "util.h"
#include "hittable.h"
#include "material.h"
#include "bvh.h"
#include "vec3.h"
#include "color.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "texture.h"
#include "hittable_list.h"
#include "color.h"

struct CameraConfig
{
  vec3 position;
  vec3 look_at;
  vec3 up;
  double fov;
  double aspect_ratio;
  int background_color;
};

// This function creates the scene and returns the BVH root node.
bvh_node *setup_scene_1(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config);

bvh_node *setup_scene_2(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config);

bvh_node *setup_scene_3(std::vector<std::unique_ptr<material>> &materials,
                        std::vector<std::unique_ptr<texture>> &textures,
                        CameraConfig &cam_config);

#endif