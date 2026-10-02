#include "wr4c.h"
#include "wr4c_internal.h"
#include <stdint.h>
#include <stdlib.h>

void wr4c_present_pixels(wr4c_window_t window, uint8_t *buf, uint32_t len) {
  gfx_list_u32_t list = { .ptr = (uint32_t *)buf, .len = len / 4 };
  webrogue_gfx_windowing_borrow_window_t borrow = webrogue_gfx_windowing_borrow_window(window->handle);
  webrogue_gfx_cpu_rendering_present_pixels(borrow, &list);
  // webrogue_gfx_windowing_window_drop_borrow(borrow);
}
