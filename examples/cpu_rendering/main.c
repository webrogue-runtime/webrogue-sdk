#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <libwr4c/wr4c.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function(void *ptr) {\
  FILE *file = fopen("/data/file.txt", "rb");
  if (file == NULL) {
    printf("Error opening file\n");
    abort();
  }
  char buff[5];
  size_t read = fread(buff, 1, 4, file);
  if (read != 4) {
    printf("Error reading file\n");
    abort();
  }
  buff[4] = 0;

  while (1) {
    sleep(1);
    printf("Hello pthread (%s)\n", buff);
  }
  return NULL;
}

int main(int argc, char **argv) {
  wr4c_window_t window;
  wr4c_make_window(&window);

  pthread_t thread1;
  pthread_create(&thread1, NULL, print_message_function, "Thread 1");
  int width, height;
  uint8_t *buffer = malloc(1);
  char needs_resize = 1;
  char should_close = 0;
  while (1) {
    if (needs_resize) {
      wr4c_gl_size(window, &width, &height);
      buffer = realloc(buffer, width * height * 4);
      needs_resize = 0;
    
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
      wr4c_present_pixels(window, buffer, width * height * 4);
    }
    while (1) {
#define SHOULD_STOP_POLLING (should_close || needs_resize)
      wr4c_event_t event = wr4c_poll_timeout(SHOULD_STOP_POLLING ? 0 : -1);
      switch (event.tag) {
      case WR4C_EVENT_TAG_QUIT: {
        should_close = 1;
        break;
      }
      case WR4C_EVENT_TAG_GL_RESIZED: {
        needs_resize = 1;
        break;
      }
      default:
        break;
      }
      if (SHOULD_STOP_POLLING && event.tag == WR4C_EVENT_TAG_INVALID)
        break;
#undef SHOULD_STOP_POLLING
    }
    if (should_close)
      break;
  }
  return 0;
}
