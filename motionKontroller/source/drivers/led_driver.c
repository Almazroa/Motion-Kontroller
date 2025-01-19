/**
 * @file led_driver.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2024-02-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "i2c.h"
#include "led_driver.h"

cy_rslt_t led_driver_write_reg(uint8_t reg, uint8_t val)
{
	// cy_rslt_t rslt;
	// master sends reg address then data to it 
	uint8_t writeBuffer[2];

	writeBuffer[0] = reg;
	writeBuffer[1] = val;
	/* ADD CODE */
	return cyhal_i2c_master_write(&i2c_master_obj, LED_DRIVER_SUBORDINATE_ADDR, writeBuffer, 2, 0, true);
}

cy_rslt_t led_driver_read_reg(uint8_t reg, uint8_t *val)
{
	// cy_rslt_t rslt;
	/* ADD CODE */
	// master needs to write the register address first 
	// should this be a while??
	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(&i2c_master_obj, LED_DRIVER_SUBORDINATE_ADDR, &reg, 1, 0, false)) {// no stop condition until read is done

		// Master generates repeated start condition???????????????????????????????????????

		// will read 1 byte into val and send stop
		return cyhal_i2c_master_read(&i2c_master_obj, LED_DRIVER_SUBORDINATE_ADDR, val, 1, 0, true);
	}

	return 1;

}