/**
 * stm60.c
 */

#include "stm60.h"

#include "rgblight.h"

#include "indicator_leds.h"
#include "ws2812.h"

extern rgblight_config_t rgblight_config;

void rgblight_set(void) {
    if (!rgblight_config.enable) {
        for (uint8_t i = 0; i < RGBLED_NUM; i++) {
            led[i].r = 0;
            led[i].g = 0;
            led[i].b = 0;
        }
    }
    for (uint8_t j = 0; j < RGBLED_NUM; j++) {
      ws2812_write_led(j, led[j].r, led[j].g, led[j].b);
    }
}

__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
  indicator_leds_init();
  ws2812_init();

	matrix_init_user();
}

/*static void test_ws2812(void)
{
  uint8_t r, g, b;
  for ( uint32_t i=0; i< WS2812_LED_N; i++){
    r = rand()%256;
    g = rand()%256;
    b = rand()%256;
    ws2812_write_led(i, r, g, b);
  }
}*/

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_scan_kb(void) {
	matrix_scan_user();
  //test_ws2812();
  rgblight_task();
}
