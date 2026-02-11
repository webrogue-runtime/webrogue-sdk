cmake_minimum_required(VERSION 3.11)

project(WebrogueGLFWExample)

add_executable(main main.c)

# find_package(PkgConfig REQUIRED)
# list(APPEND PKG_CONFIG_EXECUTABLE "--static")
# pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)

find_library(WEBROGUEGFX_LIBRARY webroguegfx REQUIRED)
find_library(EGL_LIBRARY EGL REQUIRED)
find_library(VULKAN_GFXSTREAM_LIBRARY vulkan_gfxstream REQUIRED)

target_include_directories(
    main
    PRIVATE
    ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(
    main
    # PkgConfig::GLFW
    glfw3
    c++
    c++abi
    webroguegfx
    -Wl,--export=vkGetInstanceProcAddr vulkan vulkan_gfxstream
    -Wl,--export=eglGetProcAddress EGL
)
