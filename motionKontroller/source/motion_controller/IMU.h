/*
    IMU.h
*/

#include "../main.h"

/* Pin definitions */
#define IMU_INT2        P10_4

void IMU_init(void);
void IMU_task(void* param);
void IMU_callback(void *callback_arg, cyhal_gpio_event_t event);