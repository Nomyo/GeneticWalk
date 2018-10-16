#include <main.hh>
#include <light.hh>
#include <skybox.hh>
#include <skybox-renderer.hh>
#include <water.hh>
#include <water-renderer.hh>

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
    Shader skybox_shader("shaders/skybox.vs", "shaders/skybox.fs");
    Shader water_shader("shaders/water.vs", "shaders/water.fs");

    std::vector<std::string> faces{ "textures/sky-left.jpg", "textures/sky-right.jpg",
        "textures/sky-top.jpg", "textures/sky-bottom.jpg", "textures/sky-front.jpg",
        "textures/sky-back.jpg" };
    auto skybox = Skybox(faces);
    skybox.init(skybox_shader);

    struct zone ez;
    ez.coord = glm::vec2(25.0f, 45.0f);
    ez.radius = 1.f;
    auto world = World("map/height_map.jpg", ez, glm::vec3(25.0f, 0.0f, 5.0f));
    auto light = Light(glm::vec3{25.0f , 40.0f, 25.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
    auto population = create_population(world);
    auto water = Water(glm::vec3{ 0.0f, -1.3f, 0.0f }, 50, 50);

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
        //updateFrame += GlobalConf::deltaTime;

        // input
        // -----
        global_conf.process_input(window);

        auto render_scene = [&](const glm::vec4 clip_plane, bool fb = false)
        {
            // render
            // ------
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(
                glm::radians(camera->get_zoom()),
                (float)GlobalConf::SCR_WIDTH /
                (float)GlobalConf::SCR_HEIGHT,
                0.1f, 500.0f);

            glm::mat4 view = camera->get_view_matrix();

            // render object
            // -------------
            WorldRenderer world_rd(world_shader, projection, view, light, clip_plane);
            world_rd.render(world);

            EntityRenderer er(our_shader, projection, view, light);
            er.render(population);

            glDisable(GL_CLIP_PLANE0);
            if (!fb) {
                WaterRenderer water_rd(water_shader, projection, view, light, camera->get_view_pos());
                water_rd.render(water);
            }

            // remove translation from the view matrix
            glDepthFunc(GL_LEQUAL);
            view = glm::mat4(glm::mat3(camera->get_view_matrix()));
            SkyboxRenderer skybox_rd(skybox_shader, projection, view);
            skybox_rd.render(skybox);
            glDepthFunc(GL_LESS);

            glEnable(GL_CLIP_PLANE0);
        };

        glEnable(GL_CLIP_PLANE0);

        // render reflection
        water.bind_reflection_fb();
        glm::vec3 old_camera_view_pos = camera->get_view_pos();
        float distance = 2 * (old_camera_view_pos.y - water.get_height());
        camera->inc_view_pos(glm::vec3(0, -distance, 0));
        camera->invert_pitch();
        render_scene(glm::vec4{ 0.0f, 1.0f, 0.0f, -water.get_height() - 1 }, true);
        camera->inc_view_pos(glm::vec3(0, distance, 0));
        camera->invert_pitch();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // render refraction
        water.bind_refraction_fb();
        render_scene(glm::vec4{ 0.0f, -1.0f, 0.0f, water.get_height() + 1 }, true);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_CLIP_PLANE0);

        // render main scene
        render_scene(glm::vec4{ 0.0f, -1.0f, 0.0f, 10000 });

        glfwSwapBuffers(window);
        glfwPollEvents();

        // update_population
        // -----------------
        //if (updateFrame >= 0.005f)
        //{
        for (auto& p : population) {
            p.update(world);
        }
        //updateFrame = 0.0f;
        //}

        if (std::all_of(population.begin(), population.end(), [](Character c)
        { return c.dead_or_done(); }))
        {
            std::cout << "GEN : " << nb_gen++ << std::endl; 
            population = create_next_generation(population, world);
        }

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
