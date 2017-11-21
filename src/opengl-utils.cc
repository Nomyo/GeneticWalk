#include <opengl-utils.hh>
#include <cassert>
#include <character.hh>
#include <functional>
#include <ctime>

void framebuffer_size_callback(GLFWwindow */*window*/, int width, int height)
{
  glViewport(0, 0, width, height);
}

unsigned int gen_texture(const std::string& file)
{
  unsigned texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

  int width;
  int height;
  int nr_channels;

  stbi_set_flip_vertically_on_load(true);
  std::string tex = std::string{"textures/"} + file;
  unsigned char *data = stbi_load(tex.c_str(), &width, &height, &nr_channels, 0);

  assert(data);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

GLFWwindow *window_init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(GlobalConf::SCR_WIDTH,
					GlobalConf::SCR_HEIGHT,
					"GeneticWalk", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }

  auto& global_conf = GlobalConf::get_instance();
  auto camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f),
			   glm::vec3(0.0f, 1.0f, 0.0f));

  global_conf.init(camera);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, &GlobalConf::mouse_callback);
  glfwSetScrollCallback(window, &GlobalConf::scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  glEnable(GL_DEPTH_TEST);
  return window;
}

void init_character_models()
{
  for (auto n = 0u; n < Character::character_textures.size(); ++n)
  {
    std::string texture = "models/characters/Skins/Basic/" +
      Character::character_textures[n] + ".png";
    std::shared_ptr<Model> p(new Model("models/characters/Models/Non-rigged/basicCharacter.obj",
				 texture, "", false));
    Character::character_models.push_back(p);
  }
}


Model *choose_model()
{
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(0, 7);

  auto rand = std::bind(distr, eng);
  auto r = rand();

  return Character::character_models[r].get();
}

std::vector<Character> create_population(const World& w)
{
  auto population = std::vector<Character>{};

  // Create characters
  for (unsigned int i = 0; i < SIZE_POPULATION; ++i)
  {
    std::string texture = "";

    auto orc = Character(Entity(choose_model(), w.get_startpoint(),
				glm::vec3{0.0f, 0.0f, 0.0f}, 0.11f));

    population.push_back(orc);
  }

  // Set their DNA
  std::srand(std::time(0));
  for (auto& person : population)
  {
    // FIXME LENGTH DNA SHOULD be properly defined somewhere
    for (auto n = 0u; n < 400; ++n)
    {
      int r_nb = std::rand() % 8; // Mode the number of instruction
      person.add_to_DNA(static_cast<Character::Action>(r_nb));
    }
  }

  return population;
}
