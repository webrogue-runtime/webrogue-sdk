#include "webroguegfx.h"
#include "webroguegfx_internal.h"
#include <stdint.h>


__attribute__((import_name("make_window")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_make_window(wr_window_handle *out_window);

void webroguegfx_make_window(wr_window_handle *out_window) {
  imported_webrogue_gfx_make_window(out_window);
}

__attribute__((import_name("get_window_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_window_size(wr_window_handle window,
                                      uint32_t *out_width,
                                      uint32_t *out_height);

void webroguegfx_window_size(wr_window_handle window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_window_size(window, &out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("get_gl_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_gl_size(wr_window_handle window,
                                  uint32_t *out_width, 
                                  uint32_t *out_height);

void webroguegfx_gl_size(wr_window_handle window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_gl_size(window, &out_width, &out_height);
  *width = out_width;
  *height = out_height;
}
