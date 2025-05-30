#ifndef SCENE_SETUP_H
#define SCENE_SETUP_H

#include <vector>
#include <memory>
#include "hittable.h"
#include "material.h"
#include "bvh.h"

// This function creates the scene and returns the BVH root node.
bvh_node *setup_scene_1(std::vector<std::unique_ptr<material>> &materials, std::vector<std::unique_ptr<texture>> &textures);
bvh_node *setup_scene_2(std::vector<std::unique_ptr<material>> &materials, std::vector<std::unique_ptr<texture>> &textures);

#endif