
#include <world.hh>
#include <model.hh>

World::World(unsigned int width, unsigned int height)
  : width_(width)
  , height_(height)
{
  create_mesh();
}

World::World(unsigned int width, unsigned int height, struct zone z)
  : endzone_(z)
  , width_(width)
  , height_(height)
{
  create_mesh();
}

World::World(Mesh mesh)
  : mesh_(mesh)
{

}

World::World(Mesh mesh, struct zone z)
  : mesh_(mesh)
  , endzone_(z)
{

}

void World::create_mesh()
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int z = 0; z < height_; ++z)
  {
    for (unsigned int x = 0; x < width_; ++x)
    {
      vertices.emplace_back(Vertex{glm::vec3(x, 0.0f, z),
	    glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(x, z),
	    glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}});
    }
  }

  for (unsigned int z = 0; z < height_ - 1; ++z)
  {
    for (unsigned int x = 0; x < width_ - 1; ++x)
    {
      int start = x + z * height_;
      indices.push_back(start);
      indices.push_back(start + 1);
      indices.push_back(start + height_);
      indices.push_back(start + 1);
      indices.push_back(start + 1 + height_);
      indices.push_back(start + height_);
    }
  }

  std::vector<Texture> textures;
  Texture texture;
  texture.id = TextureFromFile("textures/grass.jpg" ,false);
  texture.type = "texture_diffuse";
  texture.path = "textures/grass.jpg";
  textures.push_back(texture);

  mesh_ = Mesh(vertices, indices, textures);
}

void World::draw(Shader shader)
{
  mesh_.draw(shader);
}
