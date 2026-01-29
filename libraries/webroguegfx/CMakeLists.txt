cmake_minimum_required(VERSION 3.31)

project(webroguegfx VERSION 0.0.1)

add_library(
    webroguegfx
    STATIC
    webroguegfx.c
    webroguegfx_events.c
    webroguegfx_vulkan.c
)

set_target_properties(
    webroguegfx
    PROPERTIES
    PUBLIC_HEADER "webroguegfx.h"
)

include(GNUInstallDirs)
install(
    TARGETS webroguegfx
    PUBLIC_HEADER
    DESTINATION include/webroguegfx
)
set(
    WEBROGUE_PC
    ${CMAKE_CURRENT_BINARY_DIR}/webroguegfx.pc
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/webroguegfx.pc.cmakein
    ${WEBROGUE_PC}
    @ONLY
)
install(
    FILES ${WEBROGUE_PC}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
