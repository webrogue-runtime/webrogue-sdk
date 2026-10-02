#include "wr4c.h"
#include "gfx.h"
#include "wr4c_internal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t wr4c_vulkan_check() {
  return webrogue_gfx_vulkan_check_presence();
}


static webrogue_gfx_vulkan_own_linear_memory_marker_t linear_memory;
static bool linear_memory_initialized = false;

void init_linear_memory() {
  if(linear_memory_initialized) {
    return;
  }
  gfx_list_u8_t data;
  data.len = 1024;
  data.ptr = malloc(data.len);
  for (size_t i = 0; i < data.len; i++) {
    data.ptr[i] = (uint8_t)rand(); 
  }
  if(!webrogue_gfx_vulkan_static_linear_memory_marker_create((uint32_t)data.ptr, &data, &linear_memory)) {
    fprintf(stderr, "webrogue_gfx_vulkan_static_linear_memory_marker_create failed\n");
    abort();
  }

  linear_memory_initialized = true;
}

void wr4c_vulkan_register_blob(uint32_t res_id, uint8_t *buf, uint32_t buf_len) {
  init_linear_memory();
  webrogue_gfx_vulkan_borrow_linear_memory_marker_t borrow_linear_memory = webrogue_gfx_vulkan_borrow_linear_memory_marker(linear_memory);
  webrogue_gfx_vulkan_register_blob(res_id, (uint32_t) buf, buf_len, borrow_linear_memory);
}

uint32_t wr4c_vulkan_create_blob(void *ptr, uint32_t size, uint64_t blob_id) {
  init_linear_memory();
  webrogue_gfx_vulkan_borrow_linear_memory_marker_t borrow_linear_memory = webrogue_gfx_vulkan_borrow_linear_memory_marker(linear_memory);
  return webrogue_gfx_vulkan_create_blob(blob_id, (uint32_t) ptr, size, borrow_linear_memory);
}

void wr4c_vulkan_resource_unref(uint32_t res_id) {
  webrogue_gfx_vulkan_resource_unref(res_id);
}

uint32_t wr4c_vulkan_sync_create(uint64_t value) {
  return webrogue_gfx_vulkan_create_sync(value);
}

void wr4c_vulkan_sync_unref(uint32_t sync_id) {
  webrogue_gfx_vulkan_sync_unref(sync_id);
}


uint64_t wr4c_vulkan_sync_read(uint32_t sync_id) {
  return webrogue_gfx_vulkan_sync_read(sync_id);
}

void wr4c_vulkan_sync_write(uint32_t sync_id, uint64_t value) {
  webrogue_gfx_vulkan_sync_write(sync_id, value);
}

void wr4c_vulkan_submit_cmd(const uint8_t *headers, uint32_t headers_len,
                                   const uint8_t *cmds, uint32_t cmds_len,
                                   const uint8_t *syncs, uint32_t syncs_len) {
  gfx_list_u8_t headers_list = { .ptr=(uint8_t *)headers, .len=headers_len };
  gfx_list_u8_t cmds_list = { .ptr=(uint8_t *)cmds, .len=cmds_len };
  gfx_list_u8_t syncs_list = { .ptr=(uint8_t *)syncs, .len=syncs_len };
  return webrogue_gfx_vulkan_submit_cmd(&headers_list, &cmds_list, &syncs_list);
}

uint32_t wr4c_vulkan_sync_wait(uint32_t flags, uint32_t timeout,
                                      const uint32_t *syncs, uint32_t syncs_len) {
  gfx_list_u32_t headers = { .ptr=(uint32_t *)syncs, .len=syncs_len };
  return webrogue_gfx_vulkan_sync_wait(flags, timeout, &headers);
}

uint32_t wr4c_vulkan_get_max_timeline_count() {
  return webrogue_gfx_vulkan_get_max_timeline_count();
}

uint32_t wr4c_vulkan_get_capset(uint32_t id, uint32_t version,
                                       uint8_t *capset, uint32_t capset_size) {
  gfx_list_u8_t ret;
  webrogue_gfx_vulkan_get_capset(id, version, &ret);
  uint32_t actual_size = ret.len;
  memcpy(capset, ret.ptr, actual_size < capset_size ? actual_size : capset_size);
  gfx_list_u8_free(&ret);
  return actual_size;
}

void wr4c_vulkan_context_init(uint32_t capset_id) {
  webrogue_gfx_vulkan_context_init(capset_id);
}

void wr4c_vulkan_create_renderer(const char *name, uint32_t name_len) {
  gfx_list_u8_t name_list = { .ptr = (uint8_t *)name, .len = name_len };
  webrogue_gfx_vulkan_create_renderer(&name_list);
}

uint32_t wr4c_get_vulkan_window_id(wr4c_window_t window) {
  webrogue_gfx_windowing_borrow_window_t borrow = webrogue_gfx_windowing_borrow_window(window->handle);

  return webrogue_gfx_windowing_method_window_get_vulkan_id(borrow);
}
