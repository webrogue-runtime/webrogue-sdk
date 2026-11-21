#ifndef WR4C_WEBGPU_H_
#define WR4C_WEBGPU_H_

#include <webgpu/webgpu.h>

#include "wr4c.h"

#ifdef __cplusplus
extern "C" {
#endif

WGPUSurface wr4c_create_webgpu_surface(wr4c_window_t window);

WGPUTextureFormat wr4c_webgpu_preferred_format(WGPUInstance instance);

struct DawnProcTable;
const struct DawnProcTable *wr4c_get_dawn_proc_table(void);

#ifdef __cplusplus
}
#endif

#endif // WR4C_WEBGPU_H_
