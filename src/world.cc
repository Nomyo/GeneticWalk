#include <world.hh>
#include <model.hh>

World::World(unsigned int width, unsigned int height)
  : width_(width)
  , height_(height)
{
  create_mesh();
}

World::World(unsigned int width, unsigned int height, struct zone z,
	     glm::vec3 startpoint)
  : endzone_(z)
  , startpoint_(startpoint)
  , width_(width)
  , height_(height)
{
  create_mesh();
}

World::World(const std::string& heightmap, struct zone z,
    glm::vec3 startpoint)
    : endzone_(z)
    , startpoint_(startpoint)
{
    read_height_map(heightmap);
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

void World::read_height_map(const std::string& heightmap)
{
    int width;
    int height;
    int nr_channels;

    unsigned char *data = stbi_load(heightmap.c_str(), &width, &height, &nr_channels, 3);
    assert(data);

    width_ = width;
    height_ = height;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int z = 0; z < height_; ++z) {
        for (int x = 0; x < width_; ++x) {
            unsigned char* pixelOffset = data + (x + height_ * z) * nr_channels;
            unsigned char r = pixelOffset[0];
            unsigned char g = pixelOffset[1];
            unsigned char b = pixelOffset[2];
            auto height = ((r + g + b) / 3.0f) / 15.0f;
            vertices.emplace_back(Vertex{ glm::vec3(x, height - 16, z),
                glm::vec3(0.0f, 2.0f, 0.0f), glm::vec2(x, z),
                glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f } });
        }
    }

    stbi_image_free(data);

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
    texture.id = TextureFromFile("textures/grass2.jpg", false);
    texture.type = "texture_diffuse";
    texture.path = "textures/grass2.jpg";
    textures.push_back(texture);

    Texture texture2;
    texture2.id = TextureFromFile("textures/shulker_top_red.png", false);
    texture2.type = "texture_diffuse";
    texture2.path = "shulker_top_red.png";
    textures.push_back(texture2);

    Texture texture3;
    texture3.id = TextureFromFile("textures/sand.jpg", false);
    texture3.type = "texture_diffuse";
    texture3.path = "textures/sand.jpg";
    textures.push_back(texture3);

    Texture texture4;
    texture4.id = TextureFromFile("textures/grass_dirt.jpg", false);
    texture4.type = "texture_diffuse";
    texture4.path = "textures/grass_dirt.jpg";
    textures.push_back(texture4);

    Texture texture5;
    texture5.id = TextureFromFile("map/texture_map.png", false);
    texture5.type = "texture_diffuse";
    texture5.path = "map/texture_map.png";
    textures.push_back(texture5);


    mesh_ = Mesh(vertices, indices, textures);
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
          glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(x, z),
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
  texture.id = TextureFromFile("textures/grass2.jpg", false);
  texture.type = "texture_diffuse";
  texture.path = "textures/grass2.jpg";
  textures.push_back(texture);

  Texture texture2;
  texture2.id = TextureFromFile("textures/shulker_top_red.png", false);
  texture2.type = "texture_diffuse";
  texture2.path = "shulker_top_red.png";
  textures.push_back(texture2);

  mesh_ = Mesh(vertices, indices, textures);
}

void World::draw(Shader shader)
{
  mesh_.draw(shader);
}

glm::vec3 World::get_startpoint() const
{
  return startpoint_;
}

struct zone World::get_endzone() const
{
  return endzone_;
}

float World::dist_to_endzone(glm::vec3 p) const
{
  return sqrt(pow((endzone_.coord.x - p.x), 2) + pow((endzone_.coord.y - p.z), 2));
}

Mesh World::get_mesh() const
{
  return mesh_;
}

bool World::in_endzone(glm::vec3 position) const
{
  float nq_endz_x = (position.x - endzone_.coord.x);
  float nq_endz_y = (position.z - endzone_.coord.y);
  float sq_endz_r = endzone_.radius * endzone_.radius;

  return (nq_endz_x * nq_endz_x + nq_endz_y * nq_endz_y) < sq_endz_r;
}
