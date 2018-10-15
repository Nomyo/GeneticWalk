#include <character.hh>
#include <functional>

std::vector<std::string> Character::character_textures =
{
  "skin_adventurer", "skin_exclusiveCowboy",
  "skin_exclusiveDummy", "skin_exclusiveFembot",
  "skin_exclusiveGeisha", "skin_man", "skin_orc",
  "skin_soldier"
};

std::vector<std::shared_ptr<Model>> Character::character_models{};

Character::Character(Model *model, glm::vec3 position, glm::vec3 rotate,
    float scale)
    : Entity(model, position, rotate, scale)
{ }

Character::Character(const Entity& e)
    : Entity(e)
{ }

Character::Character(Model *model, glm::vec3 position, glm::vec3 rotate,
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

void Character::update(const World& w)
{
    switch (state_)
    {
    case CharacterState::ALIVE:
        update_living(w);
        break;
    case CharacterState::FALLING:
        update_falling();
        break;
    case CharacterState::DEAD:
        positions_[get_position()] += 1;
        break;
    case CharacterState::DONE:
        break;
    case CharacterState::SUCCESS:
        break;
    default:
        assert(0);
        break;
    }

    if (DNA_index_ > DNA_instructions_.size() - 1)
        state_ = CharacterState::DONE;
}

void Character::update_falling()
{
    direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
    increase_position(direction_ * speed_);
    positions_[get_position()] += 0.5;
}

void Character::update_living(const World& w)
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
    if ((position.x < 0 || position.x > 50) ||
        (position.z < 0 || position.z > 50))
        state_ = CharacterState::FALLING;

    if (w.in_endzone(position))
        state_ = CharacterState::SUCCESS;

    positions_[position] += 1;
}

void Character::reset(const World& w)
{
    DNA_index_ = 0;
    state_ = CharacterState::ALIVE;
    set_position(w.get_startpoint());
}


auto Character::get_DNA() const -> const std::vector<Action>&
{
    return DNA_instructions_;
}

void Character::switch_DNA(unsigned int index, Action a)
{
    DNA_instructions_[index] = a;
}

void Character::replace_DNA(const std::vector<Action>& dna)
{
    DNA_instructions_ = dna;
}

void Character::set_state(CharacterState s)
{
    state_ = s;
}

auto Character::get_state() const -> CharacterState
{
    return state_;
}

auto Character::get_position_passed() const -> const Pos_map
{
  return positions_;
}

bool Character::dead_or_done() const
{
    return state_ != CharacterState::ALIVE;
}
