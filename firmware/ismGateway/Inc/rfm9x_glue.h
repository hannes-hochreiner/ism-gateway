#ifndef __RFM9X_GLUE_H__
#define __RFM9X_GLUE_H__

#include "rfm9x_lora.h"
#include "spi.h"

void rfm9x_g_init();
void rfm9x_g_start_reception_mode();
void rfm9x_g_get_message(read_func callback);
void rfm9x_g_get_flags(rfm9x_flags_t* const flags);
void rfm9x_g_reset_flags(const rfm9x_flags_t* const flags);
void rfm9x_g_send_message(uint8_t* data, uint8_t length);

#endif
