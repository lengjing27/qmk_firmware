/**
 * hhkb_nrf52832.h
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs definitions
#define LEDS_NUMBER    3

#define LED_1          19
#define LED_2          20
#define LED_3          22

#define LEDS_ACTIVE_STATE 1

#define LEDS_INV_MASK  LEDS_MASK

#define LEDS_LIST { LED_1, LED_2, LED_3}

#define BSP_LED_0      LED_1
#define BSP_LED_1      LED_2
#define BSP_LED_2      LED_3

#define BUTTONS_NUMBER 3

#define BUTTON_START   8
#define BUTTON_1       8
#define BUTTON_2       9
#define BUTTON_3       10
#define BUTTON_STOP    10
#define BUTTON_PULL    NRF_GPIO_PIN_PULLDOWN

#define BUTTONS_ACTIVE_STATE 1

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3}

#define BSP_BUTTON_0   BUTTON_1
#define BSP_BUTTON_1   BUTTON_2
#define BSP_BUTTON_2   BUTTON_3

#ifdef __cplusplus
}
#endif
