#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#include <cstdlib>
#include <iostream>
#include <string>

class rtw_image
{
public:
  // Constructors
  rtw_image();
  rtw_image(const char *image_filename);

  // Destructor
  ~rtw_image();

  // Load image from file
  bool load(const std::string &filename);

  // Get width and height
  int width() const;
  int height() const;

  // Get pixel data at (x, y)
  const unsigned char *pixel_data(int x, int y) const;

private:
  const int bytes_per_pixel = 3;  // RGB format
  float *fdata = nullptr;         // Linear floating point pixel data
  unsigned char *bdata = nullptr; // Linear 8-bit pixel data (converted)
  int image_width = 0;            // Image width
  int image_height = 0;           // Image height
  int bytes_per_scanline = 0;     // Number of bytes per row of pixels

  // Helper function to clamp values to a range [low, high).
  static int clamp(int x, int low, int high);

  // Converts a floating point value (0.0 to 1.0) to an 8-bit unsigned byte (0 to 255).
  static unsigned char float_to_byte(float value);

  // Converts the linear floating point pixel data to unsigned 8-bit byte data.
  void convert_to_bytes();
};

#endif