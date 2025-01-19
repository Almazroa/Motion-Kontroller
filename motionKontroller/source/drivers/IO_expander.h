/*
 * IO_expander.h
 *
 *  Created on: Mar 15, 2024
 *      Author: Reem Al Mazroa
 */

#ifndef __IO_EXPANDER_H__ 
#define __IO_EXPANDER_H__ 

#include <stdint.h>
#include <stdbool.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define TCA9534_SUBORDINATE_READ_ADDR             0x41
#define TCA9534_SUBORDINATE_WRITE_ADDR            0x40
#define TCA9534_SUBORDINATE_ADDR                  0x20

#define TCA9534_REG_INPUT_PORT                    0x00
#define TCA9534_REG_OUTPUT_PORT                   0x01
#define TCA9534_REG_POLARITY_INVERSION_PORT       0x02
#define TCA9534_REG_CONFIG_PORT                   0x03

extern int IO_expander_INT;

cy_rslt_t IO_expander_write_reg(uint8_t reg, uint8_t val);
cy_rslt_t IO_expander_read_reg(uint8_t reg, uint8_t *val);
void cyhal_interrupt();


#endif /* IO_EXPANDER_H_ */
