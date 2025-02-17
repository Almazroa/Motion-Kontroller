// /*
//  * push_button.c
//  *
//  *  Created on: Aug 16, 2022
//  *      Author: Joe Krachey
//  */

// #include "push_button.h"

// volatile bool ALERT_PUSH_BUTTON = false;

// /* IO Pin Handler Struct */
// cyhal_gpio_callback_data_t push_button_cb_data =
// {
//     .callback     = push_button_handler,
//     .callback_arg = (void*)NULL
// };


// /***********************************************************
// * Handler for SW2
// ************************************************************/
// void push_button_handler(void* handler_arg, cyhal_gpio_event_t event)
// {
// 	ALERT_PUSH_BUTTON= true;
// }

// /***********************************************************
// * Initialize SW2 Interrupts
// ************************************************************/
// static void push_button_irq_init(void)
// {
// 	// Enable the interrupt for the CapSense Change IRQ
//     cyhal_gpio_register_callback(
//     		PIN_PUSH_BUTTON, 		    // Pin
// 			&push_button_cb_data);		// Handler Callback Info

//     cyhal_gpio_enable_event(
// 			PIN_PUSH_BUTTON,	       	// Pin
// 			CYHAL_GPIO_IRQ_FALL, 	    // Event
// 			INT_PRIORITY_PUSH_BUTTON,   // Priority
// 			true);			            // Enable Event
// }

// /*****************************************************
// * Function Name: push_button_io_init
// ******************************************************
// * Summary:
// *
// * Parameters:
// *  void
// *
// * Return:
// *
// *
// *****************************************************/
// static void push_button_io_init(void)
// {
// 	cy_rslt_t rslt = CY_RSLT_SUCCESS;

//     /* ADD CODE to configure push button as an input */
// 	rslt = cyhal_gpio_init(
// 			PIN_PUSH_BUTTON,            // Pin
// 			CYHAL_GPIO_DIR_INPUT,      // Direction
// 			CYHAL_GPIO_DRIVE_NONE,     // Drive Mode
// 			false);

// 	if(rslt != CY_RSLT_SUCCESS)
// 	{
// 		printf("Error initializing push button pin\n\r");
// 		while(1){};
// 	}
// 	else
// 	{
// 		printf("Initialized Push Button\n\r");
// 	}
// }


// /*****************************************************
// * Function Name: push_button_init
// ******************************************************
// * Summary:
// * Initializes the IO pin used to control the push button
// *
// * Parameters:
// *  void
// *
// * Return:
// *
// *
// *****************************************************/
// void push_button_init(void)
// {
// 	push_button_io_init();
// 	push_button_irq_init();
// }
