#include "webroguegfx.h"
#include "webroguegfx_internal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

__attribute__((import_name("make_window")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_make_window(wr_window_handle *out_window);

void webroguegfx_make_window(wr_window *out_window) {
  wr_window window = malloc(sizeof(wr_window_s));
  *out_window = window;
  imported_webrogue_gfx_make_window(&window->handle);
}

__attribute__((import_name("destroy_window")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_destroy_window(wr_window_handle window);

void webroguegfx_destroy_window(wr_window window) {
  imported_webrogue_gfx_destroy_window(window->handle);
  free(window);
}

__attribute__((import_name("get_window_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_window_size(wr_window_handle window,
                                      uint32_t *out_width,
                                      uint32_t *out_height);

void webroguegfx_window_size(wr_window window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_window_size(window->handle, &out_width,
                                        &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("get_gl_size")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_get_gl_size(wr_window_handle window, uint32_t *out_width,
                                  uint32_t *out_height);

void webroguegfx_gl_size(wr_window window, int *width, int *height) {
  uint32_t out_width, out_height; // TODO pass width and height directly
  imported_webrogue_gfx_get_gl_size(window->handle, &out_width, &out_height);
  *width = out_width;
  *height = out_height;
}

__attribute__((import_name("poll")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_poll(uint32_t *out_len);

__attribute__((import_name("poll_read")))
__attribute__((import_module("webrogue_gfx"))) void
imported_webrogue_gfx_poll_read(void *buf);

#define BUF_SIZE(LEN)                                                          \
  if (available < LEN) {                                                       \
    buffer_consumed = buffer_used_size;                                        \
    result.type = WEBROGUE_EVENT_TYPE_INVALID;                                 \
    return result;                                                             \
  }                                                                            \
  buffer_consumed += LEN;
#define GET(TYPE, OFFSET) *((TYPE *)(current_pointer + OFFSET));

// clang-format off
// BEGIN GENERATED CODE
#define WEBROGUE_MAX_ENCODED_EVENT_SIZE 44

webrogue_event webroguegfx_poll() {
    webrogue_event result;
    static void* buffer_data = NULL;
    if(!buffer_data) {
        buffer_data = malloc(WEBROGUE_MAX_ENCODED_EVENT_SIZE);
    }
    static uint32_t buffer_max_size = WEBROGUE_MAX_ENCODED_EVENT_SIZE;
    static uint32_t buffer_used_size = 0;
    static uint32_t buffer_consumed = 0;
    uint32_t available = buffer_used_size - buffer_consumed;
    if(available == 0) {
        uint32_t new_size;
        imported_webrogue_gfx_poll(&new_size);
        if(new_size > buffer_max_size) {
            free(buffer_data);
            buffer_data = malloc(new_size);
            buffer_max_size = new_size;
        }
        if(new_size) {
            imported_webrogue_gfx_poll_read(buffer_data);
        }
        buffer_used_size = new_size;
        buffer_consumed = 0;
        available = new_size;
    }
    if(available < 4) {
        buffer_consumed = buffer_used_size;
        result.type = WEBROGUE_EVENT_TYPE_INVALID;
        return result;
    }
    const char* current_pointer = ((const char*)buffer_data) + buffer_consumed;
    result.type = GET(uint32_t, 0);
    switch (result.type) {
        case WEBROGUE_EVENT_TYPE_MOUSE_BUTTON: {
            BUF_SIZE(16);
            result.inner.mouse_button.button = GET(uint8_t, 12);
            result.inner.mouse_button.down = GET(uint8_t, 13);
            result.inner.mouse_button.x = GET(uint32_t, 4);
            result.inner.mouse_button.y = GET(uint32_t, 8);
            result.inner.mouse_button.is_touch = GET(uint8_t, 14);
            return result;
        }
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION: {
            BUF_SIZE(12);
            result.inner.mouse_motion.x = GET(uint32_t, 4);
            result.inner.mouse_motion.y = GET(uint32_t, 8);
            return result;
        }
        case WEBROGUE_EVENT_TYPE_KEY: {
            BUF_SIZE(44);
            result.inner.key.down = GET(uint8_t, 40);
            result.inner.key.named_key = GET(uint16_t, 36);
            result.inner.key.physical_key = GET(uint16_t, 38);
            result.inner.key.text_length = GET(uint8_t, 41);
            memcpy(&result.inner.key.text, current_pointer + 4, 32);
            return result;
        }
        case WEBROGUE_EVENT_TYPE_QUIT: {
            BUF_SIZE(4);
            return result;
        }
        case WEBROGUE_EVENT_TYPE_WINDOW_RESIZED: {
            BUF_SIZE(4);
            return result;
        }
        case WEBROGUE_EVENT_TYPE_GL_RESIZED: {
            BUF_SIZE(4);
            return result;
        }
        default: {
            buffer_consumed = buffer_used_size;
            result.type = WEBROGUE_EVENT_TYPE_INVALID;
            return result;
        }
    }
}
// END GENERATED CODE
// clang-format on
