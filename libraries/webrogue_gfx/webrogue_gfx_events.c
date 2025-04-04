// GENERATED BY webrogue-event-apigen. DO NOT EDIT MANUALLY
// clang-format off
#include "webrogue_gfx.h"
#include <stdlib.h>

#define WEBROGUE_MAX_ENCODED_EVENT_SIZE 16

__attribute__((import_name("poll")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_poll(uint32_t* out_len);

__attribute__((import_name("poll_read")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_poll_read(void *buf);

#define BUF_SIZE(LEN) if(available < LEN) {\
    buffer_consumed = buffer_used_size;\
    result.type = WEBROGUE_EVENT_TYPE_INVALID;\
    return result;\
} buffer_consumed += LEN;
#define RETURN return result;
#define GET(TYPE, OFFSET) *((TYPE*)(current_pointer + OFFSET));

webrogue_event webrogue_gfx_poll() {
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
        case WEBROGUE_EVENT_TYPE_MOUSE_DOWN: {
            BUF_SIZE(16);
            result.inner.mouse_down.x = GET(uint32_t, 4);
            result.inner.mouse_down.y = GET(uint32_t, 8);
            result.inner.mouse_down.button = GET(uint32_t, 12);
            RETURN;
        }
        case WEBROGUE_EVENT_TYPE_MOUSE_UP: {
            BUF_SIZE(16);
            result.inner.mouse_up.x = GET(uint32_t, 4);
            result.inner.mouse_up.y = GET(uint32_t, 8);
            result.inner.mouse_up.button = GET(uint32_t, 12);
            RETURN;
        }
        case WEBROGUE_EVENT_TYPE_MOUSE_MOTION: {
            BUF_SIZE(12);
            result.inner.mouse_motion.x = GET(uint32_t, 4);
            result.inner.mouse_motion.y = GET(uint32_t, 8);
            RETURN;
        }
        case WEBROGUE_EVENT_TYPE_QUIT: {
            BUF_SIZE(4);
            RETURN;
        }
        default: {
            result.type = WEBROGUE_EVENT_TYPE_INVALID;
            RETURN;
        }
    }
}
