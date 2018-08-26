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

void set_colors(const bitbang_calls_t* const calls, const color_t* const colors, uint8_t length);

#endif
