cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(CppSDL)

set(USE_SDL_VERSION 3)
set(USE_SUBDIRECTORIES OFF)
set(SDL_DEV_RENDERER "VULKAN" CACHE STRING
    "SDL renderer to use (SOFTWARE, OPENGL, or VULKAN)")
set_property(CACHE SDL_DEV_RENDERER PROPERTY STRINGS SOFTWARE OPENGL VULKAN)

if(SDL_DEV_RENDERER STREQUAL "SOFTWARE")
    set(SDL_DEV_WINDOW_FLAGS 0)
    set(SDL_DEV_RENDER_DRIVER "software")
elseif(SDL_DEV_RENDERER STREQUAL "OPENGL")
    set(SDL_DEV_WINDOW_FLAGS SDL_WINDOW_OPENGL)
    set(SDL_DEV_RENDER_DRIVER "opengles2")
elseif(SDL_DEV_RENDERER STREQUAL "VULKAN")
    set(SDL_DEV_WINDOW_FLAGS SDL_WINDOW_VULKAN)
    set(SDL_DEV_RENDER_DRIVER "vulkan")
else()
    message(FATAL_ERROR "SDL_DEV_RENDERER must be SOFTWARE, OPENGL, or VULKAN")
endif()

if(USE_SUBDIRECTORIES)
    # add_subdirectory(/webrogue_sdk_libs/webroguegfx webroguegfx)
    set(BUILD_SHARED_LIBS OFF)
    if(USE_SDL_VERSION EQUAL 2)
        add_subdirectory(/webrogue_sdk_libs/libsamplerate libsamplerate)
    endif()

    set(SDL_STATIC ON)
    set(SDL_SHARED OFF)
    add_subdirectory(/webrogue_sdk_libs/SDL${USE_SDL_VERSION} SDL${USE_SDL_VERSION})

    set(CMAKE_POSITION_INDEPENDENT_CODE OFF)
    add_subdirectory(/webrogue_sdk_libs/SDL${USE_SDL_VERSION}_ttf SDL${USE_SDL_VERSION}_ttf)
else()
    find_package(SDL${USE_SDL_VERSION} REQUIRED)
    find_package(SDL${USE_SDL_VERSION}_ttf REQUIRED)
endif()

add_executable(sdl_dev sdl_dev.c)
target_compile_definitions(sdl_dev PRIVATE
    -DUSE_SDL_VERSION=${USE_SDL_VERSION}
    -DSDL_DEV_WINDOW_FLAGS=${SDL_DEV_WINDOW_FLAGS}
    "SDL_DEV_RENDER_DRIVER=\"${SDL_DEV_RENDER_DRIVER}\"")
if(USE_SUBDIRECTORIES)
    set(SDL_LIB_NAME SDL${USE_SDL_VERSION}-static)
else()
    set(SDL_LIB_NAME SDL${USE_SDL_VERSION})
endif()


target_link_libraries(sdl_dev ${SDL_LIB_NAME} SDL${USE_SDL_VERSION}_ttf freetype png zlib wr4c)

if(SDL_DEV_RENDERER STREQUAL "OPENGL")
    # WebRogue's OpenGL ES implementation (ANGLE) uses the Vulkan guest driver.
    target_link_libraries(sdl_dev EGL vulkan c++ c++abi vulkan_virtio)
    target_link_options(sdl_dev PRIVATE
        -Wl,--export=eglGetProcAddress
        -Wl,--export=vk_icdGetInstanceProcAddr)
elseif(SDL_DEV_RENDERER STREQUAL "VULKAN")
    target_link_libraries(sdl_dev vulkan_virtio)
    target_link_options(sdl_dev PRIVATE -Wl,--export=vk_icdGetInstanceProcAddr)
endif()

if(USE_SDL_VERSION EQUAL 2)
    target_link_libraries(sdl_dev samplerate)
endif()
