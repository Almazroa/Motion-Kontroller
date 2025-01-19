/*
    IMU.c
*/

#include "IMU.h"


cyhal_gpio_callback_data_t IMU_cb_data =
    {
    .callback     = IMU_callback,
    .callback_arg = (void*)NULL
    };

void IMU_callback(void *callback_arg, cyhal_gpio_event_t event)
{
    BaseType_t *pxHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(xIMU, 0, pxHigherPriorityTaskWoken); // pxHigherPriorityTaskWoken means smth
}

static void IMU_irq_init(cyhal_gpio_t pin)
{
    // enable interrrupts on INT2 pin
    uint8_t val = 0x01;
    CY_ASSERT(lsm6dsm_write_reg(&dev_ctx, LSM6DSM_DRDY_PULSE_CFG, &val, 1) == 0);

    cy_rslt_t rslt;

    /* configure push button as an input */
	rslt = cyhal_gpio_init(
			pin,                       // Pin
			CYHAL_GPIO_DIR_INPUT,      // Direction
			CYHAL_GPIO_DRIVE_PULL_NONE,     // Drive Mode
			false);

	if(rslt != CY_RSLT_SUCCESS)
	{
		printf("Error initializing INT2 #%i\n\r", pin);
		while(1){};
	}
	else
	{
		printf("Initialized INT2 #%i\n\r", pin);
	}

    // bool tmp = cyhal_gpio_read(pin
    
    

	// Enable the interrupt for the CapSense Change IRQ
    cyhal_gpio_register_callback(
    		pin, 	           	    // Pin
			&IMU_cb_data);		    // Handler Callback Info

    cyhal_gpio_enable_event(
			pin,	                       	  // Pin
			CYHAL_GPIO_IRQ_BOTH, 	          // Event
			INT_PRIORITY_PUSH_BUTTON,       // Priority
			true);			                  // Enable Event
}

/*
    initializing the absolute wrist tilt fxn
    and INT2 as GPIO interrupt
*/
void IMU_init(void)
{
    // The AWT function is based on the accelerometer sensor only and works at 26 Hz, so the
    // accelerometer ODR must be set at a value of 26 Hz or higher

    // to enable the AWT function it is necessary to set to 1 both the FUNC_EN bit (2) and the
    // WRIST_TILT_EN bit (7) of CTRL10_C (19h).
    if (!lsm6dsm_xl_data_rate_set(&dev_ctx, LSM6DSM_XL_ODR_26Hz)) printf("success sdata_rate_set\n\r"); // Turn on the accelerometer
    lsm6dsm_func_en_set(&dev_ctx, 1); // Enable embedded functions
    vTaskDelay(50 / portTICK_PERIOD_MS);     // wait 50 ms
    lsm6dsm_func_en_set(&dev_ctx, 0); // disable embedded functions 
    
    // set bank B regs
    uint8_t val = 0x32; //32h = 50d = 50 degrees
    if (!lsm6dsm_tilt_threshold_set(&dev_ctx, &val)) printf("success threshold_set\n\r");
    val = 0x05;     // 0Fh = 600ms
    if (!lsm6dsm_tilt_latency_set(&dev_ctx, &val)) printf("success latency_set\n\r");
    val = 0xF0;                         // 111100 enabling x and y axes
    if (!lsm6dsm_tilt_src_set(&dev_ctx, (lsm6dsm_a_wrist_tilt_mask_t*)&val)) printf("success axes set\n\r");
    
    lsm6dsm_6d_threshold_set(&dev_ctx, LSM6DSM_DEG_50);
    lsm6dsm_4d_mode_set(&dev_ctx, 1);
    
    // enable AWT
    if (!lsm6dsm_wrist_tilt_sens_set(&dev_ctx, 1)) printf("success sens_set\n\r");

    
    // val = 0x00;
    // CY_ASSERT(lsm6dsm_write_reg(&dev_ctx, LSM6DSM_DRDY_PULSE_CFG, &val, 1) == 0);

    // IMU_irq_init(IMU_INT2);

}

void IMU_task(void* param)
{

    // poll reading the WRIST_TILT_IA bit (0) of the FUNC_SRC2 (54h) register (it will also clear the
    // interrupt signal if latched)
    
    lsm6dsm_init();
    printf("lsm6dsm success\n\r");
    IMU_init();
    printf("IMU success\n\r");

    while (1) {
        // uint8_t arg;
        // platform_read(NULL, LSM6DSM_FUNC_SRC2, &arg, 1);

        // if ((&arg)[0] == 1) {
        //     printf("tilt detected\n\r");
        // }

        lsm6dsm_all_sources_t all_source;
        BLE_message message = BLE_ERROR;

        /* Check if 6D Orientation events */
        lsm6dsm_tilt_sources_get(&dev_ctx, &all_source);
        if (all_source.func_src2.wrist_tilt_ia)
        {
            message = BLE_ERROR;            // to avoid faulty interrupts
            printf("tilt detected\n\r");
            if (all_source.d6d_src.xh)
            {
                printf("d6d_XH\n\r");       // LEFT ACTUALLY
                if (R) message = BLE_R_IMU_left;
                else message = BLE_L_IMU_left;
            }

            if (all_source.d6d_src.xl)
            {
                printf("d6d_XL\n\r");       // RIGHT ACTUALLY
                if (R) message = BLE_R_IMU_right;
                else message = BLE_L_IMU_right;
            }

            if (all_source.d6d_src.yh)
            {
                printf("d6d_YH\n\r");       // UP
                if (R) message = BLE_R_IMU_down;
                else message = BLE_L_IMU_down;
            }

            if (all_source.d6d_src.yl)
            {
                printf("d6d_YL\n\r");       // DOWN
                if (R) message = BLE_R_IMU_up;
                else message = BLE_L_IMU_up;

            }
            // if (all_source.wrist_tilt_ia.wrist_tilt_ia_xpos)
            // {
            //     printf("tilt_xpos\n\r");
            // }

            // if (all_source.wrist_tilt_ia.wrist_tilt_ia_xneg)
            // {
            //     printf("tilt_xneg\n\r");
            // }

            // if (all_source.wrist_tilt_ia.wrist_tilt_ia_ypos)
            // {
            //     printf("tilt_ypos\n\r");
            // }

            // if (all_source.wrist_tilt_ia.wrist_tilt_ia_yneg)
            // {
            //     printf("tilt_yneg\n\r");
            // }

            // send to BLE queue
            if (message != BLE_ERROR)
            {
                xQueueSend(BLE_queue, &message, portMAX_DELAY);
            }
        }

        vTaskDelay(1/portTICK_PERIOD_MS);

        // // using INT2
        // lsm6dsm_d6d_src_t dir;
        // BLE_message message;
        // uint32_t ulNotificationValue;
        // bool tmp = cyhal_gpio_read(IMU_INT2);
        // printf("tmp from task is %d\n\r", tmp);
		// ulNotificationValue = ulTaskNotifyTakeIndexed( 0,
        //                                            pdTRUE,
        //                                            portMAX_DELAY);
		
		// if( ulNotificationValue == 1 )
        // {
        //     /* received notif from IMU callback = int */
        //     // read direction
        //     CY_ASSERT(lsm6dsm_read_reg(&dev_ctx, LSM6DSM_D6D_SRC,
        //                    (uint8_t *) &dir, 1) == 0);
            
        //     // TODO check if direction is right
        //     if (dir.xh)
        //     {
        //         message = BLE_R_IMU_left;
        //     }

        //     if (dir.xl)
        //     {
        //         message = BLE_R_IMU_right;
        //     }

        //     if (dir.yh)
        //     {
        //         message = BLE_R_IMU_up;
        //     }

        //     if (dir.yl)
        //     {
        //         message = BLE_R_IMU_down;
        //     }

        //     // send to BLE queue
        //     xQueueSend(BLE_queue, &message, portMAX_DELAY);
        // }
        // else
        // {
        //     /* The call to ulTaskNotifyTake() timed out. */
        //     printf("notif from IMU ISR failed");
        // }

    
    }

}