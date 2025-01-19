/*
    buttons.c
*/

#include "buttons.h"

cyhal_gpio_callback_data_t buttons_cb_data[14];
uint8_t num_buttons = 0;

typedef struct {
    cyhal_gpio_t pin;
    BLE_message message;
} PinAndMessage;

cyhal_gpio_callback_data_t button_800_cb_data =
    {
    .callback     = button_callback,
    .callback_arg = (void*)BLE_L_button_up
    };

cyhal_gpio_callback_data_t button_801_cb_data =
    {
    .callback     = button_callback,
    .callback_arg = (void*)BLE_L_button_down
    };

// callback_arg = pin# of button that caused interrupt
void button_callback(void *callback_arg, cyhal_gpio_event_t event)
{
	PinAndMessage* pin_and_message = callback_arg;
	BaseType_t * var_idk = pdFALSE;
	// debounce
	if (!cyhal_gpio_read(pin_and_message->pin))		// should be zero
	{
	//TODO: unsure if this should be true of false
	CY_ASSERT(xQueueSendFromISR(BLE_queue, &(pin_and_message->message), var_idk) == pdPASS);
	}
	// {
	// 	printf("added button interrupt to BLE queue\n\r");
	// } 
	// else 
	// {
	// 	printf("FAILED to add button interrupt to BLE queue\n\r");
	// }
}

void button_PWM_callback(void *callback_arg, cyhal_gpio_event_t event)
{
	PinAndMessage* pin_and_message = callback_arg;
	BaseType_t * var_idk = pdFALSE;
	BaseType_t *pxHigherPriorityTaskWoken = pdFALSE;
	bool btn_state = cyhal_gpio_read(pin_and_message->pin);
	btn_state = cyhal_gpio_read(pin_and_message->pin);
	btn_state = cyhal_gpio_read(pin_and_message->pin);

	if (((event == CYHAL_GPIO_IRQ_FALL) && (!btn_state))		// clicked
		| ((event == CYHAL_GPIO_IRQ_RISE) && (btn_state)))
	{
    	vTaskNotifyGiveIndexedFromISR(xPWM, 0, pxHigherPriorityTaskWoken);
		CY_ASSERT(xQueueSendFromISR(BLE_queue, &(pin_and_message->message), var_idk) == pdPASS);
	}
	// {
	// 	printf("added button interrupt to BLE queue\n\r");
	// } 
	// else 
	// {
	// 	printf("FAILED to add button interrupt to BLE queue\n\r");
	// }	

}

static BLE_message Lpin_to_BLE_message_decoder(cyhal_gpio_t pin)
{
	switch(pin) 
	{
		case PB_L_bumper:
			return BLE_L_bumper;
		case PB_L_trigger:
			return BLE_L_trigger;
		case PB_L_button_down:
			return BLE_L_button_down;
		case PB_L_button_left:
			return BLE_L_button_left;
		case PB_L_button_right:
			return BLE_L_button_right;
		case PB_L_button_up:
			return BLE_L_button_up;
		case PIN_800_PB:
			return BLE_L_button_up;
		case PIN_801_PB:
			return BLE_L_button_down;
		default:
		return BLE_ERROR;
	}
}

static BLE_message Rpin_to_BLE_message_decoder(cyhal_gpio_t pin)
{
	switch(pin) 
	{
		case PB_R_bumper:
			return BLE_R_bumper;
		case PB_R_trigger:
			return BLE_R_trigger;
		case PB_R_button_down:
			return BLE_R_button_down;
		case PB_R_button_left:
			return BLE_R_button_left;
		case PB_R_button_right:
			return BLE_R_button_right;
		case PB_R_button_up:
			return BLE_R_button_up;
		default:
		return BLE_ERROR;
	}
}

static void button_irq_init(cyhal_gpio_t pin)
{
	// BLE_message* pin_queue_message = malloc(sizeof(BLE_message));
	// *pin_queue_message = Rpin_to_BLE_message_decoder(pin);
	// CY_ASSERT(*pin_queue_message != BLE_ERROR); 

	// MALLOC
	PinAndMessage* pin_and_message = malloc(sizeof(PinAndMessage));
    if (!pin_and_message) {
        // Handle memory allocation failure
        return;
    }

	pin_and_message->pin = pin;
	if (R) pin_and_message->message = Rpin_to_BLE_message_decoder(pin);
    else pin_and_message->message = Lpin_to_BLE_message_decoder(pin);
    CY_ASSERT(pin_and_message->message != BLE_ERROR);

	
	if ((pin == PB1_L_thumb_upper) | (pin == PB2_L_thumb_lower)
		| (pin == PB1_R_thumb_upper) | (pin == PB2_R_thumb_lower))
	{
		buttons_cb_data[num_buttons].callback = button_PWM_callback;
		buttons_cb_data[num_buttons].callback_arg = (void*) pin_and_message;
	}
	else 
	{
		buttons_cb_data[num_buttons].callback = button_callback;
		buttons_cb_data[num_buttons].callback_arg = (void*) pin_and_message;
	}

	// Enable the interrupt for the CapSense Change IRQ
    cyhal_gpio_register_callback(
    		pin, 					    // Pin
			&buttons_cb_data[num_buttons]);		    // Handler Callback Info

	if ((pin == PB1_L_thumb_upper) | (pin == PB2_L_thumb_lower)
		| (pin == PB1_R_thumb_upper) | (pin == PB2_R_thumb_lower))
	{
		cyhal_gpio_enable_event(
			pin,	       				// Pin
			CYHAL_GPIO_IRQ_BOTH, 	    // Event
			INT_PRIORITY_PUSH_BUTTON,   // Priority
			true);
	}
	else 
	{
		cyhal_gpio_enable_event(
			pin,	       				// Pin
			CYHAL_GPIO_IRQ_FALL, 	    // Event
			INT_PRIORITY_PUSH_BUTTON,   // Priority
			true);
	}
    			            // Enable Event

	num_buttons++;
}

static void button_init(cyhal_gpio_t pin)
{
	cy_rslt_t rslt;

    /* configure push button as an input */
	rslt = cyhal_gpio_init(
			pin,                       // Pin
			CYHAL_GPIO_DIR_INPUT,      // Direction
			CYHAL_GPIO_DRIVE_NONE,     // Drive Mode
			false);

	if(rslt != CY_RSLT_SUCCESS)
	{
		printf("Error initializing push button pin #%i\n\r", pin);
		while(1){};
	}
	else
	{
		printf("Initialized Push Button #%i\n\r", pin);
	}

	button_irq_init(pin);

}


void buttons_init(void) 
{
	if (R)
	{
		button_init(PB1_R_thumb_upper);
		button_init(PB2_R_thumb_lower);
		button_init(PB3_R_pinky);
		button_init(PB4_R_ring);
		button_init(PB5_R_middle);
		button_init(PB6_R_index);
	}
	else 
	{
		button_init(PB1_L_thumb_upper);
		button_init(PB2_L_thumb_lower);
		button_init(PB3_L_index);
		button_init(PB4_L_middle);
		button_init(PB5_L_ring);
		button_init(PB6_L_pinky);
	}

	// button_init(PIN_800_PB);
	// button_init(PIN_801_PB);
}








// make the cyhal_gpio_callback_data_t struct in the init fxn
// and set the cb arg to the pin #
// this way we know which button caused the interrupt and send that to the BLE queue