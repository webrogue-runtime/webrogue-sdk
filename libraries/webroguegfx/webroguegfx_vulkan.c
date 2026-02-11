#include "webroguegfx.h"
#include "webroguegfx_internal.h"

__attribute__((import_name("make_vk_surface")))
__attribute__((import_module("webrogue_gfx"))) static void
imported_webrogue_gfx_make_vk_surface(wr_window_handle window,
                                      uint64_t vk_instance,
                                      uint64_t *out_vk_surface);

uint64_t webroguegfx_vulkan_make_surface(wr_window window,
                                         uint64_t vk_instance) {
  uint64_t out_vk_surface = 0;
  imported_webrogue_gfx_make_vk_surface(window->handle, vk_instance,
                                        &out_vk_surface);
  return out_vk_surface;
}

__attribute__((import_name("commit_buffer")))
__attribute__((import_module("webrogue_gfx"))) static void
imported_webrogue_gfx_commit_buffer(const void *buf, uint32_t len);

void webroguegfx_vulkan_commit_buffer(const void *buf, size_t len) {
  imported_webrogue_gfx_commit_buffer(buf, len);
}

__attribute__((import_name("ret_buffer_read")))
__attribute__((import_module("webrogue_gfx"))) static void
imported_webrogue_gfx_ret_buffer_read(const void *buf, uint32_t len);

void webroguegfx_vulkan_ret_buffer_read(const void *buf, size_t len) {
  imported_webrogue_gfx_ret_buffer_read(buf, len);
}

__attribute__((import_name("vk_register_blob")))
__attribute__((import_module("webrogue_gfx"))) static void
imported_webrogue_gfx_vk_register_blob(uint64_t blobId, uint64_t size,
                                       void *buf);

void webroguegfx_vulkan_register_blob(uint64_t blobId, uint64_t size,
                                      void *buf) {
  imported_webrogue_gfx_vk_register_blob(blobId, size, buf);
}

__attribute__((import_name("check_vk"))) __attribute__((import_module(
    "webrogue_gfx"))) void static imported_webrogue_gfx_check_vk(uint8_t *out);

uint8_t webroguegfx_vulkan_check() {
  uint8_t result = 0;
  imported_webrogue_gfx_check_vk(&result);
  return result;
}
