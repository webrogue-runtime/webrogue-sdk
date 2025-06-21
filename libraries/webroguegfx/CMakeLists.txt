cmake_minimum_required(VERSION 3.31)

project(webroguegfx VERSION 0.0.1)

add_library(
    webroguegfx
    STATIC
    webroguegfx.c
    webroguegfx_events.c
)

set_target_properties(webroguegfx PROPERTIES PUBLIC_HEADER "webroguegfx.h")

set(
    MESA_LIBS
    ${MESA_PATH}/src/gfxstream/guest/vulkan/libvulkan_gfxstream.a
    ${MESA_PATH}/src/vulkan/wsi/libvulkan_wsi.a
)

add_custom_target(mesa_libs DEPENDS ${MESA_LIBS})
add_dependencies(webroguegfx mesa_libs)
add_custom_command(
    TARGET webroguegfx
    POST_BUILD
    COMMAND ${CMAKE_AR} ARGS 
        qLs
        $<TARGET_FILE:webroguegfx>
        ${MESA_LIBS}
)

install(
    TARGETS webroguegfx
    PUBLIC_HEADER DESTINATION include/webroguegfx
)
