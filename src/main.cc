#include <main.hh>

int start_opengl()
{
  GLFWwindow *window = window_init();
  if (!window)
    return 1;
  auto& global_conf = GlobalConf::get_instance();
  auto camera = global_conf.get_camera();

  // Create our shader
  Shader our_shader("shaders/model.vs", "shaders/model.fs");

  auto orc_model = Model("models/characters/Models/Non-rigged/basicCharacter.obj",
			 "models/characters/Skins/Basic/skin_orc.png", "", false);

  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    GlobalConf::deltaTime = currentFrame - GlobalConf::lastFrame;
    GlobalConf::lastFrame = currentFrame;

    // input
    // -----
    global_conf.process_input(window);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // activate shader
    our_shader.use();

    glm::mat4 projection = glm::perspective(
      glm::radians(camera->get_zoom()),
      (float)GlobalConf::SCR_WIDTH / (float)GlobalConf::SCR_HEIGHT,
      0.1f, 100.0f);
    our_shader.setMat4("projection", projection);

    glm::mat4 view = camera->get_view_matrix();
    our_shader.setMat4("view", view);

    // render object
    // -------------
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    model = glm::rotate(model, glm::radians(0.0f),
			glm::vec3(1.0f, 0.3f, 0.5f));
    our_shader.setMat4("model", model);

    orc_model.draw(our_shader);

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
