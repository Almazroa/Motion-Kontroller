/*
    screen.h
*/

#include "../main.h"

/* Pin definitions */
#define PIN_LCD_SPI_RST P10_5
#define PIN_LCD_SPI_A0  P9_1

/* Macros */
#define NUM_PAGE 4
#define NUM_COL 128
#define PAGE_ADDR 0xB0

/* Global vars*/


/* Public Function API */
cy_rslt_t screen_spi_init(void);
void data_out(unsigned char data);    // send the data to be displayed in the screen
void comm_out(unsigned char command); // send the command to control the functions in screen
void display_message(unsigned char *message);
void lcd_init(void);
void screen_task(void* param);