/*
    BLE.c
*/

#include "../main.h"

const char* getBLEMessageName(BLE_message message) {
    switch (message) {
        case BLE_R_IMU_left:        return "BLE_R_IMU_left";
        case BLE_R_IMU_right:       return "BLE_R_IMU_right";
        case BLE_R_IMU_up:          return "BLE_R_IMU_up";
        case BLE_R_IMU_down:        return "BLE_R_IMU_down";
        case BLE_R_trigger:         return "BLE_R_trigger";
        case BLE_R_bumper:          return "BLE_R_bumper";
        case BLE_R_button_up:       return "BLE_R_button_up";
        case BLE_R_button_down:     return "BLE_R_button_down";
        case BLE_R_button_right:    return "BLE_R_button_right";
        case BLE_R_button_left:     return "BLE_R_button_left";
        case BLE_L_IMU_left:        return "BLE_L_IMU_left";
        case BLE_L_IMU_right:       return "BLE_L_IMU_right";
        case BLE_L_IMU_up:          return "BLE_L_IMU_up";
        case BLE_L_IMU_down:        return "BLE_L_IMU_down";
        case BLE_L_trigger:         return "BLE_L_trigger";
        case BLE_L_bumper:          return "BLE_L_bumper";
        case BLE_L_button_up:       return "BLE_L_button_up";
        case BLE_L_button_down:     return "BLE_L_button_down";
        case BLE_L_button_right:    return "BLE_L_button_right";
        case BLE_L_button_left:     return "BLE_L_button_left";
        case BLE_ERROR:             return "BLE_ERROR";
        default:                    return "Unknown BLE_message";
    }
}

void BLE_task(void* param)
{
    //CY_ASSERT(CY_RSLT_SUCCESS == BLE_init());
    BLE_message message;
    while (1)
    {
        // if vibrate
        // xTaskNotifyGiveIndexed(xPWM, 0);

        // block until there's smth in queue
        // change if this becomes an ISR
        // bool tmp = cyhal_gpio_read(IMU_INT2);
        // printf("tmp from BLE is %d\n\r", tmp);
        if (xQueueReceive(BLE_queue, &message, portMAX_DELAY) == pdTRUE) // never suspends
            {
            printf("%s\n\r", getBLEMessageName(message));
            if (message != BLE_ERROR) {
                    app_bt_gatt_set_notify_value(message);
                    app_bt_send_message();
                }
            }

        // vTaskDelay(10 / portTICK_PERIOD_MS);
        // send message over Bluetooth

    }
}