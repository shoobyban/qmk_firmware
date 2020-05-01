#include "nofn.h"
#include "expander.h"
#include <stdio.h>
#include "adafruit_ble.h"
#include "backlight.h"


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
    char buf[20];
    // oled_write_P(PSTR("E1234567890-=<-\n"),true);
    // oled_write_P(PSTR(">QWERTYUIOP[] +\n"),true);
    // oled_write_P(PSTR("_ASDFGHJKL;\"\\\n"),true);
    // oled_write_P(PSTR("_`ZXCVBNM,./_^\"\\\n"),true);
//    snprintf(buf,sizeof(buf), "B%02ld %2d:%2d\n", adafruit_ble_read_battery_voltage()/33,selbuf[1],selbuf[0]);
//    oled_write(buf, false);
switch (layer) {
    case 0:
    oled_write_P(PSTR("Esc 1234567890-= Bsp\nTab qwertyuiop[] Ent\nCaps ASDFGHJKL;'\\\nSf `ZXCVBNM,./ Up\nCFFAC ___ C Le Dn Rt\n"), false);
    break;
    case 1:
    oled_write_P(PSTR("Esc F1-F6 F7-F12 Del\nTab qwertyuiop[] Ent\nCaps ASDFGHJKL;'\\\nSf `ZXCVBNM,./ Up\nCFFAC ___ C Le Dn Rt\n"), false);
    break;
    case 2:
    oled_write_P(PSTR("Reset L+-  Media Del\nTab qwertyuiop[] Ent\nCaps ASDFGHJKL;'\\\nSf `ZXCVBNM,./ Up\nCFFAC ___ C Le Dn Rt\n"), false);
    break;
}
    oled_write_P(layer ? PSTR(" FN") : PSTR("     "), false);
    if (layer) {
        snprintf(buf,sizeof(buf), "%d ", layer);
        oled_write(buf, false);
    }
    oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CapsLock\n") : PSTR("\n"), false);
    oled_write_P(adafruit_ble_is_connected() ? PSTR("Connected\n") : PSTR("\n"), false);
    snprintf(buf,sizeof(buf),"Backlight: %02d\n", get_backlight_level());
    oled_write(buf, false);
    oled_render();
}

void led_set_kb(uint8_t usb_led) {
	if (1 << USB_LED_CAPS_LOCK) {
		expander_select(0xE);
	}
}
