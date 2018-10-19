#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <string>
#include <random>

#include <glob_conf.hh>
#include <entity.hh>
#include <world.hh>
#include <character.hh>
#include <ga_core.hh>

/**
 * Whenever the window size changes this funtion is called
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/**
 * Generate a texture and return the texture_id
 */
unsigned int gen_texture(const std::string& path);

unsigned int loadCubemap(const std::vector<std::string>& faces);

/**
 *
 */
void init_character_models();

/**
 * Choose a random character texture
 */
Model *choose_model();

/**
 * Initialisation of the screen
 */
GLFWwindow *window_init();

/**
 * Create population of GA algorithm
 */
std::vector<Character> create_population(const World& w);

/**
* Barry centric interpolation between 3 points
*/
float barry_centric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
