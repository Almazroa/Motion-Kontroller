/******************************************************************************
* File Name: main.c
*
* Description: This is the source code for the FreeRTOS
*              LE Hello Sensor Example for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2021-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "main.h"

TaskHandle_t xscreen = NULL;
TaskHandle_t xIMU = NULL;
TaskHandle_t xPWM = NULL;
TaskHandle_t xBLE = NULL;
QueueHandle_t BLE_queue;

bool R = 0;

/******************************************************************************
 * Function Definitions
 ******************************************************************************/

/**
 * Function Name : main
 *
 * Function Description :
 *   @brief Entry point to the application. Set device configuration and start
 *   BT stack initialization.  The actual application initialization will happen
 *   when stack reports that BT device is ready.
 *
 *   @param: None
 *
 *   @return: None
 */
int main()
{
    cy_rslt_t cy_result;
    wiced_result_t wiced_result;

    /* Initialize the board support package */
    cy_result = cybsp_init();

    if (CY_RSLT_SUCCESS != cy_result)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    #ifdef ENABLE_BT_SPY_LOG
    {
        cybt_debug_uart_config_t config = {
            .uart_tx_pin = CYBSP_DEBUG_UART_TX,
            .uart_rx_pin = CYBSP_DEBUG_UART_RX,
            .uart_cts_pin = CYBSP_DEBUG_UART_CTS,
            .uart_rts_pin = CYBSP_DEBUG_UART_RTS,
            .baud_rate = DEBUG_UART_BAUDRATE,
            .flow_control = TRUE};
        cybt_debug_uart_init(&config, NULL);
    }
    #else
    {
        /* Initialize retarget-io to use the debug UART port */
        cy_retarget_io_init(P6_5,
                            P6_4,
                            CY_RETARGET_IO_BAUDRATE);
    }
    #endif //ENABLE_BT_SPY_LOG

    // printf("******************** "
    //        "BTSTACK FreeRTOS Example "
    //        "************************\n");

    // printf("****************** "
    //        "Hello Sensor Application Start "
    //        "********************\n");

    printf("\x1b[2J\x1b[;H");

    printf("******************\n\r");
    printf("* ECE453 Dev Platform\n\r");
    printf("******************\n\r");

    /*
        BLE init
    */

    /* Configure platform specific settings for the BT device */
    cybt_platform_config_init(&cybsp_bt_platform_cfg);

    /*Initialize the block device used by kv-store for performing
     * read/write operations to the flash*/
    app_kvstore_bd_config(&block_device);

    /* Register call back and configuration with stack */
    wiced_result = wiced_bt_stack_init(app_bt_management_callback,
                                       &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if(WICED_BT_SUCCESS == wiced_result)
    {
        printf("Bluetooth Stack Initialization Successful \n");
    }
    else
    {
        printf("Bluetooth Stack Initialization failed!! \n");
        CY_ASSERT(0);
    }

    /*
        Peripheral init
    */
    if (spi_IMU_init() != CY_RSLT_SUCCESS) printf("spi_init failed\n\r");
    buttons_init();
    if (spi_screen_init() != CY_RSLT_SUCCESS) printf("spi_init failed\n\r");

    CY_ASSERT((BLE_queue = xQueueCreate(20, sizeof(BLE_message))) != NULL);

    /*
        Task creation
    */
    // idle task = 0 priority 
    if (R) xTaskCreate(screen_task, "screen", configMINIMAL_STACK_SIZE, NULL, 5, &xscreen);

    if (
        xTaskCreate(IMU_task, "IMU", configMINIMAL_STACK_SIZE, NULL, 5, &xIMU) &&
        xTaskCreate(PWM_task, "PWM", configMINIMAL_STACK_SIZE, NULL, 5, &xPWM) &&
        xTaskCreate(BLE_task, "BLE", configMINIMAL_STACK_SIZE, NULL, 6, &xBLE)
        )
    {
        /* Start the FreeRTOS scheduler */
        vTaskStartScheduler();
    }

    /* Should never get here */
    CY_ASSERT(0) ;
}

/* END OF FILE [] */
