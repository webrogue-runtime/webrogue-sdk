// WebGPU surfaces for wr4c windows.
//
// Surfaces are Webrogue specific: the guest creates them through
// `webrogue:gfx/windowing-webgpu`, which needs a `webrogue:gfx/windowing`
// window. Everything else in the WebGPU C API is implemented in webgpu_ext.c;
// this file only maps WGPUSurface* to that interface.

#include "wr4c_internal.h"
#include "wr4c_webgpu.h"

#include <stdlib.h>

// Object layouts from webgpu_ext.c (which includes the vendored
// wasi-webgpu-headers implementation). They are private there, but the
// surface glue needs to read the device handle and wrap surface textures.
typedef struct WGPUDeviceImpl {
    wasi_webgpu_webgpu_own_gpu_device_t device;
    uint32_t refCount;
} WGPUDeviceImpl;

typedef struct WGPUTextureImpl {
    wasi_webgpu_webgpu_own_gpu_texture_t texture;
    uint32_t refCount;
} WGPUTextureImpl;

struct WGPUSurfaceImpl {
    webrogue_gfx_windowing_webgpu_own_context_t context;
    uint32_t refCount;
};

WGPUSurface wr4c_create_webgpu_surface(wr4c_window_t window) {
    if (!window) abort();

    struct WGPUSurfaceImpl *surface = malloc(sizeof(*surface));
    if (!surface) abort();

    surface->context = webrogue_gfx_windowing_webgpu_constructor_context(
        webrogue_gfx_windowing_borrow_window(window->handle));
    surface->refCount = 1;
    return surface;
}

void wgpuSurfaceConfigure(WGPUSurface surface,
                          WGPUSurfaceConfiguration const *config) {
    if (!surface || !config || !config->device) abort();

    webrogue_gfx_windowing_webgpu_context_configuration_t wasi = {};
    wasi.device = wasi_webgpu_webgpu_borrow_gpu_device(config->device->device);
    wasi.format = (wasi_webgpu_webgpu_gpu_texture_format_t)(config->format - 1);
    wasi.usage.is_some = config->usage != WGPUTextureUsage_None;
    wasi.usage.val = (wasi_webgpu_webgpu_gpu_texture_usage_t)config->usage;
    wasi.view_formats.is_some = false;
    wasi.color_space.is_some = false;
    wasi.tone_mapping.is_some = false;

    switch (config->alphaMode) {
    case WGPUCompositeAlphaMode_Opaque:
        wasi.alpha_mode.is_some = true;
        wasi.alpha_mode.val = WASI_WEBGPU_WEBGPU_GPU_CANVAS_ALPHA_MODE_OPAQUE;
        break;
    case WGPUCompositeAlphaMode_Premultiplied:
        wasi.alpha_mode.is_some = true;
        wasi.alpha_mode.val =
            WASI_WEBGPU_WEBGPU_GPU_CANVAS_ALPHA_MODE_PREMULTIPLIED;
        break;
    default:
        break;
    }

    webrogue_gfx_windowing_webgpu_method_context_configure(
        webrogue_gfx_windowing_webgpu_borrow_context(surface->context), &wasi);
}

void wgpuSurfaceGetCurrentTexture(WGPUSurface surface,
                                  WGPUSurfaceTexture *surfaceTexture) {
    if (!surface || !surfaceTexture) abort();

    wasi_webgpu_webgpu_own_gpu_texture_t texture =
        webrogue_gfx_windowing_webgpu_method_context_get_current_texture(
            webrogue_gfx_windowing_webgpu_borrow_context(surface->context));

    WGPUTextureImpl *texture_impl = malloc(sizeof(*texture_impl));
    if (!texture_impl) abort();
    texture_impl->texture = texture;
    texture_impl->refCount = 1;

    surfaceTexture->texture = texture_impl;
    surfaceTexture->status = WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal;
}

WGPUStatus wgpuSurfacePresent(WGPUSurface surface) {
    if (!surface) return WGPUStatus_Error;
    webrogue_gfx_windowing_webgpu_method_context_present(
        webrogue_gfx_windowing_webgpu_borrow_context(surface->context));
    return WGPUStatus_Success;
}

WGPUStatus wgpuSurfaceGetCapabilities(WGPUSurface surface,
                                      WGPUAdapter adapter,
                                      WGPUSurfaceCapabilities *capabilities) {
    if (!surface || !capabilities) return WGPUStatus_Error;
    (void)adapter;

    // Every browser canvas accepts both 8-bit unorm formats; ANGLE's only
    // config uses BGRA8, so advertising it unconditionally is safe.
    static const WGPUTextureFormat formats[] = {
        WGPUTextureFormat_BGRA8Unorm,
        WGPUTextureFormat_RGBA8Unorm,
    };
    static const WGPUPresentMode presentModes[] = {
        WGPUPresentMode_Fifo,
    };
    static const WGPUCompositeAlphaMode alphaModes[] = {
        WGPUCompositeAlphaMode_Opaque,
        WGPUCompositeAlphaMode_Premultiplied,
    };

    capabilities->usages = WGPUTextureUsage_RenderAttachment |
                            WGPUTextureUsage_CopySrc |
                            WGPUTextureUsage_CopyDst |
                            WGPUTextureUsage_TextureBinding;
    capabilities->formatCount = sizeof(formats) / sizeof(formats[0]);
    capabilities->formats = formats;
    capabilities->presentModeCount =
        sizeof(presentModes) / sizeof(presentModes[0]);
    capabilities->presentModes = presentModes;
    capabilities->alphaModeCount = sizeof(alphaModes) / sizeof(alphaModes[0]);
    capabilities->alphaModes = alphaModes;
    return WGPUStatus_Success;
}

void wgpuSurfaceUnconfigure(WGPUSurface surface) {
    if (!surface) return;
    webrogue_gfx_windowing_webgpu_method_context_unconfigure(
        webrogue_gfx_windowing_webgpu_borrow_context(surface->context));
}

void wgpuSurfaceAddRef(WGPUSurface surface) {
    if (!surface) abort();
    surface->refCount++;
}

void wgpuSurfaceRelease(WGPUSurface surface) {
    if (!surface) return;
    if (surface->refCount == 0) abort();
    surface->refCount--;
    if (surface->refCount == 0) {
        webrogue_gfx_windowing_webgpu_context_drop_own(surface->context);
        free(surface);
    }
}
