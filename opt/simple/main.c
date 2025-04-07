#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

void *thread(void *ptr) {
  *((int *)ptr) = 15;
  printf("Thread created\n");
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t thread1;

  int thread_result = 11;
  int ret = pthread_create(&thread1, NULL, *thread, (void *)&thread_result);

  printf("pthread_create returned: %d\n", ret);

  pthread_join(thread1, NULL);

  printf("Thread result: %d\n", thread_result);

  FILE *file = fopen("/app/test_file.txt", "r");
  printf("file: %ld\n", file);
  fseek(file, 1, SEEK_SET);
  char *data = calloc(1, 100);
  int read = fread(data, 1, 3, file);
  fclose(file);
  printf("read: %d %s\n", read, data);

  file = fopen("/data/test_file.txt", "w");
  printf("file: %ld\n", file);
  fwrite("output text\n", 12, 1, file);
  fclose(file);

  // sleep(2);
  for (int s = 1; s < 40; s++) {
    usleep(200 * 1000);
    printf("s: %d\n", s);
  }
  // imported_func_1(argc);
  // imported_func_3((size_t)argv[0], strlen(argv[0]));
}