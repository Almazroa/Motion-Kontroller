/*
    BLE.h
*/



/* Pin definitions */


/* Global vars*/


/* Public Function API */
cy_rslt_t BLE_init();    // initializes BLE attr.
void BLE_task(void* param); // receieves alerts in the queue from other peripherals and sends notifications to shaker

/* Macros */
typedef enum
{
  BLE_R_IMU_left        = 0,
  BLE_R_IMU_right         = 1,
  BLE_R_IMU_up          = 2,
  BLE_R_IMU_down        = 3,
  BLE_R_bumper         = 4,
  BLE_R_trigger          = 5,
  BLE_R_button_up       = 6,
  BLE_R_button_down     = 7,
  BLE_R_button_right    = 8,
  BLE_R_button_left     = 9,
  BLE_L_IMU_left        = 10,
  BLE_L_IMU_right       = 11,
  BLE_L_IMU_up          = 12,
  BLE_L_IMU_down        = 13,
  BLE_L_trigger         = 14,
  BLE_L_bumper          = 15,
  BLE_L_button_up       = 16,
  BLE_L_button_down     = 17,
  BLE_L_button_right    = 18,
  BLE_L_button_left     = 19,
  BLE_ERROR             = 20,                
} BLE_message;