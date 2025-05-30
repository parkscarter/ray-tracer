#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "triangle.h"
#include "sphere.h"
#include "bvh.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

class HittableList : public Hittable
{
public:
  // List to hold multiple Hittable objects (either Triangles, Spheres, etc.)
  std::vector<Hittable *> objects;
  material *mat;
  AABB bbox;
  bvh_node *local_bvh = nullptr;

  // Constructor that initializes the list with reflectivity
  // HittableList(double reflectivity = 0.0) : Hittable(reflectivity) {}

  // Method to add objects to the list
  void add(Hittable *object)
  {
    objects.push_back(object);
  }

  // Override the hit() method to check intersection with all objects in the list
  bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
  {
    if (!getBoundingBox().hit(r, t_min, t_max))
    {
      return false; // Early exit if ray misses the overall AABB
    }
    return local_bvh && local_bvh->hit(r, t_min, t_max, rec);
  }

  void
  computeBoundingBox()
  {
    if (objects.empty())
    {
      bbox = AABB(vec3(0, 0, 0), vec3(0, 0, 0)); // or mark as invalid
      return;
    }

    bbox = objects[0]->getBoundingBox();
    for (size_t i = 1; i < objects.size(); ++i)
    {
      bbox = AABB::combine(bbox, objects[i]->getBoundingBox());
    }
  }

  // Override the getBoundingBox() method to compute the bounding box for all objects in the list
  AABB getBoundingBox() const override
  {
    return bbox;
  }

  static HittableList load_triangles_from_obj(const std::string &filename, material *mat)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open OBJ file");
    }

    std::vector<vec3> vertices; // Store the vertices (points)
    HittableList triangle_list; // HittableList to store the triangles

    triangle_list.mat = mat;

    std::string line;
    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      std::string prefix;

      // Read the type of line (v for vertex, f for face)
      iss >> prefix;

      // Process vertex lines (v x y z)
      if (prefix == "v")
      {
        double x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(vec3(x, y, z));
      }
      // Process face lines (f v1 v2 v3)
      else if (prefix == "f")
      {
        int v1, v2, v3;

        // Check if the face line has 3 vertex indices
        if (iss >> v1 >> v2 >> v3)
        {
          // OBJ format face indices are 1-based, so subtract 1 to make them 0-based
          v1--;
          v2--;
          v3--;

          // Ensure the vertex indices are valid
          if (v1 >= 0 && v2 >= 0 && v3 >= 0 && v1 < vertices.size() && v2 < vertices.size() && v3 < vertices.size())
          {
            // Add triangle to the HittableList
            // std::cout << reflectivity << std::endl;
            triangle_list.add(new Triangle(vertices[v1], vertices[v2], vertices[v3], mat));
          }
          else
          {
            std::cerr << "Invalid vertex indices for face: " << v1 + 1 << ", " << v2 + 1 << ", " << v3 + 1 << std::endl;
          }
        }
        else
        {
          std::cerr << "Error parsing face line: " << line << std::endl;
        }
      }
    }

    file.close();

    triangle_list.computeBoundingBox();
    triangle_list.local_bvh = new bvh_node(triangle_list.objects.data(), 0, triangle_list.objects.size());

    return triangle_list; // Return the populated HittableList
  }
};

#endif // HITTABLE_LIST_H