#include "webrogue_gfx.h"
#include <stdint.h>

__attribute__((import_name("present")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_present();

void webrogue_gfx_present() { imported_webrogue_gfx_present(); }

__attribute__((import_name("make_window")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_make_window();

void webrogue_gfx_make_window() { imported_webrogue_gfx_make_window(); }

__attribute__((import_name("get_window_size")))
__attribute__((import_module("webrogue_gfx"))) 
void imported_webrogue_gfx_get_window_size(uint32_t* out_width, uint32_t* out_height);

void webrogue_gfx_window_size(int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_window_size(&out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("get_gl_size")))
__attribute__((import_module("webrogue_gfx"))) 
void imported_webrogue_gfx_get_gl_size(uint32_t* out_width, uint32_t* out_height);

void webrogue_gfx_gl_size(int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_gl_size(&out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("gl_init")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_init_gl();

void webrogue_gfx_init_gl() {
  imported_webrogue_gfx_init_gl();
}
