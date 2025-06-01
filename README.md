# Advanced C++ Ray Tracer

A physically-based ray tracer implemented in C++ featuring realistic lighting, materials, textures, and geometry rendering with BVH acceleration.

![Rendered Image](images/render_screenshot.png)

## Features

### 🎨 **Materials & Shading**
- **Lambertian (Diffuse)**: Realistic matte surfaces with customizable reflectivity
- **Metal**: Reflective surfaces with adjustable fuzziness for realistic metal rendering
- **Dielectric (Glass)**: Transparent materials with refraction and reflection
- **Emissive Materials**: Light-emitting surfaces for realistic illumination

### 🖼️ **Advanced Texturing**
- **Solid Colors**: Basic color textures
- **Procedural Noise**: Perlin noise-based textures for natural-looking surfaces
- **Checker Patterns**: Configurable checkerboard textures
- **Image Textures**: Support for loading external images (JPG format)
- **UV Mapping**: Proper texture coordinate mapping for spheres and other primitives

### 🎯 **Geometry Support**
- **Spheres**: With proper UV mapping for textures
- **Triangles**: Individual triangle primitives
- **Quads**: Quadrilateral surfaces for walls, floors, etc.
- **OBJ File Loading**: Import complex 3D models from OBJ file and renders trianlge mesh

### ⚡ **Performance Optimization**
- **Multi-Threading**: Leverages multithreading (via C++ threads) to utilize open CPU cores for faster generation
- **BVH (Bounding Volume Hierarchy)**: Efficient ray-object intersection acceleration
- **AABB (Axis-Aligned Bounding Boxes)**: Fast spatial partitioning

### 🔍 **Advanced Ray Tracing Features**
- **Recursive Ray Bounces**: Realistic light transport with configurable depth
- **Gamma Correction**: Proper color space handling for realistic output
- **Depth of Field**: Camera focus effects (infrastructure present)
- **Anti-aliasing**: Multi-sample anti-aliasing for smooth edges

### 📷 **Camera System**
- **Flexible Positioning**: Full camera control
- **Field of View**: Adjustable perspective
- **Aspect Ratio Support**: Configurable output dimensions
- **Look-at System**: Intuitive camera aiming

### 🎬 Preset Scene Selection
- Supports multiple preset scenes
- Select scenes at runtime using a command-line number:
- Create your own scenes in `scene_setup.cpp`

![Rendered Image](images/cow_render.png)

## Getting Started

### Prerequisites
- C++14 compatible compiler (g++, clang++)
- STB Image library (included as `rtw_stb_image.h`)

### Compilation
```bash
g++ -std=c++11 project.cpp camera.cpp rtw_stb_image.cpp -o raytracer
```

### Basic Usage
```bash
  # Render the default scene
  ./raytracer 1

  #Render second preset scene
  ./raytracer 2

The program will output a PPM image file named `output.ppm`.

## Scene Configuration

The main scene is configured in `project.cpp`. You can customize:

### Materials
```cpp
// Create different materials
dielectric *glass_mat = new dielectric(1.5, 0.8);          // Glass with refraction
lambertian *noise_mat = new lambertian(perlin_texture, 0.85); // Textured surface
metal *metal_mat = new metal(grey, 1.0, 0.0);           // Shiny metal
diffuse_light *light_mat = new diffuse_light(&light_texture); // Light source
```

### Geometry
```cpp
// Add spheres with different materials
hittables.push_back(new Sphere(vec3(0.0, 0.8, 6.0), 0.8, basketball_mat));
hittables.push_back(new Sphere(vec3(4.5, 1.8, 4.0), 1.8, glass_mat));

// Add quads with any materials
hittables.push_back(new Quad(vec3(-8.0, 12.0, 8.0), vec3(16, 0, 0), vec3(0, 0, 16), light_mat));
```

### Camera Settings
```cpp
  cam_config.position = vec3(0.0, 8.0, 25.0);
  cam_config.look_at = vec3(0.0, 0.0, -1.0);
  cam_config.up = vec3(0.0, -1.0, 0.0);
  cam_config.fov = 65.0;
  cam_config.aspect_ratio = 3.0 / 2.0;
  cam_config.background_color = 1;
```

## File Structure

- **`project.cpp`** - Main program and scene setup
- **`camera.h/cpp`** - Camera implementation and rendering pipeline
- **`scene_setup.h/cpp`** - Defines camera configuration, textures, materials, and objects in scene
- **`ray.h`** - Ray class with reflection/refraction utilities
- **`vec3.h`** - 3D vector mathematics 
- **`color.h`** - Color handling with gamma correction
- **`material.h`** - Material system (Lambertian, Metal, Dielectric, Emissive)
- **`texture.h`** - Texture system (Solid, Noise, Checker, Image)
- **`hittable.h`** - Base class for renderable objects
- **`sphere.h`** - Sphere primitive implementation
- **`triangle.h`** - Triangle primitive with Möller-Trumbore intersection
- **`quad.h`** - Quad primitive 
- **`hittable_list.h`** - Object collections with OBJ file loading
- **`bvh.h`** - Bounding Volume Hierarchy acceleration structure
- **`aabb.h`** - Axis-Aligned Bounding Box implementation
- **`util.h`** - Utility functions 
- **`perlin.h`** - Perlin noise implementation 

## Supported Texture Files

Place texture files in the images directory:
- Already populated with textures used in preset scenes
- Any JPG files for image textures

## Performance Notes

The ray tracer includes several optimizations:
- **BVH Acceleration**: Logarithmic intersection testing for complex scenes
- **Multi-Threading**: Leverages multithreading (via C++ threads) to utilize open CPU cores for faster generation
- **Configurable Quality**: Adjust `samples_per_pixel` vs render time
- **Image Resolution**: Modify `IW` constant in `project.cpp` (240, 480, 960, 1920, 3840)

### Render Quality Settings
- **Fast Preview**: 10-30 samples, 480px width
- **Production**: 100-500+ samples, 1920px+ width
- **High Quality**: 1000+ samples for final renders

## Example Scenes

The default scenes include all features:
- Textured Earth sphere with image mapping
- Glass spheres with realistic refraction
- Metal spheres with varying fuzziness
- Procedural noise textures
- Checkered textures
- Area lights for realistic illumination
- OBJ model loading

## Known Issues

- Motion blur - supported for spheres, but deprecated (Notes in sphere.h)
- BVH performance may degrade with very dense triangle meshes
- OBJ loader supports basic triangular faces only

## Future Enhancements

Potential improvements for the ray tracer:
- Volumetric rendering (fog, smoke)
- Subsurface scattering
- Advanced lighting models (PBR)
- GPU acceleration
- Real-time preview
- More primitive types (cylinders, prisms, etc.)

Feel free to extend and modify for educational or personal projects!
