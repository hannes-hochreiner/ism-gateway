#ifndef __tlc59731_H
#define __tlc59731_H
#include <stdio.h>

typedef void(*void_func)();
typedef struct {
  void_func delay;
  void_func set_pin;
  void_func reset_pin;
} bitbang_calls_t;
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

void set_colors(bitbang_calls_t calls, color_t* colors, uint8_t length);

#endif
