cmake_minimum_required(VERSION 3.11)

project(WebrogueGTKExample)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4>=4.14)

add_executable(main main.c) 
target_link_libraries(main PRIVATE PkgConfig::GTK)
