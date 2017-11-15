cmake_minimum_required(VERSION 3.5)
project(GeneticWalk)

#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -g -O0 -Wall -Wextra -Iinc")

set(

  SOURCE_FILES

  src/main.cc
  src/glad.c

  src/camera.cc
  src/glob_conf.cc
  src/opengl-utils.cc
  src/shader_m.cc
  src/stb_image.cc
  src/mesh.cc
  src/model.cc
  )


add_executable(GeneticWalk ${SOURCE_FILES})
target_link_libraries(GeneticWalk GLEW)
target_link_libraries(GeneticWalk glfw3)
target_link_libraries(GeneticWalk GL)
target_link_libraries(GeneticWalk m)
target_link_libraries(GeneticWalk dl)
target_link_libraries(GeneticWalk Xinerama)
target_link_libraries(GeneticWalk Xrandr)
target_link_libraries(GeneticWalk Xi)
target_link_libraries(GeneticWalk Xcursor)
target_link_libraries(GeneticWalk X11)
target_link_libraries(GeneticWalk Xxf86vm)
target_link_libraries(GeneticWalk pthread)
target_link_libraries(GeneticWalk assimp)
