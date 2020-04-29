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
uint8_t upshiftcode = 0;

static void unselect_rows(void);
static void select_row(uint8_t row);

void matrix_init_custom(void) {
    // TODO: initialize hardware here
    expander_init();
    // initialize row and col
    unselect_rows();
}

void matrix_init_kb(void) {

    matrix_init_user();

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

        if (l_time && (timer_elapsed(l_time) > 200)) {
            changed = true;
            l_time = timer_read();
            if (!upshiftcode) {
                tap_code(KC_UP);
            } else {
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
        case KC_UP:
            if (record->event.pressed) {
                l_time = timer_read(); // init timer
            } else {
                if (l_time) { // only if not combo
                    l_time = 0;
                    if (!upshiftcode) {
                        tap_code(KC_UP);
                    }
                    upshiftcode = 0;
                }
            }
            return false;
            break;
        default:
            if (record->event.pressed) {
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
