/*
 *  Created on: Jan 18, 2022
 *      Author: Joe Krachey
 */

#ifndef SPI_H__
#define SPI_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define prototype1
#undef CONN_SPI_J300
#undef CONN_SPI_J301
#undef CONN_SPI_J302

#if defined(prototype1)
#define PIN_IMU_SPI_MOSI	P10_0
#define PIN_IMU_SPI_MISO	P10_1
#define PIN_IMU_SPI_CLK		P10_2
#define PIN_LCD_SPI_SI      P9_0
#define PIN_LCD_SPI_SCL     P9_2
#define PIN_LCD_SPI_CS      P9_3        
#elif defined(CONN_SPI_J300)
#define PIN_MCU_SPI_MOSI	P9_0
#define PIN_MCU_SPI_MISO	P9_1
#define PIN_MCU_SPI_CLK		P9_2
#define PIN_MCU_SPI_CS      P9_3
#define PIN_IMU_SPI_MOSI	P9_0
#define PIN_IMU_SPI_MISO	P9_1
#define PIN_IMU_SPI_CLK		P9_2
#define PIN_IMU_SPI_CS      P9_3
#elif defined(CONN_SPI_J301)
#define PIN_MCU_SPI_MOSI	P10_0
#define PIN_MCU_SPI_MISO	P10_1
#define PIN_MCU_SPI_CLK		P10_2
#elif defined(CONN_SPI_J302)
#define PIN_MCU_SPI_MOSI	P5_0
#define PIN_MCU_SPI_MISO	P5_1
#define PIN_MCU_SPI_CLK		P5_2
#else
#error "MUST DEFINE A SPI CONNECTOR"
#endif
/* Macros */

#define SPI_FREQ			1000000

/* Public Global Variables */
extern cyhal_spi_t IMU_SPI;
extern cyhal_spi_t screen_SPI;

/* Public API */
cy_rslt_t spi_IMU_init(void);
cy_rslt_t spi_screen_init(void);

#endif 
