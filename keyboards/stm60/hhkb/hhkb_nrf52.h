/**
 * hhkb_nrf52.h
 */
#pragma once
#include <stdint.h>
#include <stdbool.h>

#include <nrf_gpio.h>

/*
 * row:     p0.27, p0.28, p0.29
 * col:     p0.30, p0.31, p0.02, p0.03
 * key:     p0.25
 * prev:    p0.26
 * power:   p0.04
 */
static inline void KEY_ENABLE(void) { nrf_gpio_pin_clear(3); }
static inline void KEY_UNABLE(void) { nrf_gpio_pin_set(3); }
static inline bool KEY_STATE(void) { return nrf_gpio_pin_read(25); }
static inline void KEY_PREV_ON(void) { nrf_gpio_pin_set(26); }
static inline void KEY_PREV_OFF(void) { nrf_gpio_pin_clear(26); }

static inline void KEY_POWER_ON(void) {}
static inline void KEY_POWER_OFF(void) {}
static inline bool KEY_POWER_STATE(void) { return true; }

static inline void KEY_INIT(void)
{
    /* row,col,prev: output */
    nrf_gpio_cfg_output(27);
    nrf_gpio_cfg_output(28);
    nrf_gpio_cfg_output(29);

    nrf_gpio_cfg_output(30);
    nrf_gpio_cfg_output(31);
    nrf_gpio_cfg_output(2);
    nrf_gpio_cfg_output(3);

    nrf_gpio_cfg_output(26);

    /* key: input with pull-up */
    nrf_gpio_cfg_input(25, NRF_GPIO_PIN_PULLUP);

    KEY_UNABLE();
    KEY_PREV_OFF();

    KEY_POWER_OFF();
}

const uint32_t KEY_COLS[] = {30, 31, 2, 3};
const uint32_t KEY_ROWS[] = {27, 28, 29};

static inline void KEY_SELECT(uint8_t ROW, uint8_t COL)
{
    nrf_gpio_pin_set(KEY_COLS[COL]);
    nrf_gpio_pin_set(KEY_ROWS[ROW]);
}
