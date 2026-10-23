#include "wr4c.h"
#include "wr4c_internal.h"
#include <stdlib.h>
#include <string.h>

uint8_t wr4c_vulkan_check() { return webrogue_gfx_vulkan_check_presence(); }

void wr4c_vulkan_register_blob(wr4c_vulkan_renderer_t renderer, uint32_t res_id,
                               uint8_t *buf, uint32_t buf_len) {
  webrogue_gfx_list_u8_t buf_list = {.ptr = buf, .len = buf_len};
  webrogue_gfx_vulkan_method_renderer_register_blob(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), res_id, &buf_list);
}

uint32_t wr4c_vulkan_create_shmem(wr4c_vulkan_renderer_t renderer, void *ptr,
                                  uint32_t size) {
  webrogue_gfx_list_u8_t buf_list = {.ptr = (uint8_t *)ptr, .len = size};
  return webrogue_gfx_vulkan_method_renderer_create_shmem(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), &buf_list);
}

uint32_t wr4c_vulkan_create_device_memory_blob(wr4c_vulkan_renderer_t renderer,
                                               uint64_t blob_id,
                                               uint64_t size) {
  return webrogue_gfx_vulkan_method_renderer_create_device_memory_blob(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), blob_id, size);
}

void wr4c_vulkan_resource_unref(wr4c_vulkan_renderer_t renderer,
                                uint32_t res_id) {
  webrogue_gfx_vulkan_method_renderer_resource_unref(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), res_id);
}

uint32_t wr4c_vulkan_sync_create(wr4c_vulkan_renderer_t renderer,
                                 uint64_t value) {
  return webrogue_gfx_vulkan_method_renderer_create_sync(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), value);
}

void wr4c_vulkan_sync_unref(wr4c_vulkan_renderer_t renderer, uint32_t sync_id) {
  webrogue_gfx_vulkan_method_renderer_sync_unref(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), sync_id);
}

uint64_t wr4c_vulkan_sync_read(wr4c_vulkan_renderer_t renderer,
                               uint32_t sync_id) {
  return webrogue_gfx_vulkan_method_renderer_sync_read(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), sync_id);
}

void wr4c_vulkan_sync_write(wr4c_vulkan_renderer_t renderer, uint32_t sync_id,
                            uint64_t value) {
  webrogue_gfx_vulkan_method_renderer_sync_write(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), sync_id, value);
}

void wr4c_vulkan_submit_cmd(wr4c_vulkan_renderer_t renderer,
                            const uint8_t *headers, uint32_t headers_len,
                            const uint8_t *cmds, uint32_t cmds_len,
                            const uint8_t *syncs, uint32_t syncs_len) {
  webrogue_gfx_list_u8_t headers_list = {.ptr = (uint8_t *)headers, .len = headers_len};
  webrogue_gfx_list_u8_t cmds_list = {.ptr = (uint8_t *)cmds, .len = cmds_len};
  webrogue_gfx_list_u8_t syncs_list = {.ptr = (uint8_t *)syncs, .len = syncs_len};
  return webrogue_gfx_vulkan_method_renderer_submit_cmd(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), &headers_list,
      &cmds_list, &syncs_list);
}

uint32_t wr4c_vulkan_sync_wait(wr4c_vulkan_renderer_t renderer, uint32_t flags,
                               uint32_t timeout, const uint32_t *syncs,
                               uint32_t syncs_len) {
  webrogue_gfx_list_u32_t headers = {.ptr = (uint32_t *)syncs, .len = syncs_len};
  return webrogue_gfx_vulkan_method_renderer_sync_wait(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), flags, timeout,
      &headers);
}

uint32_t wr4c_vulkan_get_max_timeline_count(wr4c_vulkan_renderer_t renderer) {
  return webrogue_gfx_vulkan_method_renderer_get_max_timeline_count(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle));
}

uint32_t wr4c_vulkan_get_capset(wr4c_vulkan_renderer_t renderer, uint32_t id,
                                uint32_t version, uint8_t *capset,
                                uint32_t capset_size) {
  webrogue_gfx_list_u8_t ret;
  webrogue_gfx_vulkan_method_renderer_get_capset(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), id, version, &ret);
  uint32_t actual_size = ret.len;
  memcpy(capset, ret.ptr,
         actual_size < capset_size ? actual_size : capset_size);
  webrogue_gfx_list_u8_free(&ret);
  return actual_size;
}

void wr4c_vulkan_context_init(wr4c_vulkan_renderer_t renderer,
                              uint32_t capset_id) {
  webrogue_gfx_vulkan_method_renderer_context_init(
      webrogue_gfx_vulkan_borrow_renderer(renderer->handle), capset_id);
}

wr4c_vulkan_renderer_t wr4c_vulkan_create_renderer(const char *name,
                                                   uint32_t name_len) {
  webrogue_gfx_list_u8_t name_list = {.ptr = (uint8_t *)name, .len = name_len};
  wr4c_vulkan_renderer_t renderer = malloc(sizeof(wr4c_vulkan_renderer_s));
  renderer->handle = webrogue_gfx_vulkan_constructor_renderer(&name_list);
  return renderer;
}

void wr4c_vulkan_destroy_renderer(wr4c_vulkan_renderer_t renderer) {
  webrogue_gfx_vulkan_renderer_drop_own(renderer->handle);
  free(renderer);
}

uint32_t wr4c_get_vulkan_window_id(wr4c_window_t window) {
  webrogue_gfx_windowing_borrow_window_t borrow =
      webrogue_gfx_windowing_borrow_window(window->handle);

  return webrogue_gfx_windowing_method_window_get_vulkan_id(borrow);
}
