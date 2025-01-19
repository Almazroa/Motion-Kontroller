/*
    PWM.h
*/

#include "../main.h"

/* Pin definitions */
#define shaker             P6_2

/* Global vars*/
// extern cyhal_pwm_t pwm_obj;


/* Public Function API */
cy_rslt_t shaker_init();    // initializes all 6 buttons & their interrupts
void PWM_task(void* param); // receieves alert from BLE to vibrate