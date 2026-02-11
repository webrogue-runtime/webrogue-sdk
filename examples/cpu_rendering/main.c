#include <stdint.h>
#include <stdlib.h>
#include <webroguegfx/webroguegfx.h>

int main(int argc, char **argv) {
  wr_window window;
  webroguegfx_make_window(&window);
  int width, height;
  uint8_t *buffer = malloc(1);
  char needs_resize = 1;
  while (1) {
    char should_close = 0;
    while (1) {
      webrogue_event event = webroguegfx_poll();
      switch (event.type) {
      case WEBROGUE_EVENT_TYPE_QUIT: {
        should_close = 1;
        break;
      }
      case WEBROGUE_EVENT_TYPE_GL_RESIZED: {
        needs_resize = 1;
        break;
      }
      default:
        break;
      }
      if (event.type == WEBROGUE_EVENT_TYPE_INVALID)
        break;
    }
    if (should_close)
      break;
    if (needs_resize) {
      webroguegfx_gl_size(window, &width, &height);
      buffer = realloc(buffer, width * height * 4);
      needs_resize = 0;
    }
    for (int x = 0; x < width; x++)
      for (int y = 0; y < height; y++) {
        int offset = 4 * (x + width * y);
        buffer[offset + 3] = 0;
        // Red
        buffer[offset + 2] = x % 256;
        // Green
        buffer[offset + 1] = y % 256;
        // Blue
        buffer[offset + 0] = (x * y) % 256;
      }
    webroguegfx_present_pixels(window, buffer, width * height * 4);
  }
}
