
cmake_minimum_required(VERSION 3.5)
project(GeneticWalk)

#find_library(GLEW_LIBRARY glew32 HINTS ${CMAKE_SOURCE_DIR}/lib)
#find_library(GLFW_LIBRARY glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib)
#find_library(ASSIMP_LIBRARY glews32.lib PATHS ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
message(STATUS ${CMAKE_MODULE_PATH})

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")


#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic -g -O0 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if(WIN32)
  set(LIBS glfw3 opengl32 assimp GLU32 glew32)
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
  src/water-renderer.cc
  src/water.cc
  )


add_executable(GeneticWalk ${SOURCE_FILES})
include_directories(inc)
target_link_libraries(GeneticWalk ${ASSIMP_LIBRARY})
target_link_libraries(GeneticWalk ${GLFW_LIBRARY})
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
    target_link_libraries(GeneticWalk ${LIBS})
endif()
