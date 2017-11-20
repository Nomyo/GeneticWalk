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
  World(unsigned int width, unsigned int height, struct zone z,
	glm::vec3 startpoint);
  World(Mesh mesh);
  World(Mesh mesh, struct zone z);

  ~World() = default;

  void draw(Shader shader);
  void create_mesh();
  glm::vec3 get_startpoint() const;
  struct zone get_endzone() const;
  float dist_to_endzone(glm::vec3 p) const;
  bool in_endzone(glm::vec3 position) const;
  Mesh get_mesh() const;

private:
  Mesh mesh_;
  struct zone endzone_;
  glm::vec3 startpoint_;
  unsigned int width_;
  unsigned int height_;
};
