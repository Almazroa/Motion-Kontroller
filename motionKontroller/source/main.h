/*
 * main.h
 *
 *  Created on: Aug 16, 2022
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>

#include <stdio.h>
#include <stdlib.h>

#include "app_flash_common.h"
#include "app_bt_bonding.h"
#include "mtb_kvstore.h"
#include "cycfg_bt_settings.h"
#include "wiced_bt_stack.h"
#include "cybsp_bt_config.h"
#include "cybt_platform_config.h"
#include "app_bt_event_handler.h"
#include "app_bt_gatt_handler.h"
#include "app_hw_device.h"
#include "app_bt_utils.h"
#ifdef ENABLE_BT_SPY_LOG
#include "cybt_debug_uart.h"
#endif

#include "drivers/leds.h"
#include "drivers/push_button.h"
#include "drivers/console.h"
#include "drivers/i2c.h"
#include "drivers/eeprom.h"
#include "drivers/leds.h"
#include "drivers/relay.h"
#include "drivers/led_driver.h"
#include "drivers/IO_expander.h"
#include "drivers/spi.h"
#include "drivers/imu.h"
#include "motion_controller/buttons.h"
#include "motion_controller/IMU.h"
#include "motion_controller/screen.h"
#include "motion_controller/PWM.h"
#include "motion_controller/BLE.h"


//#define DEMO_RELAY
#define DEMO_IO_PINS
//#define DEMO_LED_DRIVER
//#define DEMO_SPI_EEPROM
// #define R_BOARD
#define L_BOARD

extern TaskHandle_t xblink;
extern TaskHandle_t xbuttons;
extern TaskHandle_t xIMU;
extern TaskHandle_t xscreen;
extern TaskHandle_t xPWM;
extern TaskHandle_t xBLE;
extern QueueHandle_t BLE_queue;

extern bool R;

#endif /* MAIN_H_ */
