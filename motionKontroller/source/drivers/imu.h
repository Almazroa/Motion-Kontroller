#ifndef __IMU_H__
#define __IMU_H__

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "spi.h"
#include "lsm6dsm_reg.h"

#define prototype1
#undef CONN_SPI_IMU_CS_J300
#undef CONN_SPI_IMU_CS_J301
#undef CONN_SPI_IMU_CS_J302

#if defined(prototype1)
#define PIN_SPI_IMU_CS	    P10_3
#elif defined(CONN_SPI_IMU_CS_J300)
#define PIN_SPI_IMU_CS	    P9_3
#elif defined(CONN_SPI_IMU_CS_J301)
#define PIN_SPI_IMU_CS	    P10_3
#elif defined(CONN_SPI_IMU_CS_J302)
#define PIN_SPI_IMU_CS	    P5_3
#else
#error "MUST DEFINE A IMU SPI CS CONNECTOR"
#endif


#define IMU_REG_READ             0x80
#define IMU_REG_W$ITE            0x00

/** Device Identification (Who am I) **/
#define LSM6DSM_ID            0x6AU

#define IMU_REG_FIFO_CTRL_1     0x06
#define IMU_REG_FIFO_CTRL_2     0x07
#define IMU_REG_FIFO_CTRL_3     0x08
#define IMU_REG_FIFO_CTRL_4     0x09
#define IMU_REG_FIFO_CTRL_5     0x0A

#define IMU_REG_INT1_CTRL       0x0D
#define IMU_REG_INT2_CTRL       0x0E
#define IMU_REG_WHO_AM_I        0x0F
#define IMU_REG_CTRL1_XL        0x10
#define IMU_REG_CTRL2_G         0x11
#define IMU_REG_CTRL3_C         0x12
#define IMU_REG_CTRL4_C         0x13
#define IMU_REG_CTRL5_C         0x14
#define IMU_REG_CTRL6_C         0x15
#define IMU_REG_CTRL7_G         0x16
#define IMU_REG_CTRL8_XL        0x17
#define IMU_REG_CTRL9_XL        0x18
#define IMU_REG_CTRL10_C        0x19
#define IMU_REG_MASTER_CONFIG   0x1A
#define IMU_REG_WAKE_UP_SCRF    0x1B
#define IMU_REG_TAP_SCR         0x1C
#define IMU_REG_D6D_SCR         0x1D
#define IMU_REG_STATUS_REG      0x1E

#define IMU_REG_OUT_TEMP_L      0x20
#define IMU_REG_OUT_TEMP_H      0x21
#define IMU_REG_OUTX_L_G        0x22
#define IMU_REG_OUTX_H_G        0x23
#define IMU_REG_OUTY_L_G        0x24
#define IMU_REG_OUTY_H_G        0x25
#define IMU_REG_OUTZ_L_G        0x26
#define IMU_REG_OUTZ_H_G        0x27

#define IMU_REG_OUTX_L_XL       0x28
#define IMU_REG_OUTX_H_XL       0x29
#define IMU_REG_OUTY_L_XL       0x2A
#define IMU_REG_OUTY_H_XL       0x2B
#define IMU_REG_OUTZ_L_XL       0x2C
#define IMU_REG_OUTZ_H_XL       0x2D

void lsm6dsm_init(void);
void lsm6dsm_orientation(void);
void lsm6dsm_orientation_single(void);
int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);

extern stmdev_ctx_t dev_ctx;

#endif