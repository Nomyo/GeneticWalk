#include <entity.hh>
#include <character.hh>
#include <light.hh>

class EntityRenderer
{
public:
  EntityRenderer(Shader shader, glm::mat4 projection_mat, glm::mat4 view_mat, const Light& light);
  ~EntityRenderer() = default;

  void render(const std::vector<Entity>& entities);
  void render(const std::vector<Character>& characters);
  void prepare_instance(const Entity& entity);

private:
  Shader shader_;
};
