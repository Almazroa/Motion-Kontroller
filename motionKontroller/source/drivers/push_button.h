/*
 * ece453.h
 *
 *  Created on: Jan 18, 2022
 *      Author: Joe Krachey
 */

#ifndef PUSH_BUTTON_H_
#define PUSH_BUTTON_H_

#include <stdio.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "leds.h"

// Pin definitions for the ECE453 Staff Dev board
#define PIN_800_PB	         			P5_6 
#define PIN_801_PB                      P7_1
#define INT_PRIORITY_PUSH_BUTTON	5

// Exported Global Variables
extern volatile bool ALERT_PUSH_BUTTON;

/* Public Function API */
void push_button_handler(void* handler_arg, cyhal_gpio_event_t event);
void push_button_init(void);


#endif
