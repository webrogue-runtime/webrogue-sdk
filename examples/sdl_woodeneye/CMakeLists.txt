cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(SDLWoodeneye)

find_package(SDL3 REQUIRED)

add_executable(woodeneye woodeneye.c)

target_link_libraries(woodeneye SDL3 wr4c)
