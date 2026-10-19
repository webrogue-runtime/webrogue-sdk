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
  char needs_redraw = 1;
  char should_close = 0;
  while (!should_close) {
    if (needs_resize) {
      wr4c_physical_size(window, &width, &height);
    }
    size_t buffer_size = (size_t)width * (size_t)height * 4;
    if (needs_resize && buffer_size > 0) {
      uint8_t *resized_buffer = realloc(buffer, buffer_size);
      if (resized_buffer == NULL) {
        printf("Error allocating pixel buffer\n");
        abort();
      }
      buffer = resized_buffer;
      needs_resize = 0;
      needs_redraw = 1;
    }
    if (needs_redraw && width > 0 && height > 0) {
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

      size_t buffer_size = (size_t)width * (size_t)height * 4;
      wr4c_present_pixels(window, buffer, (uint32_t)buffer_size);
      needs_redraw = 0;
    }

    char redraw_or_quit_event_received = 0;
    wr4c_event_t event;
    while (1) {
      event = wr4c_poll_timeout(redraw_or_quit_event_received ? 0 : -1);

      if (event.tag == WR4C_EVENT_TAG_INVALID) {
        if (redraw_or_quit_event_received) {
          break;
        }
        continue;
      }

      switch (event.tag) {
      case WR4C_EVENT_TAG_QUIT:
        should_close = 1;
        redraw_or_quit_event_received = 1;
        break;
      case WR4C_EVENT_TAG_RESIZED:
      case WR4C_EVENT_TAG_SURFACE_ATTACHED:
        needs_resize = 1;
        redraw_or_quit_event_received = 1;
        break;
      case WR4C_EVENT_TAG_REDRAW_REQUESTED:
        needs_redraw = 1;
        redraw_or_quit_event_received = 1;
        break;
      default:
        break;
      }
    }
  }
  wr4c_destroy_window(window);
  free(buffer);
  return 0;
}
