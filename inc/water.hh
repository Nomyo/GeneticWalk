#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shader_m.hh>
#include <mesh.hh>

class Water
{
public:
  Water(glm::vec3 position, unsigned int width, unsigned int height);
  ~Water();

  void draw(Shader shader);
  void create_mesh();
  void create_frame_buffers();
  void bind_reflection_fb();
  void bind_refraction_fb();
  Mesh get_mesh() const;
  float get_height() const;

private:
  Mesh mesh_;
  glm::vec3 position_;

  // Reflection
  unsigned int reflectionFrameBuffer;
  unsigned int reflectionTexture;
  unsigned int reflectionDepthBuffer;
  unsigned int reflectionHeight = 1850;
  unsigned int reflectionWidth = 2400;

  // Refraction
  unsigned int refractionFrameBuffer;
  unsigned int refractionTexture;
  unsigned int refractionDepthBuffer;
  unsigned int refractionHeight = 1850;
  unsigned int refractionWidth = 2400;

  unsigned int dudv_texture_;
  unsigned int normal_map_;

  unsigned int width_;
  unsigned int height_;
};
