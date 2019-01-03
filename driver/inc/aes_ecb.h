#ifndef __AES_ECB_H__
#define __AES_ECB_H__

#include <memory>
#include <array>

#include <cryptopp/cryptlib.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>

#include "aes_exception.h"

#include <iostream>

class aes_ecb
{
private:
  const CryptoPP::SecByteBlock _key;
  aes_ecb() = delete;
  aes_ecb(const aes_ecb&) = delete;
public:
  aes_ecb(std::array<uint8_t, 16> key);
  void decrypt(const uint8_t* const in, uint8_t* const out, int length);
  void encrypt(const uint8_t* const in, uint8_t* const out, int length);
};

#endif
