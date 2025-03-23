/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f421_wk_config.h
  * @brief    header file of work bench config
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __AT32F421_WK_CONFIG_H
#define __AT32F421_WK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "stdio.h"
#include "at32f421.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* add user code begin dma define */
/* user can only modify the dma define value */
#define DMA1_CHANNEL1_BUFFER_SIZE   0
#define DMA1_CHANNEL1_MEMORY_BASE_ADDR   0
#define DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR  0

//#define DMA1_CHANNEL2_BUFFER_SIZE   0
//#define DMA1_CHANNEL2_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL3_BUFFER_SIZE   0
//#define DMA1_CHANNEL3_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL4_BUFFER_SIZE   0
//#define DMA1_CHANNEL4_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL5_BUFFER_SIZE   0
//#define DMA1_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL5_PERIPHERAL_BASE_ADDR   0
/* add user code end dma define */

/* Private defines -------------------------------------------------------------*/
#define CS_PIN    GPIO_PINS_13
#define CS_GPIO_PORT    GPIOC
#define RESET_PIN    GPIO_PINS_0
#define RESET_GPIO_PORT    GPIOA
#define RS_PIN    GPIO_PINS_1
#define RS_GPIO_PORT    GPIOA
#define RD_PIN    GPIO_PINS_2
#define RD_GPIO_PORT    GPIOA
#define WR_PIN    GPIO_PINS_3
#define WR_GPIO_PORT    GPIOA
#define SPI1_CS_PIN    GPIO_PINS_4
#define SPI1_CS_GPIO_PORT    GPIOA
#define DB0_PIN    GPIO_PINS_0
#define DB0_GPIO_PORT    GPIOB
#define DB1_PIN    GPIO_PINS_1
#define DB1_GPIO_PORT    GPIOB
#define DB2_PIN    GPIO_PINS_2
#define DB2_GPIO_PORT    GPIOB
#define DB10_PIN    GPIO_PINS_10
#define DB10_GPIO_PORT    GPIOB
#define DB11_PIN    GPIO_PINS_11
#define DB11_GPIO_PORT    GPIOB
#define DB12_PIN    GPIO_PINS_12
#define DB12_GPIO_PORT    GPIOB
#define DB13_PIN    GPIO_PINS_13
#define DB13_GPIO_PORT    GPIOB
#define DB14_PIN    GPIO_PINS_14
#define DB14_GPIO_PORT    GPIOB
#define DB15_PIN    GPIO_PINS_15
#define DB15_GPIO_PORT    GPIOB
#define DB3_PIN    GPIO_PINS_3
#define DB3_GPIO_PORT    GPIOB
#define DB4_PIN    GPIO_PINS_4
#define DB4_GPIO_PORT    GPIOB
#define DB5_PIN    GPIO_PINS_5
#define DB5_GPIO_PORT    GPIOB
#define DB6_PIN    GPIO_PINS_6
#define DB6_GPIO_PORT    GPIOB
#define DB7_PIN    GPIO_PINS_7
#define DB7_GPIO_PORT    GPIOB
#define DB8_PIN    GPIO_PINS_8
#define DB8_GPIO_PORT    GPIOB
#define DB9_PIN    GPIO_PINS_9
#define DB9_GPIO_PORT    GPIOB

/* exported functions ------------------------------------------------------- */
  /* system clock config. */
  void wk_system_clock_config(void);

  /* config periph clock. */
  void wk_periph_clock_config(void);

  /* nvic config. */
  void wk_nvic_config(void);

/* add user code begin exported functions */

/* add user code end exported functions */

#ifdef __cplusplus
}
#endif

#endif
