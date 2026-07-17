#include "webroguegfx.h"
#include "webroguegfx_internal.h"

// __attribute__((import_name("make_vk_surface")))
// __attribute__((import_module("webrogue_gfx"))) static void
// imported_webrogue_gfx_make_vk_surface(wr_window_handle window,
//                                       uint64_t vk_instance,
//                                       uint64_t *out_vk_surface);

// uint64_t webroguegfx_vulkan_make_surface(wr_window window,
//                                          uint64_t vk_instance) {
//   uint64_t out_vk_surface = 0;
//   imported_webrogue_gfx_make_vk_surface(window->handle, vk_instance,
//                                         &out_vk_surface);
//   return out_vk_surface;
// }

__attribute__((import_name("check_vk")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_check_vk(uint8_t *out);

uint8_t webroguegfx_vulkan_check() {
  uint8_t result = 0;
  imported_webrogue_gfx_check_vk(&result);
  return result;
}

__attribute__((import_name("vulkan_register_blob")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_register_blob(uint32_t res_id, uint8_t *buf, uint32_t buf_len);

void webroguegfx_vulkan_register_blob(uint32_t res_id, uint8_t *buf, uint32_t buf_len) {
  imported_webrogue_gfx_vulkan_register_blob(res_id, buf, buf_len);
}

__attribute__((import_name("vulkan_create_blob")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_create_blob(void *ptr, uint32_t size, uint64_t blob_id, uint32_t *out_res_id);

uint32_t webroguegfx_vulkan_create_blob(void *ptr, uint32_t size, uint64_t blob_id) {
  uint32_t res_id = 0;
  imported_webrogue_gfx_vulkan_create_blob(ptr, size, blob_id, &res_id);
  return res_id;
}

__attribute__((import_name("vulkan_resource_unref")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_resource_unref(uint32_t res_id);

void webroguegfx_vulkan_resource_unref(uint32_t res_id) {
  imported_webrogue_gfx_vulkan_resource_unref(res_id);
}

__attribute__((import_name("vulkan_sync_create")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_sync_create(uint64_t value,
                                                     uint32_t *out_sync_id);

uint32_t webroguegfx_vulkan_sync_create(uint64_t value) {
  uint32_t sync_id = 0;
  imported_webrogue_gfx_vulkan_sync_create(value, &sync_id);
  return sync_id;
}

__attribute__((import_name("vulkan_sync_unref")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_sync_unref(uint32_t sync_id);

void webroguegfx_vulkan_sync_unref(uint32_t sync_id) {
  imported_webrogue_gfx_vulkan_sync_unref(sync_id);
}

__attribute__((import_name("vulkan_sync_read")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_sync_read(uint32_t sync_id,
                                                   uint64_t *out_value);

uint64_t webroguegfx_vulkan_sync_read(uint32_t sync_id) {
  uint64_t value = 0;
  imported_webrogue_gfx_vulkan_sync_read(sync_id, &value);
  return value;
}

__attribute__((import_name("vulkan_sync_write")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_sync_write(uint32_t sync_id,
                                                    uint64_t value);

void webroguegfx_vulkan_sync_write(uint32_t sync_id, uint64_t value) {
  imported_webrogue_gfx_vulkan_sync_write(sync_id, value);
}

__attribute__((import_name("vulkan_submit_cmd")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_submit_cmd(const uint8_t *headers,
                                                    uint32_t headers_len,
                                                    const uint8_t *cmds,
                                                    uint32_t cmds_len,
                                                    const uint8_t *syncs,
                                                    uint32_t syncs_len);

void webroguegfx_vulkan_submit_cmd(const uint8_t *headers, uint32_t headers_len,
                                   const uint8_t *cmds, uint32_t cmds_len,
                                   const uint8_t *syncs, uint32_t syncs_len) {
  imported_webrogue_gfx_vulkan_submit_cmd(headers, headers_len, cmds, cmds_len,
                                          syncs, syncs_len);
}

__attribute__((import_name("vulkan_sync_wait")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_sync_wait(uint32_t flags,
                                                   uint32_t timeout,
                                                   const uint8_t *syncs,
                                                   uint32_t syncs_len,
                                                   uint32_t *out_result);

uint32_t webroguegfx_vulkan_sync_wait(uint32_t flags, uint32_t timeout,
                                      const uint8_t *syncs, uint32_t syncs_len) {
  uint32_t result = 0;
  imported_webrogue_gfx_vulkan_sync_wait(flags, timeout, syncs, syncs_len,
                                         &result);
  return result;
}

__attribute__((import_name("vulkan_get_max_timeline_count")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_get_max_timeline_count(uint32_t *out_value);

uint32_t webroguegfx_vulkan_get_max_timeline_count() {
  uint32_t value = 0;
  imported_webrogue_gfx_vulkan_get_max_timeline_count(&value);
  return value;
}

__attribute__((import_name("vulkan_get_capset")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_get_capset(uint32_t id,
                                                    uint32_t version,
                                                    uint8_t *capset,
                                                    uint32_t capset_size,
                                                    uint32_t *out_size);

uint32_t webroguegfx_vulkan_get_capset(uint32_t id, uint32_t version,
                                       uint8_t *capset, uint32_t capset_size) {
  uint32_t size = 0;
  imported_webrogue_gfx_vulkan_get_capset(id, version, capset, capset_size,
                                          &size);
  return size;
}

__attribute__((import_name("vulkan_context_init")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_context_init(uint32_t capset_id);

void webroguegfx_vulkan_context_init(uint32_t capset_id) {
  imported_webrogue_gfx_vulkan_context_init(capset_id);
}

__attribute__((import_name("vulkan_create_renderer")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vulkan_create_renderer(const uint8_t *name,
                                                         uint32_t name_len);

void webroguegfx_vulkan_create_renderer(const char *name, uint32_t name_len) {
  imported_webrogue_gfx_vulkan_create_renderer((const uint8_t *)name, name_len);
}

uint32_t webroguegfx_get_vulkan_window_id(wr_window window) {
  return window->handle;
}
