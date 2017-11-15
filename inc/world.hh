#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <mesh.hh>
#include <shader_m.hh>

struct zone
{
  glm::vec2 coord;
  float radius;
};

class World
{
public:
  World(unsigned int width, unsigned int height);
  World(unsigned int width, unsigned int height, struct zone z);
  World(Mesh mesh);
  World(Mesh mesh, struct zone z);

  ~World() = default;

  void draw(Shader shader);
  void create_mesh();

private:
  Mesh mesh_;
  struct zone endzone_;
  unsigned int width_;
  unsigned int height_;
};
