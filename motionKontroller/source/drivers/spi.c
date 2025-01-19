/*
 * ece453.c
 *
 *  Created on: Jan 18, 2022
 *      Author: Joe Krachey
 */

#include "spi.h"


cyhal_spi_t screen_SPI;
cyhal_spi_t IMU_SPI;

cy_rslt_t spi_screen_init(void)
{
	cy_rslt_t   rslt;

	memset(&screen_SPI, 0, sizeof(screen_SPI));

    // Configuring the  SPI master:  Specify the SPI interface pins, frame size, SPI Motorola mode
    // and master mode
    rslt = cyhal_spi_init(
    						&screen_SPI,
							PIN_LCD_SPI_SI,				// MOSI Pin
							NC,							// MISO Pin
							PIN_LCD_SPI_SCL,			// Clock Pin
							PIN_LCD_SPI_CS,						 	// CS -- Will control using an IO pin
							NULL,						// Clock Source -- if not provided a new clock will be allocated
							8,							// Bits per frame
							CYHAL_SPI_MODE_00_MSB,		// SPI Mode
							false						// Is Subordinate??
						);

    if (rslt == CY_RSLT_SUCCESS)
    {
        // Set the data rate to 1 Mbps
    	rslt = cyhal_spi_set_frequency(&screen_SPI, SPI_FREQ);
    }

	return rslt;

}

cy_rslt_t spi_IMU_init(void)
{
	cy_rslt_t   rslt;

	memset(&IMU_SPI, 0, sizeof(IMU_SPI));

    // Configuring the  SPI master:  Specify the SPI interface pins, frame size, SPI Motorola mode
    // and master mode
    rslt = cyhal_spi_init(
    						&IMU_SPI,
							PIN_IMU_SPI_MOSI,			// MOSI Pin
							PIN_IMU_SPI_MISO,			// MISO Pin
							PIN_IMU_SPI_CLK,			// Clock Pin
							NC,						 	// CS -- Will control using an IO pin
							NULL,						// Clock Source -- if not provided a new clock will be allocated
							8,							// Bits per frame
							CYHAL_SPI_MODE_00_MSB,		// SPI Mode
							false						// Is Subordinate??
						);

    if (rslt == CY_RSLT_SUCCESS)
    {
        // Set the data rate to 1 Mbps
    	rslt = cyhal_spi_set_frequency(&IMU_SPI, SPI_FREQ);
    }

	return rslt;

}