#include "aes_ecb.h"

aes_ecb::aes_ecb(std::array<uint8_t, 16> key) : _key(key.data(), key.size()) {
}

void aes_ecb::decrypt(uint8_t const * const in, uint8_t * const out, int length) {
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption decryptor;

  decryptor.SetKey(this->_key, this->_key.size());
  decryptor.ProcessData(out, in, length);
}

void aes_ecb::encrypt(uint8_t const * const in, uint8_t* const out, int length) {
  CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryptor;

  encryptor.SetKey(this->_key, this->_key.size());
  encryptor.ProcessData(out, in, length);
}
