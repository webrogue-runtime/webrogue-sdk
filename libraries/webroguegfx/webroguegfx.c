#include "webroguegfx.h"
#include "webroguegfx_internal.h"
#include <stdint.h>
#include <stdlib.h>


__attribute__((import_name("make_window")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_make_window(wr_window_handle *out_window);

void webroguegfx_make_window(wr_window *out_window) {
    wr_window window = malloc(sizeof(wr_window_s));
    *out_window = window;
    imported_webrogue_gfx_make_window(&window->handle);
}

__attribute__((import_name("get_window_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_window_size(wr_window_handle window,
                                      uint32_t *out_width,
                                      uint32_t *out_height);

void webroguegfx_window_size(wr_window window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_window_size(window->handle, &out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("get_gl_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_gl_size(wr_window_handle window,
                                  uint32_t *out_width, 
                                  uint32_t *out_height);

void webroguegfx_gl_size(wr_window window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_gl_size(window->handle, &out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

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
