#include <main.hh>

int start_opengl()
{
  GLFWwindow *window = window_init();
  if (!window)
    return 1;
  auto& global_conf = GlobalConf::get_instance();
  auto camera = global_conf.get_camera();

  unsigned int VBO;
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned int texture1 = gen_texture("grass.jpg");

  // Create our shader
  Shader our_shader("shaders/basic.vs", "shaders/basic.fs");
  //our_shader.use();
  //our_shader.setInt("texture1", 0);

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

    // bind textures
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

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
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model;
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle),
			  glm::vec3(1.0f, 0.3f, 0.5f));
      our_shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
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
