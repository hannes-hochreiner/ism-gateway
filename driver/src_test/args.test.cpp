#include <gtest/gtest.h>
#include "args.h"

namespace {
  TEST(global, round_trip) {
    std::array<uint8_t, 16> key = {
      0x00, 0x11, 0x22, 0x33,
      0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF
    };

    const char* argv[] = {
      "arg-test",
      "-k",
      "00112233445566778899AABBCCDDEEFF"
    };

    args a(3, argv);

    std::array<uint8_t, 16> k = a.get_key();

    for (uint cntr = 0; cntr < 16; cntr++) {
      EXPECT_EQ(k[cntr], key[cntr]);
    }
  }
}
