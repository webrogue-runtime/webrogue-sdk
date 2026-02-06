#include "webroguegfx.h"
#include "webroguegfx_internal.h"
#include <stdint.h>
#include <stdlib.h>

__attribute__((import_name("present_pixels")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_present_pixels(wr_window_handle window, uint8_t *buf,
                                     uint32_t len, uint8_t *out_error);

void webroguegfx_present_pixels(wr_window window, uint8_t *buf, uint32_t len) {
  uint8_t error = 0;
  imported_webrogue_gfx_present_pixels(window->handle, buf, len, &error);
}
