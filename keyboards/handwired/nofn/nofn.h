#ifndef NOFN_H
#define NOFN_H

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#define I2C_ADDR        0b0100000
#define I2C_ADDR_WRITE  ( (I2C_ADDR<<1) | I2C_WRITE )
#define I2C_ADDR_READ   ( (I2C_ADDR<<1) | I2C_READ  )
#define IODIRA          0x00            // i/o direction register
#define IODIRB          0x01
#define GPPUA           0x0C            // GPIO pull-up resistor register
#define GPPUB           0x0D
#define GPIOA           0x12            // general purpose i/o port register (write modifies OLAT)
#define GPIOB           0x13
#define OLATA           0x14            // output latch register
#define OLATB           0x15

extern uint8_t expander_status;
extern uint8_t expander_input_pin_mask;
extern bool i2c_initialized;

void init_expander(void);

#define KEYMAP( \
	  K000, K001,  K002,  K003,  K004,  K005,  K006,      K007,  K008,  K009,  K010,  K011, K012,  K013,       \
	  K100,        K102,  K103,  K104,  K105,  K106,      K107,  K108,  K109,  K110,  K111, K112,  K113, K114, \
	  K200,        K202,  K203,  K204,  K205,  K206,      K207,  K208,  K209,  K210,  K211, K212,  K213,       \
	  K300, K301,  K302,  K303,  K304,  K305,  K306,      K307,  K308,  K309,  K310,  K311,        K313,       \
	  K400, K401,  K402,  K403,                           K407,                       K411, K412,  K413, K414  \
) { \
	{ K000, K001,  K002,  K003,  K004,  K005,  K006,      K007,  K008,  K009,  K010,  K011, K012,  K013, KC_NO }, \
	{ K100, KC_NO, K102,  K103,  K104,  K105,  K106,      K107,  K108,  K109,  K110,  K111, K112,  K113, K114  }, \
	{ K200, KC_NO, K202,  K203,  K204,  K205,  K206,      K207,  K208,  K209,  K210,  K211, K212,  K213, KC_NO }, \
	{ K300, K301,  K302,  K303,  K304,  K305,  K306,      K307,  K308,  K309,  K310,  K311, KC_NO, K313, KC_NO }, \
	{ K400, K401,  K402,  K403,  KC_NO, KC_NO, KC_NO,     K407,  KC_NO, KC_NO, KC_NO, K411, K412,  K413, K414  }  \
}

#endif
