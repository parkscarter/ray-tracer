#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm> // For std::sort

class bvh_node : public Hittable
{
public:
  Hittable *left;
  Hittable *right;
  // Constructor that accepts a vector of hittable objects and start/end indices
  bvh_node(Hittable **objects, size_t start, size_t end)
  {

    // std::cout << "bvh_constructor" << std::endl;
    //   Choose a random axis to split along (0 = x-axis, 1 = y-axis, 2 = z-axis)
    int axis = Util::random_int(0, 2);

    // Choose a comparison function based on the axis
    auto comparator = (axis == 0)   ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1)
    {
      left = objects[start];

      right = objects[start];
    }
    else if (object_span == 2)
    {
      // std::cout << "FIRST " << left->bounding_box.min << std::endl;
      left = objects[start];
      right = objects[start + 1];
    }
    else
    {
      // Sort the objects based on the chosen axis
      std::sort(objects + start, objects + end, comparator);

      size_t mid = start + object_span / 2;
      left = new bvh_node(objects, start, mid);
      right = new bvh_node(objects, mid, end);
    }

    // Combine the bounding boxes of the left and right children to get the bounding box of this node
    bbox = AABB::combine(left->getBoundingBox(), right->getBoundingBox());
  }

  // Recursively Check if the ray intersects with this BVH node and its children
  bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
  {
    //  First, check if the ray intersects the bounding box of this node
    if (!bbox.hit(r, t_min, t_max))
      return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
  }

  // Return the bounding box of this BVH node
  AABB getBoundingBox() const override { return bbox; }

  // Destructor to clean up dynamic memory
  ~bvh_node()
  {
    delete left;
    delete right;
  }

private:
  // Bounding box for this node
  AABB bbox;

  // Comparison functions for sorting objects along each axis
  static bool box_x_compare(const Hittable *a, const Hittable *b)
  {
    return a->getBoundingBox().min.x < b->getBoundingBox().min.x;
  }

  static bool box_y_compare(const Hittable *a, const Hittable *b)
  {
    return a->getBoundingBox().min.y < b->getBoundingBox().min.y;
  }

  static bool box_z_compare(const Hittable *a, const Hittable *b)
  {
    return a->getBoundingBox().min.z < b->getBoundingBox().min.z;
  }
};

#endif // BVH_H