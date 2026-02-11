cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(webrogue_raylib_example C CXX)

set(PROJECT_TARGET ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CUSTOMIZE_BUILD ON)
set(SUPPORT_MODULE_RAUDIO OFF)
set(USE_AUDIO OFF)
set(SUPPORT_QUADS_DRAW_MODE ON)
set(OPENGL_VERSION "ES 3.0")
# -DOPENGL_INCLUDE_DIR=/opt/wasip1/include 
set(PLATFORM Desktop) # SDL Desktop
set(USE_EXTERNAL_GLFW ON)
#set(glfw3_DIR /opt/wasip1/lib/cmake/glfw3)
#set(SDL2_DIR /opt/wasip1/lib/cmake/SDL2)

add_subdirectory(raylib)

# file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")

add_executable(${PROJECT_TARGET} main.c)

target_link_libraries(${PROJECT_TARGET} raylib)
# target_link_libraries(${PROJECT_TARGET} glfw3 SDL2 vulkan_gfxstream)
# target_link_options(${PROJECT_TARGET} PRIVATE -v)
