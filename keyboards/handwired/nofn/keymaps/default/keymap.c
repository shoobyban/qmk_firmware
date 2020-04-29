#include QMK_KEYBOARD_H
#include "outputselect.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Qwerty
   * +-----------------------------------------+-----+             +---------------------------------------------------------+
   * | ESC  |   1  |   2  |   3  |   4  |   5  |  6  |             |   7  |   8  |   9  |   O  |   -  |   +  | Bksp          |
   * +------+-+----+------+------+------+------+--+--+          +--+------+------+------+------+------+------+---------------|
   * | TAB    |   Q  |   W  |   E  |   R  |   T   |             |   Y  |   U  |   I  |   O  |   P  |  [   |   ]  |   Enter   |
   * +--------+--+---+--+---+--+---+--+---+--+----+-+           +-+----+------+------+------+------+------+------|___        |
   * | Caps Lock |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |   |    |       |
   * +-------+--+----+-+---+--+---+--+---+--+------+---+          +--+------+------+------+------+------+------+------+      |
   * | Shift |   ~  |   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  |      | Up   |      |
   * +------++-----++----+-+-----++------+------+------+-----------+ +------+------+------+------+------+------+------+------+
   * | Ctrl |  FN  | Alt |  Cmd  |             |      Space        |                    |   Cmd  | Alt  | Left | Down | Right|
   * +------+------+-----+-------+             +-------------------+                    +------------------------------------+
   */
	[0] = KEYMAP(
		KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,         KC_7, 	 KC_8,    KC_9,     KC_0,   KC_MINS, KC_EQL,  KC_BSPACE,
		KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y, 	 KC_U,    KC_I,     KC_O,   KC_P,    KC_LBRC, KC_RBRC, KC_ENT,
		KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H, 	 KC_J,    KC_K,     KC_L,   KC_SCLN, KC_QUOT, KC_BSLS,
		KC_LSFT, KC_GRV,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N, 	 KC_M,    KC_COMM,          KC_DOT,  KC_SLSH,  KC_UP,
		KC_LCTL, MO(1),   KC_LALT, KC_LGUI,                    				KC_SPC,                             KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT),

  /* FN
   * +-----------------------------------------+-----+             +---------------------------------------------------------+
   * | ESC  |  F1  |  F2  |  F3  |  F4  |  F5  | F6  |             |  F7  |  F8  |  F9  |  F1O | F11  | F12  | Delete        |
   * +------+-+----+------+------+------+------+--+--+          +--+------+------+------+------+------+------+---------------|
   * | TAB    |   Q  |   W  |   E  |   R  |   T   |             |   Y  |   U  |   I  |   O  |   P  |  [   |   ]  |   Enter   |
   * +--------+--+---+--+---+--+---+--+---+--+----+-+           +-+----+------+------+------+------+------+------|___        |
   * | Caps Lock |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |   |    |       |
   * +-------+--+----+-+---+--+---+--+---+--+------+---+          +--+------+------+------+------+------+------+------+      |
   * | Shift |   ~  |   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  |      | PgUp |      |
   * +------++-----++----+-+-----++------+------+------+-----------+ +------+------+------+------+------+------+------+------+
   * | Ctrl |      | FN2 |  Cmd  |             |      Space        |                    |   Cmd  | Alt  | Left | PgDn | Right|
   * +------+------+-----+-------+             +-------------------+                    +------------------------------------+
   */
	[1] = KEYMAP(
		KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
		KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_PGUP,
		KC_TRNS, KC_TRNS, MO(2),   KC_TRNS,                                 KC_TRNS,                            KC_TRNS, KC_TRNS, KC_PGDN, KC_TRNS),

  /* COMMAND
   * +-----------------------------------------+-----+             +---------------------------------------------------------+
   * | RESET | BRDN  | BRUP |  F3  |  F4 | BDN | BUP |             | REW  | PLAY | FWD  |  F1O | BLON | BLOFF | Delete       |
   * +------+-+----+------+------+------+------+--+--+          +--+------+------+------+------+------+------+---------------|
   * | TAB    |   Q  |   W  |   E  |   R  |   T   |             |   Y  |   U  |   I  |   O  |   P  |  [   |   ]  |   Enter   |
   * +--------+--+---+--+---+--+---+--+---+--+----+-+           +-+----+------+------+------+------+------+------|___        |
   * | Caps Lock |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |   |    |       |
   * +-------+--+----+-+---+--+---+--+---+--+------+---+          +--+------+------+------+------+------+------+------+      |
   * | Shift |   ~  |   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  |      | PgUp |      |
   * +------++-----++----+-+-----++------+------+------+-----------+ +------+------+------+------+------+------+------+------+
   * | Ctrl |  FN  |     |  Cmd  |             |      Space        |                    |   Cmd  | Alt  | Left | PgDn | Right|
   * +------+------+-----+-------+             +-------------------+                    +------------------------------------+
   */
	[2] = KEYMAP(
		RESET,   KC_BRMD, KC_BRMU, KC_TRNS, KC_TRNS, BL_DEC,  BL_INC,       KC_MRWD, KC_MPLY, KC_MFFD, KC_TRNS, BL_ON,   BL_OFF,  KC_BSPC,
		KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                 KC_TRNS, 						    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};
