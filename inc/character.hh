#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <entity.hh>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include <world.hh>

struct GVec3Comp
{
  bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const
    {
      return (lhs.x < rhs.x || lhs.x == rhs.x ) &&
	(lhs.y < rhs.y || lhs.y == rhs.y) && (lhs.z < rhs.z);
    }
};

class Character : public Entity
{
public:
  using Pos_map = std::map<glm::vec3, unsigned int, GVec3Comp>;
  static std::vector<std::string> character_textures;
  static std::vector<std::shared_ptr<Model>> character_models;

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
    DEAD,
    SUCCESS
  };

public:
  Character(Model *model, glm::vec3 position, glm::vec3 rotate,
	    float scale);
  Character(Model *model, glm::vec3 position, glm::vec3 rotate,
	    float scale, std::vector<Action> dna);
  Character(const Entity& e, std::vector<Action> dna);
  Character(const Entity& entity);
  ~Character() = default;

  void update(const World& w);
  void update_falling();
  void update_living(const World& w);
  void add_to_DNA(Action a);
  const std::vector<Action>& get_DNA() const;
  void switch_DNA(unsigned int index, Action a);
  void replace_DNA(const std::vector<Action>& dna);
  void set_state(CharacterState s);
  CharacterState get_state() const;
  void reset(const World& w);
  const Pos_map get_position_passed() const;
  bool dead_or_done() const;

private:
  float speed_ = 0.15f;
  std::vector<Action> DNA_instructions_;
  std::uint64_t DNA_index_ = 0;
  CharacterState state_ = CharacterState::ALIVE;

  Pos_map positions_;
};
