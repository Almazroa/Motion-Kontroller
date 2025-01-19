/*
    PWM.c
*/

#include "PWM.h"

cyhal_pwm_t pwm_obj;

cy_rslt_t shaker_init()
{
    cy_rslt_t rslt;

    /* Initialize PWM on the supplied pin and assign a new clock */
    // rslt = cyhal_pwm_init(&pwm_obj, shaker, NULL);
    // might need to try oneshot vs continuous using advanced init
    rslt = cyhal_pwm_init_adv(&pwm_obj, shaker, NC, CYHAL_PWM_LEFT_ALIGN, false, 0, false, NULL);
    CY_ASSERT(CY_RSLT_SUCCESS == rslt);
    /* Set a duty cycle of 50% and frequency of 1Hz */
    rslt = cyhal_pwm_set_duty_cycle(&pwm_obj, 10, 1);   //TODO might change after testing

    

    return rslt;
}

void PWM_task(void* param)
{
    cy_rslt_t rslt;
    uint32_t ulNotificationValue;
    CY_ASSERT(CY_RSLT_SUCCESS == shaker_init());
    while (1)
    {
        // block task to recieve message from button 
        ulNotificationValue = ulTaskNotifyTakeIndexed( 0,
                                                   pdTRUE,
                                                   portMAX_DELAY);
		
		if( ulNotificationValue == 1 )
        {
            /* The transmission ended as expected. */
            rslt = cyhal_pwm_start(&pwm_obj);
            CY_ASSERT(CY_RSLT_SUCCESS == rslt);

            // vTaskDelay(20 / portTICK_PERIOD_MS);

            // rslt = cyhal_pwm_stop(&pwm_obj);
            // CY_ASSERT(CY_RSLT_SUCCESS == rslt);
        }
        else
        {
            /* The call to ulTaskNotifyTake() timed out. */
        }
    }
}