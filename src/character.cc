#include <character.hh>
#include <functional>

std::vector<std::string> Character::character_textures =
{
  "skin_adventurer", "skin_exclusiveCowboy",
  "skin_exclusiveDummy", "skin_exclusiveFembot",
  "skin_exclusiveGeisha", "skin_man", "skin_orc",
  "skin_soldier"
};

Character::Character(const Model& model, glm::vec3 position, glm::vec3 rotate,
	  float scale)
  : Entity(model, position, rotate, scale)
{ }

Character::Character(const Entity& e)
  : Entity(e)
{ }

Character::Character(const Model& model, glm::vec3 position, glm::vec3 rotate,
		     float scale, std::vector<Action> dna)
  : Entity(model, position, rotate, scale)
  , DNA_instructions_(dna)
{ }

Character::Character(const Entity& e, std::vector<Action> dna)
  : Entity(e)
  , DNA_instructions_(dna)
{ }

void Character::add_to_DNA(Action a)
{
  DNA_instructions_.emplace_back(a);
}

void Character::update()
{
  if (DNA_index_ > DNA_instructions_.size() - 1)
    return;

  switch (state_)
  {
  case CharacterState::ALIVE:
    update_living();
    break;
  case CharacterState::FALLING:
    update_falling();
    break;
  case CharacterState::DEAD:
    break;
  default:
    assert(0);
    break;
  }
}

void Character::update_falling()
{
  direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
  increase_position(direction_ * speed_);
}

void Character::update_living()
{
  switch (DNA_instructions_[DNA_index_++])
  {
  case Action::MOVE_WEST:
    direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_NORTH:
    direction_ = glm::vec3(0.0f, 0.0f, 1.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_EAST:
    direction_ = glm::vec3(-1.0f, 0.0f, 0.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_SOUTH:
    direction_ = glm::vec3(0.0f, 0.0f, -1.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_NORTH_E:
    direction_ = glm::vec3(-1.0f, 0.0f, 1.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_NORTH_W:
    direction_ = glm::vec3(1.0f, 0.0f, 1.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_SOUTH_E:
    direction_ = glm::vec3(-1.0f, 0.0f, -1.0f);
    increase_position(direction_ * speed_);
    break;
  case Action::MOVE_SOUTH_W:
    direction_ = glm::vec3(1.0f, 0.0f, -1.0f);
    increase_position(direction_ * speed_);
    break;
  default:
    break;
  }

  glm::vec3 position = get_position();
  if ((position.x < 0 || position.x > 100) ||
      (position.z < 0|| position.z > 100))
    state_ = CharacterState::FALLING;
}
