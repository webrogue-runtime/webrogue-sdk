// GENERATED BY webrogue-event-apigen. DO NOT EDIT MANUALLY
#include "webrogue_gfx.h"
#include <stdlib.h>

__attribute__((import_name("poll")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_poll(uint32_t* out_len);

__attribute__((import_name("poll_read")))
__attribute__((import_module("webrogue_gfx")))
void imported_webrogue_gfx_poll_read(void *buf);

#define BUF_SIZE(LEN) if(buffer_len < LEN) {\
    free(buffer_data);\
    result.type = WEBROGUE_EVENT_TYPE_INVALID;\
    return result;\
}
#define RETURN free(buffer_data); return result;
#define GET(TYPE, OFFSET) *((TYPE*)(((char*)buffer_data) + OFFSET));

webrogue_event webrogue_gfx_poll() {
    webrogue_event result;
    uint32_t buffer_len;
    imported_webrogue_gfx_poll(&buffer_len);
    void* buffer_data = malloc(buffer_len);
    BUF_SIZE(4);
    imported_webrogue_gfx_poll_read(buffer_data);
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
#undef BUF_SIZE
#undef RETURN
#undef GET
