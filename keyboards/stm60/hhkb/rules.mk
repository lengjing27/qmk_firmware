## chip/board settings
MCU_FAMILY = NRF52
MCU_SERIES = NRF52832

# Linker script to use
# - it should exist either in <nrf_sdk>/config/nrf52832/armgcc
#   or <this_dir>/ld/
MCU_LDSCRIPT = hhkb_nrf52

# Startup code to use
#  - it should exist in <nrf_sdk>/modules/nrfx/mdk
MCU_STARTUP = gcc_startup_nrf52

# Board: it should exist <this_dir>/boards
BOARD = hhkb_nrf52832

# Cortex version
MCU  = cortex-m4

# ARM version, CORTEX-M0/M1 are 6, CORTEX-M3/M4/M7 are 7
ARMV = 7

USE_FPU = yes

# extra definitons
OPT_DEFS =

# Options to pass to dfu-util when flashing
DFU_ARGS =

# Build Options
#   comment out to disable the options.
#
BOOTMAGIC_ENABLE = no # Virtual DIP switch configuration
## (Note that for BOOTMAGIC on Teensy LC you have to use a custom .ld script.)
MOUSEKEY_ENABLE = no # Mouse keys
EXTRAKEY_ENABLE = yes# Audio control and System control
CONSOLE_ENABLE = no# Console for debug
COMMAND_ENABLE = no # Commands for debug and configuration
#SLEEP_LED_ENABLE = yes  # Breathing sleep LED during USB suspend
BACKLIGHT_ENABLE = no
NKRO_ENABLE = no # USB Nkey Rollover
CUSTOM_MATRIX = yes # Custom matrix file
AUDIO_ENABLE = no
#RGB_MATRIX_ENABLE = custom
#RGBLIGHT_ENABLE = no
#RGBLIGHT_CUSTOM_DRIVER = no

# project specific files
