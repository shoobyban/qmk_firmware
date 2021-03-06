#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xBEEF
#define PRODUCT_ID      0x0221
#define DEVICE_VER      0x0111
#define MANUFACTURER    Sam
#define PRODUCT         SamKeyboard v3
#define DESCRIPTION     Sam Ban Keyboard Rev 3

#define GPA0 0x0
#define GPA1 0x1
#define GPA2 0x2
#define GPA3 0x3
#define GPA4 0x4
#define GPA5 0x5
#define GPA6 0x6
#define GPA7 0x7
#define GPB0 0x8
#define GPB1 0x9
#define GPB2 0xA
#define GPB3 0xB
#define GPB4 0xC
#define GPB5 0xD
#define GPB6 0xE
#define GPB7 0xF

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 15
#define MATRIX_LEFT_COLS 7

/* key matrix pins */
//                        A0  A1  A2  A3  A4
#define MATRIX_ROW_PINS { F7, F6, F5, F4, F1 }
//                                        A5  5   6   RX  12  13  TX  10
//#define MATRIX_COL_PINS {  0,0,0,0,0,0,0, F0, C6, D7, D2, D6, C7, D3, B6 }
//                                        A5  RX  TX  5   6   13  12  10
#define MATRIX_COL_PINS {  0,0,0,0,0,0,0, F0, D2, D3, C6, D7, C7, D6, B6 }

/* key matrix pins */
#define MATRIX_LEFT_ROW_PINS { GPA4, GPA3, GPA2, GPA1, GPA0 }
#define MATRIX_LEFT_COL_PINS { GPB0, GPB1, GPB2, GPB3, GPB4, GPB5, GPB6 }

#define UNUSED_PINS

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION ROW2COL

#define BACKLIGHT_PIN B5

#ifdef BACKLIGHT_PIN
#ifndef BACKLIGHT_LEVELS
#define BACKLIGHT_LEVELS 5
#endif
#endif

#define BATPIN 9
#define MAX_VOLTAGE 4.2
#define MIN_VOLTAGE 3.2
#define BATTERY_POLL 30000

#define DEBOUNCING_DELAY 5

#define OLED_DISPLAY_128X64
#define OLED_DISPLAY_ADDRESS 0x3c

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS
