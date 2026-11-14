cmake_minimum_required(VERSION 3.11)

project(WebGPU)

add_executable(main main.cpp)

target_link_libraries(
    main
    webgpu
)
