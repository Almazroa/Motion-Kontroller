/**
 * @file IO_expander.c
 * @author Reem Al Mazroa (almazroa@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "i2c.h"
#include "IO_expander.h"
#include <stdlib.h>

int IO_expander_INT = 0;

cy_rslt_t IO_expander_write_reg(uint8_t reg, uint8_t val)
{
	// cy_rslt_t rslt;
	// master sends reg address then data to it 
	uint8_t writeBuffer[2];

	writeBuffer[0] = reg;
	writeBuffer[1] = val;
	
	return cyhal_i2c_master_write(&i2c_master_obj, TCA9534_SUBORDINATE_ADDR, writeBuffer, 2, 0, true);
}

cy_rslt_t IO_expander_read_reg(uint8_t reg, uint8_t *val)
{
	// cy_rslt_t rslt;
	
	// master needs to write the register address first 
	// should this be a while??
	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(&i2c_master_obj, TCA9534_SUBORDINATE_ADDR, &reg, 1, 0, false)) {// no stop condition until read is done

		// Master generates repeated start condition???????????????????????????????????????

		// will read 1 byte into val and send stop
		return cyhal_i2c_master_read(&i2c_master_obj, TCA9534_SUBORDINATE_ADDR, val, 1, 0, true);
	}

	return 1;

}

/* Interrupt handler callback function */
void io_expander_callback(void *callback_arg, cyhal_gpio_event_t event) 
{
    CY_UNUSED_PARAMETER(callback_arg);
    /* set the global interrupt variable */
    // IF INT = 1 WAIT???????????????????????????????????????????
    if (event == CYHAL_GPIO_IRQ_FALL) {
        IO_expander_INT = 1;
    }
    
}



void cyhal_interrupt()
{
    cy_rslt_t rslt;

    /* Initialize pin P9_2 GPIO as an input pin */
    rslt = cyhal_gpio_init(P9_2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    
    cyhal_gpio_callback_data_t *int_handler = malloc(sizeof(cyhal_gpio_callback_data_t));
    int_handler->callback = io_expander_callback;
    int_handler->callback_arg = NULL;

    /* Register callback function */
    cyhal_gpio_register_callback(P9_2, int_handler);
    
    /* Enable falling edge interrupt event with interrupt priority set to 3 */
    cyhal_gpio_enable_event(P9_2, CYHAL_GPIO_IRQ_FALL, 3, true);
}

