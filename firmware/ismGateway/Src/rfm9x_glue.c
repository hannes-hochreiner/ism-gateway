#include "rfm9x_glue.h"

volatile static rfm9x_t rfm98;

void _rfm9x_g_set_reset_pin();
void _rfm9x_g_reset_reset_pin();
void _rfm9x_g_delay(uint16_t millisec);
void _rfm9x_g_spi_transfer(uint8_t* const data, uint16_t length);

void rfm9x_g_init() {
  rfm98.set_reset_pin = _rfm9x_g_set_reset_pin;
  rfm98.reset_reset_pin = _rfm9x_g_reset_reset_pin;
  rfm98.delay = _rfm9x_g_delay;
  rfm98.spi_transfer = _rfm9x_g_spi_transfer;

  LL_GPIO_SetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin);
  LL_SPI_Enable(SPI1);

  RFM9X_Init(&rfm98);
  RFM9X_Reset(&rfm98);

  rfm9x_mode_t mode = RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_SLEEP;
  
  RFM9X_SetMode(&rfm98, &mode);
  mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_SLEEP;
  RFM9X_SetMode(&rfm98, &mode);
  mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_STANDBY;
  RFM9X_SetMode(&rfm98, &mode);

  rfm9x_conf1_t conf1 = RFM9X_CONF1_BW_125_KHZ | RFM9X_CONF1_CR_4_5 | RFM9X_CONF1_HEADER_EXPL;
  RFM9X_SetConf1(&rfm98, &conf1);
  rfm9x_conf2_t conf2 = RFM9X_CONF2_SF_128;
  RFM9X_SetConf2(&rfm98, &conf2);
  rfm9x_conf3_t conf3 = RFM9X_CONF3_AGC_AUTO_ON;
  RFM9X_SetConf3(&rfm98, &conf3);

  uint8_t power = 0x00;
  RFM9X_SetPower(&rfm98, &power);
}

void rfm9x_g_start_reception_mode() {
  rfm9x_mode_t mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_RECEIVE_CONT;
  RFM9X_SetMode(&rfm98, &mode);
}

void rfm9x_g_get_message(read_func callback) {
  RFM9X_ReadMessage(&rfm98, callback);
  rfm9x_flags_t flags = RFM9X_FLAG_ALL;
  RFM9X_ResetFlags(&rfm98, &flags);
}

void rfm9x_g_get_flags(const rfm9x_flags_t* const flags) {
  RFM9X_GetFlags(&rfm98, flags);
}

void rfm9x_g_send_message(uint8_t* data, uint8_t length) {
  rfm9x_mode_t mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_STANDBY;
  RFM9X_SetMode(&rfm98, &mode);

  rfm9x_flags_t flags;
  rfm9x_flags_t flags_all = RFM9X_FLAG_ALL;

  RFM9X_WriteMessage(&rfm98, data, length);
  
  mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_TRANSMIT;
  RFM9X_SetMode(&rfm98, &mode);
  RFM9X_GetMode(&rfm98, &mode);
  RFM9X_GetFlags(&rfm98, &flags);
  
  while (((flags & RFM9X_FLAG_TX_DONE) != RFM9X_FLAG_TX_DONE) ||
         ((mode & 0x07) != RFM9X_MODE_STANDBY)) {
    RFM9X_GetMode(&rfm98, &mode);
    RFM9X_GetFlags(&rfm98, &flags);
    delay(25);
  }

  RFM9X_ResetFlags(&rfm98, &flags_all);
  mode = RFM9X_MODE_LORA | RFM9X_MODE_LOW_FREQ_MODE | RFM9X_MODE_SLEEP;
  RFM9X_SetMode(&rfm98, &mode);
}

void _rfm9x_g_set_reset_pin() {
  LL_GPIO_SetOutputPin(RFM_RESET_GPIO_Port, RFM_RESET_Pin);
}

void _rfm9x_g_reset_reset_pin() {
  LL_GPIO_ResetOutputPin(RFM_RESET_GPIO_Port, RFM_RESET_Pin);
}

void _rfm9x_g_delay(uint16_t millisec) {
  LL_mDelay(millisec);
}

void _rfm9x_g_spi_transfer(uint8_t* const data, uint16_t length) {
  LL_GPIO_ResetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin);

  for (uint16_t cntr = 0; cntr < length; cntr++) {
    while (!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
    LL_SPI_TransmitData8(SPI1, *(data + cntr));
    while (!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
    while (!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    *(data + cntr) = LL_SPI_ReceiveData8(SPI1);
    while (!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
  }

  LL_GPIO_SetOutputPin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin);
}
