#ifndef __rfm98_H
#define __rfm98_H
#include "stdio.h"

typedef void(*void_func)();
typedef void(*delay_func)(uint8_t millisec);
typedef void(*spi_transfer_func)(uint8_t* const data);
typedef void(*read_func)(const uint8_t* const data, uint8_t length);
typedef struct {
  void_func set_reset_pin;
  void_func reset_reset_pin;
  void_func set_spi_nss_pin;
  void_func reset_spi_nss_pin;
  delay_func delay;
  spi_transfer_func spi_transfer;
} rfm98_t;
typedef enum {
  RFM98_MODE_SLEEP = 0x00,
  RFM98_MODE_STANDBY = 0x01,
  RFM98_MODE_TRANSMIT = 0x03,
  RFM98_MODE_RECEIVE = 0x05
} rfm98_mode_t;
typedef enum {
  RFM98_FLAG_MODE_READY = 0x8000,
  RFM98_FLAG_RX_READY = 0x4000,
  RFM98_FLAG_TX_READY = 0x2000,
  RFM98_FLAG_PLL_LOCK = 0x1000,
  RFM98_FLAG_RSSI = 0x0800,
  RFM98_FLAG_TIMEOUT = 0x0400,
  RFM98_FLAG_PREAMBLE_DETECT = 0x0200,
  RFM98_FLAG_SYNC_ADDRESS_MATCH = 0x0100,
  RFM98_FLAG_FIFO_FULL = 0x0080,
  RFM98_FLAG_FIFO_EMPTY = 0x0040,
  RFM98_FLAG_FIFO_LEVEL = 0x0020,
  RFM98_FLAG_FIFO_OVERRUN = 0x0010,
  RFM98_FLAG_PACKET_SENT = 0x0008,
  RFM98_FLAG_PAYLOAD_READY = 0x0004,
  RFM98_FLAG_CRC_OK = 0x0002,
  RFM98_FLAG_LOW_BAT = 0x0001
} rfm98_flags_t;

void RFM98_Init(const rfm98_t* const rfm98);
void RFM98_Reset(const rfm98_t* const rfm98);
void RFM98_GetVersion(const rfm98_t* const rfm98, uint8_t* const version);
void RFM98_GetFrequency(const rfm98_t* const rfm98, uint32_t* const frequencyHz);
void RFM98_GetSyncWord(const rfm98_t* const rfm98, uint8_t syncWord[8], uint8_t* const length);
void RFM98_SetSyncWord(const rfm98_t* const rfm98, const uint8_t* const syncWord, uint8_t length);
void RFM98_GetMode(const rfm98_t* const rfm98, rfm98_mode_t* const mode);
void RFM98_SetMode(const rfm98_t* const rfm98, const rfm98_mode_t* const mode);
void RFM98_GetFlags(const rfm98_t* const rfm98, uint16_t* const flags);
void RFM98_ReadMessage(const rfm98_t* const rfm98, read_func callback);
#endif
