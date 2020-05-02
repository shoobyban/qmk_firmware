/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Mike Roberts

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "timer.h"
#include "expander.h"
#include "quantum.h"
#include "outputselect.h"
#include "adafruit_ble.h"
#include "nofn.h"

#define RIGHTSIDE

#ifndef CONFIG_H
#include "config.h" // make editors happy
#endif


#ifndef DEBOUNCING_DELAY
#   define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
    static uint16_t debouncing_time;
    static bool debouncing = false;
#endif

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_LEFT_ROW_PINS;

static const uint8_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
static const uint8_t right_row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

#define print_matrix_header()  print("\nr/c 01234567\n")
#define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#define matrix_bitpop(i)       bitpop(matrix[i])
#define ROW_SHIFTER ((uint8_t)1)

uint16_t l_time = 0;
uint16_t delaymul = 1;
uint8_t upshiftcode = 0;
uint8_t shifton = 0;

static void unselect_rows(void);
static void select_row(uint8_t row);

enum custom_keycodes {
    KC_OUTB = SAFE_RANGE,
    KC_OUTU
};

void matrix_init_custom(void) {
    // TODO: initialize hardware here
    expander_init();
    // initialize row and col
    unselect_rows();
}

void matrix_init_kb(void) {

    matrix_init_user();

    if (USB_DeviceState == DEVICE_STATE_Configured) {
        set_output(OUTPUT_USB);
    } else if (adafruit_ble_is_connected()) {
        set_output(OUTPUT_BLUETOOTH);
    } else {
        set_output(OUTPUT_USB);
    }

//    expander_init();
    // initialize row and col
//    unselect_rows();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        setPinInputHigh(col_pins[i]);
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

}

void matrix_set_row(uint8_t row, matrix_row_t val)
{
    matrix[row] = val;
}

void matrix_scan_kb() {
    matrix_scan_user();
}

bool matrix_scan_custom(matrix_row_t current_matrix[])
{
    bool changed = false;

    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        if (col_pins[i] == 0) {
            continue;
        }
        setPinOutput(col_pins[i]);
        writePinLow(col_pins[i]);
        matrix_io_delay();

        // For each col...
        for (uint8_t j = 0; j < MATRIX_ROWS; j++) {
            matrix_row_t last_row_value = current_matrix[j];
            setPinInputHigh(right_row_pins[j]);
            if (readPin(right_row_pins[j]) == 0) {
                // Pin LO, set col bit
                current_matrix[j] |= (MATRIX_ROW_SHIFTER << i);
            } else {
                // Pin HI, clear col bit
                current_matrix[j] &= ~(MATRIX_ROW_SHIFTER << i);
            }
            if ((last_row_value != current_matrix[j]) && !(changed)) {
                changed = true;
            }
        }
        setPinInputHigh(col_pins[i]);

    }

    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {

        select_row(i);
        wait_us(30);
        // orig = current_matrix[i];
        uint8_t readval = expander_read_row();
        if (readval != 0) {
            selbuf[0] = readval;
            selbuf[1] = i;
        }
        matrix_row_t newval = (current_matrix[i] & ~0x7F) | readval;

//        uprintf("orig(%d): %d read: %d => %d\n", i, orig, readval, newval);

        if (newval != current_matrix[i]) {
            debouncing = true;
            debouncing_time = timer_read();
        }

        if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)) {
            newval = current_matrix[i];
            debouncing = false;
        }

        if (newval != current_matrix[i]) {
            changed = true;
            current_matrix[i] = newval;
        }
    }

        if (l_time && (timer_elapsed(l_time) > 50*delaymul)) {
            delaymul = 1;
            changed = true;
            l_time = timer_read();
            if (!upshiftcode) {
                tap_code(KC_UP);
            } else {
                shifton = 1;
                tap_code16(LSFT(upshiftcode));
            }
        }
    return changed;
}

static void select_row(uint8_t row)
{
    uint8_t pin = row_pins[row];
    expander_select(pin);
}

static void unselect_rows(void)
{
    expander_unselect_all();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LSFT:
            if (record->event.pressed) {
                shifton = 1;
            } else {
                shifton = 0;
            }
            break;
        case KC_OUTB:
            set_output(OUTPUT_BLUETOOTH);
            break;
        case KC_OUTU:
            set_output(OUTPUT_USB);
            break;
        case KC_UP:
            if (record->event.pressed) {
                l_time = timer_read(); // init timer
                delaymul = 6;
            } else {
                if (l_time) { // only if not combo
                    l_time = 0;
                    shifton = 0;
                    if (!upshiftcode) {
                        tap_code(KC_UP);
                    } else {
                        shifton = 0;
                    }
                    upshiftcode = 0;
                }
            }
            return false;
            break;
        default:
            if (record->event.pressed) {
                delaymul = 6;
                if (l_time) {
                    l_time = timer_read();
                    upshiftcode = keycode;
                    tap_code16(LSFT(keycode));
                    return false;
                }
                return true;
            }
    }
    return true;
}


void oled_task_user(void) {
    //oled_clear();
    // // Host Keyboard Layer Status
    // static const char PROGMEM qmk_logo[] = {
    //     0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
    //     0x90,0x91,0x92,0x93,0x94,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,
    //     0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,
    //     0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0x00,
    // };
     //   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    uint8_t layer = biton(layer_state);
    char buf[40];
    // oled_write_P(PSTR("E1234567890-=<-\n"),true);
    // oled_write_P(PSTR(">QWERTYUIOP[] +\n"),true);
    // oled_write_P(PSTR("_ASDFGHJKL;\"\\\n"),true);
    // oled_write_P(PSTR("_`ZXCVBNM,./_^\"\\\n"),true);
    //    snprintf(buf,sizeof(buf), "B%02ld %2d:%2d\n", adafruit_ble_read_battery_voltage()/33,selbuf[1],selbuf[0]);
    //    oled_write(buf, false);
    switch (layer) {
        case 0:
        if (shifton) {
            oled_write_P(PSTR("Esc !@#$%^&*()_+ BkspTab QWERTYUIOP{} EntrCaps ASDFGHJKL:\"|    Sft ~ZXCVBNM<>? Up   C^AC _____ C Le Dn Rt"), false);
        } else {
            oled_write_P(PSTR("Esc 1234567890-= BkspTab qwertyuiop[] EntrCaps asdfghjkl;'\\    Sft `zxcvbnm,./ Up   C^AC _____ C Le Dn Rt"), false);
        }
        break;
        case 1:
            oled_write_P(PSTR("Esc F1-F6  F7-F12 Del                                                        PgUp   __^_ __ Home PgDn End"), false);
        break;
        case 2:
            oled_write_P(PSTR("Reset L+- Media-+              USB                                      BT          ____                 "), false);
        break;
    }
    if (shifton || led_usb_state & (1<<USB_LED_CAPS_LOCK) ) {
        oled_write_P(PSTR(""), false);
    } else {
        oled_write_P(PSTR(""), false);
    }
    oled_write_P(layer ? PSTR("FN") : PSTR("    "), false);
    if (layer) {
        snprintf(buf,sizeof(buf), "%d ", layer);
        oled_write(buf, false);
    }
    oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CapsLock\n") : PSTR("\n"), false);
    switch (USB_DeviceState) {
        case DEVICE_STATE_Unattached:
            oled_write_P(PSTR(" n/a "), false);
            break;
        case DEVICE_STATE_Suspended:
            oled_write_P(PSTR(" -  "), false);
            break;
        case DEVICE_STATE_Configured:
            oled_write_P(PSTR(" USB "), false);
            break;
        case DEVICE_STATE_Powered:
            oled_write_P(PSTR("Chrg "), false);
            break;
        case DEVICE_STATE_Default:
            oled_write_P(PSTR(" def "), false);
            break;
        case DEVICE_STATE_Addressed:
            oled_write_P(PSTR(" usb "), false);
            break;
        default:
            oled_write_P(PSTR(" ?  "), false);
    }
    oled_write_P(where_to_send() == OUTPUT_BLUETOOTH ? PSTR("B "):PSTR("U "), false);
    oled_write_P(adafruit_ble_is_connected() ? PSTR("BlueTooth\n"):PSTR("\n"), false);
    snprintf(buf,sizeof(buf),"Backlt: %02d Batt: %02ld\n", get_backlight_level(), adafruit_ble_read_battery_voltage()/10);
    oled_write(buf, false);
    oled_render();
}

void led_set_kb(uint8_t usb_led) {
	if (1 << USB_LED_CAPS_LOCK) {
		expander_select(0xE);
	}
}
