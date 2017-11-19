#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <entity.hh>
#include <string>
#include <vector>
#include <cstdint>

class Character : public Entity
{
public:
  static std::vector<std::string> character_textures;

  enum class Action : std::int8_t
  {
    MOVE_NORTH = 0,
    MOVE_NORTH_E = 1,
    MOVE_NORTH_W = 2,
    MOVE_SOUTH = 3,
    MOVE_SOUTH_E = 4,
    MOVE_SOUTH_W = 5,
    MOVE_WEST = 6,
    MOVE_EAST = 7,
  };

  enum class CharacterState : char
  {
    ALIVE,
    FALLING,
    DONE,
    DEAD
  };

public:
  Character(const Model& model, glm::vec3 position, glm::vec3 rotate,
	    float scale);
  Character(const Model& model, glm::vec3 position, glm::vec3 rotate,
	    float scale, std::vector<Action> dna);
  Character(const Entity& e, std::vector<Action> dna);
  Character(const Entity& entity);
  ~Character() = default;

  void update();
  void update_falling();
  void update_living();
  void add_to_DNA(Action a);

private:
  float speed_ = 0.25f;
  std::vector<Action> DNA_instructions_;
  std::uint64_t DNA_index_ = 0;
  CharacterState state_ = CharacterState::ALIVE;
};
