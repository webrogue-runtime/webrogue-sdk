#include "webroguegfx.h"
#include "webroguegfx_internal.h"

__attribute__((import_name("make_vk_surface")))
__attribute__((import_module("webrogue_gfx"))) 
void imported_webrogue_gfx_make_vk_surface(wr_window_handle window,
                                           uint64_t vk_instance, 
                                           uint64_t *out_vk_surface);

uint64_t webroguegfx_vulkan_make_surface(wr_window window, uint64_t vk_instance) {
    uint64_t out_vk_surface = 0;
    imported_webrogue_gfx_make_vk_surface(window->handle, vk_instance, &out_vk_surface);
    return out_vk_surface;
}
