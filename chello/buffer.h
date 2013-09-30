#include <string.h>
#include <stdlib.h>
#pragma once

typedef struct {
  size_t len;
  size_t cap;
  char* str;
} Buffer;

static void buffer_append(Buffer* buf, const char* s, size_t l) {
  if(buf->len + l + 1 > buf->cap) {
    buf->cap = buf->cap * 2 + l;
    buf->str = realloc(buf->str, buf->cap);
  }
  strncpy(buf->str + buf->len, s, l);
  buf->len += l;
}

static Buffer buffer_new(size_t cap) {
  Buffer b = {0, cap, (char*)malloc(cap)};
  return b;
}
