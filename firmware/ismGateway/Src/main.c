
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l0xx_hal.h"
#include "aes.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "rfm98_glue.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
typedef enum {
  TIMER_PENDING,
  TIMER_DONE
} timer_status_t;
volatile timer_status_t timer_status = TIMER_DONE;
volatile color_t colorSystem = {0x00, 0x03, 0x00};
volatile color_t colorTransmit = {0x00, 0x03, 0x00};
const color_t COLOR_RED = {0x03, 0x00, 0x00};
const color_t COLOR_GREEN = {0x00, 0x03, 0x00};
const color_t COLOR_BLUE = {0x00, 0x00, 0x03};
const color_t COLOR_YELLOW = {0x03, 0x03, 0x00};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_AES_Init();
  /* USER CODE BEGIN 2 */
  rfm9x_t rfm98;
  RFM98Glue_Init(&rfm98);
  RFM9X_Init(&rfm98);
  uint8_t syncWord[] = {0x46, 0xA5, 0xE3};
  RFM9X_SetSyncWord(&rfm98, syncWord, 3);
  rfm9x_mode_t setMode = RFM9X_MODE_RECEIVE;
  RFM9X_SetMode(&rfm98, &setMode);

  bitbang_calls_t led_calls = {led_delay, led_set_pin, led_reset_pin};
  SetColors(&led_calls, &colorSystem, &colorTransmit);

  LL_DBGMCU_EnableDBGSleepMode();
  LL_DBGMCU_EnableDBGStopMode();
  LL_DBGMCU_EnableDBGStandbyMode();

  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  // safety delay
  LL_mDelay(1000);

  // test encrypt
  uint8_t plain[] = {0xAA, 0x11, 0x12, 0x13, 0xAA, 0x11, 0x12, 0x13, 0xAA, 0x11, 0x12, 0x13, 0xAA, 0x11, 0x12, 0x13};
  uint8_t encr[16];
  HAL_CRYP_AESECB_Encrypt(&hcryp, plain, 16, encr, 1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    while (LL_GPIO_IsInputPinSet(RFM_IO0_GPIO_Port, RFM_IO0_Pin) == 1) {
      uint16_t flags;
      RFM9X_GetFlags(&rfm98, &flags);

      if (flags & RFM9X_FLAG_PAYLOAD_READY) {
        colorTransmit = COLOR_BLUE;
        SetColors(&led_calls, &colorSystem, &colorTransmit);
        RFM9X_ReadMessage(&rfm98, ReadData);
        LL_mDelay(50);
        colorTransmit = COLOR_GREEN;
      } else if (flags & RFM9X_FLAG_RSSI) {
        colorTransmit = COLOR_GREEN;
      } else {
        colorTransmit = COLOR_YELLOW;
      }

      SetColors(&led_calls, &colorSystem, &colorTransmit);
    }

    __WFI();
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();  
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);

  LL_RCC_HSI48_Enable();

   /* Wait till HSI48 is ready */
  while(LL_RCC_HSI48_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4, LL_RCC_PLL_DIV_2);

  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(32000000);

  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

  LL_SetSystemCoreClock(32000000);

  LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_HSI48);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 0);
}

/* USER CODE BEGIN 4 */
void led_set_pin() {
  LL_GPIO_SetOutputPin(LED_SDI_GPIO_Port, LED_SDI_Pin);
}

void led_reset_pin() {
  LL_GPIO_ResetOutputPin(LED_SDI_GPIO_Port, LED_SDI_Pin);
}

void led_delay() {
  timer_status = TIMER_PENDING;
  LL_TIM_EnableIT_UPDATE(TIM2);
  LL_TIM_EnableCounter(TIM2);

  while(timer_status == TIMER_PENDING) {
    __WFI();
  }

  LL_TIM_DisableCounter(TIM2);
  LL_TIM_DisableIT_UPDATE(TIM2);
}

void ReadData(const uint8_t* const data, uint8_t length) {
  uint8_t statusUSB = USBD_OK;

  if ((hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) || (hUsbDeviceFS.dev_state == USBD_STATE_SUSPENDED)) {
    statusUSB = CDC_Transmit_FS(data, length);
  }

  if (statusUSB == USBD_OK) {
    colorSystem = COLOR_GREEN;
  } else if (statusUSB == USBD_BUSY) {
    colorSystem = COLOR_BLUE;
  } else if (statusUSB == USBD_FAIL) {
    colorSystem = COLOR_RED;
  } else {
    colorSystem = COLOR_YELLOW;
  }
}

void SetColors(const bitbang_calls_t* const calls, const color_t* const cSystem, const color_t* const cTransmit) {
  color_t colors[2] = {*cSystem, *cTransmit};
  set_colors(calls, colors, 2);
}

void TIMER_Callback() {
  timer_status = TIMER_DONE;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
