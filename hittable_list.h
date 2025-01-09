#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "triangle.h"
#include "sphere.h"
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
    std::cout << "HIT OBJECT LIST" << std::endl;
    double closest_hit = std::numeric_limits<double>::infinity(); // Start with a very large value
    for (const auto &obj : objects)
    {
      hit_record h;
      obj->hit(r, t_min, t_max, h); // Check if the ray hits this objecta
      double t = h.t;
      if (t > 0 && t < closest_hit) // If the hit is closer than the current closest hit
      {
        closest_hit = t;
        rec.normal = h.normal;
        rec.p = h.p;
        rec.t = h.t;
        rec.mat = mat;
      }
    }

    // If closest_hit hasn't been updated, return -1 (no intersection)
    return (closest_hit == std::numeric_limits<double>::infinity()) ? false : true;
  }

  // Override the getBoundingBox() method to compute the bounding box for all objects in the list
  AABB getBoundingBox() const override
  {
    if (objects.empty())
    {
      return AABB(vec3(0, 0, 0), vec3(0, 0, 0)); // Return an invalid AABB if the list is empty
    }

    AABB box = objects[0]->getBoundingBox(); // Start with the bounding box of the first object
    for (size_t i = 1; i < objects.size(); ++i)
    {
      AABB obj_box = objects[i]->getBoundingBox();
      box = AABB::combine(box, obj_box); // Combine bounding boxes to get the union of all objects' bounding boxes
    }

    return box;
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

    return triangle_list; // Return the populated HittableList
  }
};

#endif // HITTABLE_LIST_H