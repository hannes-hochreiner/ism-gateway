#ifndef __rfm98_glue_h
#define __rfm98_glue_h

#include "main.h"
#include "rfm98.h"

void RFM98Glue_Init(rfm98_t* const rfm98);
void RFM98Glue_Set_Reset_Pin();
void RFM98Glue_Reset_Reset_Pin();
void RFM98Glue_Set_NSS_Pin();
void RFM98Glue_Reset_NSS_Pin();
void RFM98Glue_SPI_Transfer(uint8_t* const data);
void RFM98Glue_Delay(uint8_t millisec);

#endif
