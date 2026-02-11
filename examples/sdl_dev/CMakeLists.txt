cmake_minimum_required(VERSION 3.22.0 FATAL_ERROR)

project(CppSDL)

set(USE_SDL_VERSION 3)
set(USE_SUBDIRECTORIES OFF)

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
target_compile_definitions(sdl_dev PRIVATE -DUSE_SDL_VERSION=${USE_SDL_VERSION})
if(USE_SUBDIRECTORIES)
    set(SDL_LIB_NAME SDL${USE_SDL_VERSION}-static)
else()
    set(SDL_LIB_NAME SDL${USE_SDL_VERSION})
endif()


target_link_libraries(sdl_dev ${SDL_LIB_NAME} SDL${USE_SDL_VERSION}_ttf freetype png zlib webroguegfx)

target_link_libraries(sdl_dev ${SDL_LIB_NAME} vulkan_gfxstream c++ c++abi)
target_link_options(sdl_dev PRIVATE -Wl,--export=vk_icdGetInstanceProcAddr)

target_link_libraries(sdl_dev ${SDL_LIB_NAME} EGL vulkan)
target_link_options(sdl_dev PRIVATE -Wl,--export=eglGetProcAddress)

if(USE_SDL_VERSION EQUAL 2)
    target_link_libraries(sdl_dev samplerate)
endif()
