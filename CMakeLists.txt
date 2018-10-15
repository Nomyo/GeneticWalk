
cmake_minimum_required(VERSION 3.5)
project(GeneticWalk)

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -g -O0 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

set(

  SOURCE_FILES

  src/main.cc
  src/glad.c

  src/camera.cc
  src/character.cc
  src/glob_conf.cc
  src/opengl-utils.cc
  src/shader_m.cc
  src/stb_image.cc
  src/mesh.cc
  src/model.cc
  src/world.cc
  src/world-renderer.cc
  src/entity.cc
  src/entity-renderer.cc
  src/ga_core.cc
  src/light-renderer.cc
  src/light.cc
  src/skybox-renderer.cc
  src/skybox.cc
  )


add_executable(GeneticWalk ${SOURCE_FILES})
include_directories(inc)

target_link_libraries(GeneticWalk glfw3)
if(NOT WIN32)
    target_link_libraries(GeneticWalk GL)
    target_link_libraries(GeneticWalk glut)
    target_link_libraries(GeneticWalk dl)
    target_link_libraries(GeneticWalk Xinerama)
    target_link_libraries(GeneticWalk Xrandr)
    target_link_libraries(GeneticWalk Xi)
    target_link_libraries(GeneticWalk Xcursor)
    target_link_libraries(GeneticWalk X11)
    target_link_libraries(GeneticWalk Xxf86vm)
    target_link_libraries(GeneticWalk pthread)
else()
    target_link_libraries(GeneticWalk glew32)
    target_link_libraries(GeneticWalk opengl32)
    target_link_libraries(GeneticWalk GLU32)
    target_link_libraries(GeneticWalk assimp)
endif()
