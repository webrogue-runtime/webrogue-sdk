cmake_minimum_required(VERSION 3.11)

project(CPURenderingExample)

add_executable(main main.c)

find_library(WR4C_LIBRARY wr4c REQUIRED)

target_link_libraries(
    main
    wr4c
)
