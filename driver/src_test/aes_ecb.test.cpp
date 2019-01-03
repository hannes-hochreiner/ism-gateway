#include <gtest/gtest.h>
#include "aes_ecb.h"

namespace {
  TEST(global, round_trip) {
    std::array<uint8_t, 16> key = {
      0x00, 0x11, 0x22, 0x33,
      0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF
    };

    uint8_t data_plain[] = {
      0x00, 0x11, 0x22, 0x33,
      0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xAA, 0xBB,
      0xCC, 0xDD, 0xEE, 0xFF
    };
    uint8_t data_encrypted[16];
    uint8_t data_recovered[16];

    aes_ecb crypt(key);

    crypt.encrypt(data_plain, data_encrypted, 16);
    crypt.decrypt(data_encrypted, data_recovered, 16);

    for (uint cntr = 0; cntr < 16; cntr++) {
      EXPECT_EQ(data_plain[cntr], data_recovered[cntr]);
    }
  }

  TEST(global, round_trip_check) {
    uint8_t data_check[] = {
      0x75, 0x7C, 0xCD, 0x0C,
      0xDC, 0x5C, 0x90, 0xEA,
      0xDB, 0xEE, 0xEC, 0xF6,
      0x38, 0xDD, 0x00, 0x00
    };

    std::array<uint8_t, 16> key = {
      0x31, 0x32, 0x33, 0x34,
      0x35, 0x36, 0x37, 0x38,
      0x39, 0x30, 0x31, 0x32,
      0x33, 0x34, 0x35, 0x36
    };

    uint8_t data_plain[] = {
      0x31, 0x32, 0x33, 0x34,
      0x35, 0x36, 0x37, 0x38,
      0x39, 0x30, 0x31, 0x32,
      0x33, 0x34, 0x35, 0x36
    };

    uint8_t data_encrypted[16];
    uint8_t data_recovered[16];

    aes_ecb crypt(key);

    crypt.encrypt(data_plain, data_encrypted, 16);

    for (uint cntr = 0; cntr < 16; cntr++) {
      EXPECT_EQ(data_check[cntr], data_encrypted[cntr]);
    }

    crypt.decrypt(data_encrypted, data_recovered, 16);

    for (uint cntr = 0; cntr < 16; cntr++) {
      EXPECT_EQ(data_plain[cntr], data_recovered[cntr]);
    }
  }

  TEST(global, stm32_check) {
    uint8_t data_check[] = {
      125, 16, 110, 251,
      145, 232, 84, 226,
      151, 231, 153, 251,
      207, 165, 180, 243
    };

    std::array<uint8_t, 16> key = {
      0xCC, 0xDD, 0xEE, 0xFF,
      0x99, 0x00, 0xAA, 0xBB,
      0x55, 0x66, 0x77, 0x88,
      0x11, 0x22, 0x33, 0x44
    };

    uint8_t data_plain[] = {
      0x5A, 0x3E, 0xDC, 0x85,
      0x18, 0x34, 0x92, 0x12,
      0x01, 0xF3, 0x8A, 0x54,
      0x86, 0x8B, 0x2D, 0x47
    };

    uint8_t data_encrypted[16];
    uint8_t data_recovered[16];

    aes_ecb crypt(key);

    crypt.encrypt(data_plain, data_encrypted, 16);

    for (uint cntr = 0; cntr < 16; cntr++) {
      uint8_t dc = data_check[cntr];
      uint8_t de = data_encrypted[cntr];
      EXPECT_EQ(data_check[cntr], data_encrypted[cntr]);
    }

    crypt.decrypt(data_encrypted, data_recovered, 16);

    for (uint cntr = 0; cntr < 16; cntr++) {
      EXPECT_EQ(data_plain[cntr], data_recovered[cntr]);
    }
  }
}
