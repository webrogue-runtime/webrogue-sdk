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

__attribute__((import_name("vtest_write")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vtest_write(const uint8_t *data, uint32_t data_len);

void webroguegfx_vtest_write(const uint8_t *data, uint32_t data_len) {
  imported_webrogue_gfx_vtest_write(data, data_len);
}

__attribute__((import_name("vtest_read")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vtest_read(uint8_t *data, uint32_t data_len);

void webroguegfx_vtest_read(uint8_t *data, uint32_t data_len) {
  imported_webrogue_gfx_vtest_read(data, data_len);
}


// __attribute__((import_name("vtest_register_blob")))
// __attribute__((import_module("webrogue_gfx")))
// void static imported_webrogue_gfx_vtest_register_blob(uint64_t blob_id, uint8_t *buf, uint32_t buf_len);

// void webroguegfx_vtest_register_blob(uint64_t blob_id, uint8_t *buf, uint32_t buf_len) {
//   imported_webrogue_gfx_vtest_register_blob(blob_id, buf, buf_len);
// }

__attribute__((import_name("vtest_receive_fd")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vtest_receive_fd(uint32_t *out_fd);

uint32_t webroguegfx_vtest_receive_fd() {
  uint32_t fd = 0;
  imported_webrogue_gfx_vtest_receive_fd(&fd);
  return fd;
}

__attribute__((import_name("vtest_map_fd")))
__attribute__((import_module("webrogue_gfx")))
void static imported_webrogue_gfx_vtest_map_fd(uint32_t fd, uint8_t *buf, uint32_t buf_len);

void webroguegfx_vtest_map_fd(uint32_t fd, uint8_t *buf, uint32_t buf_len) {
  imported_webrogue_gfx_vtest_map_fd(fd, buf, buf_len);
}