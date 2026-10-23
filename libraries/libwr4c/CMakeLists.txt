cmake_minimum_required(VERSION 3.31)

project(libwr4c VERSION 0.0.1)

add_library(
    wr4c
    STATIC
    wr4c.h
    wr4c.c
    wr4c_vulkan.c
    wr4c_cpu_rendering.c
    webrogue_gfx.h
    webrogue_gfx.c
    webrogue_gfx_component_type.o
)

set_target_properties(
    wr4c
    PROPERTIES
    PUBLIC_HEADER "wr4c.h"
)

include(GNUInstallDirs)
install(
    TARGETS wr4c
    PUBLIC_HEADER
    DESTINATION include/libwr4c
)
set(
    wr4c_PC
    ${CMAKE_CURRENT_BINARY_DIR}/libwr4c.pc
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libwr4c.pc.cmakein
    ${wr4c_PC}
    @ONLY
)
install(
    FILES ${wr4c_PC}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
