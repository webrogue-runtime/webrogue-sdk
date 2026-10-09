#include "gfx.h"
#include <stdbool.h>

struct wr4c_window_s {
  webrogue_gfx_windowing_own_window_t handle;
  size_t internal_id;
  webrogue_gfx_windowing_stream_window_event_t window_event_stream;
  bool read_in_flight;
  webrogue_gfx_windowing_window_event_t polled_event;
};

struct wr4c_vulkan_renderer_s {
  webrogue_gfx_vulkan_own_renderer_t handle;
};
