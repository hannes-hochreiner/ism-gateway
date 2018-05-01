#include "tlc59731.h"
// 20 kHz to 600 kHz
// 5 pulses: on, off, on/off, off, off
// byte counter 0..length, bit counter 0..7, sequence counter 0..4
// EOS = 3.5 - 5.5 t_cycle without rise
void set_colors(bitbang_calls_t calls, color_t* colors, uint8_t length) {
  uint8_t colorCntr = 0;

  while (colorCntr < length) {
    uint8_t seqCounter = 0;
    uint8_t bitCounter = 0;
    uint8_t byteCounter = 0;
    uint8_t data[] = {0x3A, colors[colorCntr].b, colors[colorCntr].g, colors[colorCntr].r, 0x00};

    while (byteCounter < 5) {
      if (byteCounter == 4) {
        (*(calls.reset_pin))();
      } else if (seqCounter == 0) {
        (*(calls.set_pin))();
      } else if (seqCounter == 1 || seqCounter == 3 || seqCounter == 4) {
        (*(calls.reset_pin))();
      } else {
        if ((data[byteCounter] << bitCounter) & 0x80) {
          (*(calls.set_pin))();
        } else {
          (*(calls.reset_pin))();
        }
      }

      (*(calls.delay))();
      seqCounter++;

      if (seqCounter > 4) {
        bitCounter++;
        seqCounter = 0;
      }

      if ((byteCounter == 4 && bitCounter > 3) || bitCounter > 7) {
        byteCounter++;
        bitCounter = 0;
      }
    }

    colorCntr++;
  }
}
