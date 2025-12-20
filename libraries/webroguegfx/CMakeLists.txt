cmake_minimum_required(VERSION 3.31)

project(webroguegfx VERSION 0.0.1)

add_library(
    webroguegfx
    STATIC
    webroguegfx.c
    webroguegfx_events.c
    webroguegfx_vulkan.c
)

set_target_properties(webroguegfx PROPERTIES PUBLIC_HEADER "webroguegfx.h")

install(
    TARGETS webroguegfx
    PUBLIC_HEADER DESTINATION include/webroguegfx
)
