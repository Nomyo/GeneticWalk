#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <string>

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

/**
 * Choose a random character texture
 */
std::string choose_texture();

/**
 * Initialisation of the screen
 */
GLFWwindow *window_init();

/**
 * Create population of GA algorithm
 */
std::vector<Character> create_population(const World& w);
