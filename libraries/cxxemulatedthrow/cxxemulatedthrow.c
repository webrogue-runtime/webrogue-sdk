#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *__cxa_allocate_exception(size_t thrown_size) {
  printf("__cxa_allocate_exception\n");
  size_t actual_size = thrown_size + 1024;
  void *result = malloc(actual_size);
  memset(result, 0, actual_size);
  return ((char *)result) + 512;
}

void __cxa_throw(void *thrown_object, void *tinfo, void (*dest)(void *)) {
  printf("__cxa_throw\n");
  __builtin_unreachable();
  abort();
}
