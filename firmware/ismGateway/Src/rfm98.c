#include "rfm98.h"

const uint8_t RFM98_READ = 0x00;
const uint8_t RFM98_WRITE = 0x80;

const uint8_t RFM98_REG_FIFO = 0x00;
const uint8_t RFM98_REG_MODE = 0x01;
const uint8_t RFM98_REG_FREQUENCY = 0x06;
const uint8_t RFM98_REG_SYNC_CONFIG = 0x27;
const uint8_t RFM98_REG_VERSION = 0x42;
const uint8_t RFM98_REG_IRQ_FLAGS = 0x3e;

void RFM98_ReadMessage(const rfm98_t* const rfm98, read_func callback) {
  uint8_t com = RFM98_READ | RFM98_REG_FIFO;
  uint8_t length = 0;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&length);

  uint8_t data[length];

  for (uint8_t cntr = 0; cntr < length; cntr++) {
    rfm98->spi_transfer(data + cntr);
  }

  rfm98->set_spi_nss_pin();

  callback(data, length);
}

void RFM98_GetVersion(const rfm98_t* const rfm98, uint8_t* const version) {
  uint8_t com = RFM98_READ | RFM98_REG_VERSION;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(version);
  rfm98->set_spi_nss_pin();
}

void RFM98_Init(const rfm98_t* const rfm98) {
  rfm98->set_reset_pin();
  rfm98->delay(15);
}

void RFM98_Reset(const rfm98_t* const rfm98) {
  rfm98->reset_reset_pin();
  rfm98->delay(15);
  rfm98->set_reset_pin();
}

void RFM98_GetFrequency(const rfm98_t* const rfm98, uint32_t* const frequencyHz) {
  uint8_t com = RFM98_READ | RFM98_REG_FREQUENCY;
  uint8_t val;

  *frequencyHz = 0;
  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&val);
  *frequencyHz += val << 16;
  rfm98->spi_transfer(&val);
  *frequencyHz += val << 8;
  rfm98->spi_transfer(&val);
  *frequencyHz += val;
  rfm98->set_spi_nss_pin();
  *frequencyHz *= 61;
}

void RFM98_GetSyncWord(const rfm98_t* const rfm98, uint8_t syncWord[8], uint8_t* const length) {
  uint8_t com = RFM98_READ | RFM98_REG_SYNC_CONFIG;
  uint8_t syncConfig;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&syncConfig);
  for (uint8_t cntr = 0; cntr < 8; cntr++) {
    rfm98->spi_transfer(syncWord + cntr);
  }
  rfm98->set_spi_nss_pin();
  *length = (syncConfig & 0b00000111) + 1;
}

void RFM98_SetSyncWord(const rfm98_t* const rfm98, const uint8_t* const syncWord, uint8_t length) {
  uint8_t com = RFM98_READ | RFM98_REG_SYNC_CONFIG;
  uint8_t syncConfig;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&syncConfig);
  rfm98->set_spi_nss_pin();
  length -= 1;
  length &= 0b00000111;
  syncConfig &= 0b11111000;
  syncConfig |= length;
  com = RFM98_WRITE | RFM98_REG_SYNC_CONFIG;
  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&syncConfig);
  for (uint8_t cntr = 0; cntr < length + 1; cntr++) {
    uint8_t syncWordByte = *(syncWord + cntr);
    rfm98->spi_transfer(&syncWordByte);
  }
  rfm98->set_spi_nss_pin();
}

void RFM98_GetMode(const rfm98_t* const rfm98, rfm98_mode_t* const mode) {
  uint8_t com = RFM98_READ | RFM98_REG_MODE;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(mode);
  rfm98->set_spi_nss_pin();

  *mode &= 0b00000111;
}

void RFM98_SetMode(const rfm98_t* const rfm98, const rfm98_mode_t* const mode) {
  uint8_t com = RFM98_READ | RFM98_REG_MODE;
  uint8_t oldMode = 0;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&oldMode);
  rfm98->set_spi_nss_pin();

  oldMode &= 0b11111000;
  oldMode |= *mode;
  com = RFM98_WRITE | RFM98_REG_MODE;

  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&oldMode);
  rfm98->set_spi_nss_pin();
}

void RFM98_GetFlags(const rfm98_t* const rfm98, uint16_t* const flags) {
  uint8_t com = RFM98_READ | RFM98_REG_IRQ_FLAGS;
  uint8_t flagByte = 0;

  *flags = 0;
  rfm98->reset_spi_nss_pin();
  rfm98->spi_transfer(&com);
  rfm98->spi_transfer(&flagByte);
  *flags += flagByte << 8;
  rfm98->spi_transfer(&flagByte);
  *flags += flagByte;
  rfm98->set_spi_nss_pin();
}
