/*
    button.h
*/

#include "../main.h"

/* Pin definitions */
#undef prototype1
#define prototype2

#if defined(prototype2)
#define PB1_R_thumb_upper               P7_1        // thumb upper
#define PB2_R_thumb_lower               P5_0        // thumb lower
#define PB3_R_pinky                     P5_1        // pinky
#define PB4_R_ring                      P5_2        // ring
#define PB5_R_middle                    P5_3        // middle
#define PB6_R_index                     P10_6       // index
#define PB_R_trigger                    PB1_R_thumb_upper 
#define PB_R_bumper                     PB2_R_thumb_lower
#define PB_R_button_up                  PB4_R_ring
#define PB_R_button_down                PB5_R_middle 
#define PB_R_button_right               PB3_R_pinky
#define PB_R_button_left                PB6_R_index

#define PB1_L_thumb_upper                P5_5        // thumb upper
#define PB2_L_thumb_lower                P5_0        // thumb lower
#define PB3_L_index                      P5_1        // index
#define PB4_L_middle                     P5_2        // middle
#define PB5_L_ring                       P5_3        // ring
#define PB6_L_pinky                      P10_6       // pinky 
#define PB_L_trigger                     PB1_L_thumb_upper
#define PB_L_bumper                      PB2_L_thumb_lower
#define PB_L_button_up                   PB5_L_ring
#define PB_L_button_down                 PB4_L_middle
#define PB_L_button_right                PB6_L_pinky
#define PB_L_button_left                 PB3_L_index

#elif defined(prototype1)
// only left
#define PB1_L_thumb_upper                P10_6        // thumb upper
#define PB2_L_thumb_lower                P10_4        // thumb lower
#define PB3_L_index                      P5_3        // index
#define PB4_L_middle                     P5_2        // middle
#define PB5_L_ring                       P5_0        // ring
#define PB6_L_pinky                      P5_1       // pinky 
#define PB_L_trigger                     PB1_L_thumb_upper
#define PB_L_bumper                      PB2_L_thumb_lower
#define PB_L_button_up                   PB5_L_ring
#define PB_L_button_down                 PB4_L_middle
#define PB_L_button_right                PB6_L_pinky
#define PB_L_button_left                 PB3_L_index
#endif


/* Global vars*/


/* Public Function API */
void buttons_init();    // initializes all 6 buttons & their interrupts
void button_callback(void *callback_arg, cyhal_gpio_event_t event); // alerts the BLE that a button was clicked
void button_PWM_callback(void *callback_arg, cyhal_gpio_event_t event);