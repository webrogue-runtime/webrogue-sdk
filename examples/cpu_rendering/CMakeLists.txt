cmake_minimum_required(VERSION 3.11)

project(CPURenderingExample)

add_executable(main main.c)

find_library(WEBROGUEGFX_LIBRARY webroguegfx REQUIRED)

target_link_libraries(
    main
    webroguegfx
)
