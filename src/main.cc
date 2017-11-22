#include <main.hh>

int start_opengl()
{
  GLFWwindow *window = window_init();
  if (!window)
    return 1;
  auto& global_conf = GlobalConf::get_instance();
  init_character_models();
  auto camera = global_conf.get_camera();
  camera->set_camera(glm::vec3(30.0f, 15.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		     90.0f, 0.0f);

  // Create our shader
  Shader our_shader("shaders/model.vs", "shaders/model.fs");
  Shader world_shader("shaders/world.vs", "shaders/world.fs");

  struct zone ez;
  ez.coord = glm::vec2(25.0f, 45.0f);
  ez.radius = 1.f;
  auto world = World(50, 50, ez, glm::vec3(25.0f, 0.0f, 5.0f));
  auto population = create_population(world);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float updateFrame = 0.0f;
  long nb_gen = 0;
  std::cout << "GEN : " << nb_gen++ << std::endl;
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    GlobalConf::deltaTime = currentFrame - GlobalConf::lastFrame;
    GlobalConf::lastFrame = currentFrame;
    updateFrame += GlobalConf::deltaTime;

    // input
    // -----
    global_conf.process_input(window);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
      glm::radians(camera->get_zoom()),
      (float)GlobalConf::SCR_WIDTH / (float)GlobalConf::SCR_HEIGHT,
      0.1f, 500.0f);

    glm::mat4 view = camera->get_view_matrix();

    // render object
    // -------------
    WorldRenderer wr(world_shader, projection, view);
    wr.render(world);

    EntityRenderer er(our_shader, projection, view);
    er.render(population);

    // update_population
    // -----------------
    if (updateFrame >= 0.005f)
    {
      for (auto& p : population)
	p.update(world);

      updateFrame = 0.0f;
    }

    if (std::all_of(population.begin(), population.end(), [](Character c)
		    { return c.dead_or_done(); }))
    {
      std::cout << "GEN : " << nb_gen++ << std::endl;
      population = create_next_generation(population, world);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  return start_opengl();
}
