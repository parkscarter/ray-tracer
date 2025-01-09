#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>

class Util
{
public:
  static const double pi;

  static double linear_to_gamma(double lin_comp)
  {
    if (lin_comp > 0.0)
    {
      return std::sqrt(lin_comp);
    }
    return 0.0;
  }
  // Convert degrees to radians
  static double degrees_to_radians(double degrees)
  {
    return degrees * pi / 180.0;
  }

  // Generate a random double in the range [0, 1)
  static double random_double()
  {
    static std::random_device rd;
    static std::mt19937 gen(rd()); // Mersenne Twister RNG
    static std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
  }

  // Generate a random double in a specified range [min, max)
  static double random_double_range(double min, double max)
  {
    std::uniform_real_distribution<> dis(min, max);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return dis(gen);
  }

  static int random_int(int a, int b)
  {
    return a + rand() % (b - a + 1);
  }

  // Clamp a number between a specified min and max range
  static double clamp(double min, double max, double num)
  {
    if (num < min)
    {
      return min;
    }
    else if (num > max)
    {
      return max;
    }
    else
    {
      return num;
    }
  }

  static void print_obj_bounding_box(const std::string &filename)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      std::cerr << "Could not open OBJ file: " << filename << std::endl;
      return;
    }

    // Initialize min/max values to extreme values
    double minX = std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    double minZ = std::numeric_limits<double>::infinity();
    double maxZ = -std::numeric_limits<double>::infinity();

    std::string line;
    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      std::string prefix;
      iss >> prefix;

      if (prefix == "v")
      {
        // Parse vertex (v x y z)
        double x, y, z;
        iss >> x >> y >> z;

        // Update the bounding box
        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
        minZ = std::min(minZ, z);
        maxZ = std::max(maxZ, z);
      }
    }

    file.close();

    // Print the min/max values
    std::cout << "Bounding box:" << std::endl;
    std::cout << "X: [" << minX << ", " << maxX << "]" << std::endl;
    std::cout << "Y: [" << minY << ", " << maxY << "]" << std::endl;
    std::cout << "Z: [" << minZ << ", " << maxZ << "]" << std::endl;
  }
};
// const double Util::pi = 3.14159;
#endif // UTIL_H