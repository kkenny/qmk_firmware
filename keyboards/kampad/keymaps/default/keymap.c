/* MIT License

Copyright (c) 2019 Mattia Dal Ben

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
/*
enum layers {
    _BL,
    _DRE,
    _DRC,
    _FN
};
*/

#define _BL   0
#define	_DRE  1
#define	_DRC  2
#define _FN   3


uint8_t prev = _BL;
uint32_t desiredmode = 4;
uint16_t hue = 148;
uint16_t sat = 200;
uint16_t val = 255;

// Does something to grab the RGB LED values to retain when switching layers.

void get_hsv(void) {
   hue = rgblight_get_hue();
   sat = rgblight_get_sat();
   val = rgblight_get_val();
}

void reset_hsv(void) {
   rgblight_sethsv(hue, sat, val);
}

void matrix_init_user() {
   //rgblight_mode(desiredmode);
   //reset_hsv();
   rgblight_enable();
   rgblight_mode(4);
   rgblight_sethsv(148, 200, 255);
}


/*
enum custom_keycodes {
    KC_DBL0 = SAFE_RANGE,
};
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap _BL: (Base Layer) Default Layer
 * ,-------------------.
 * | NV | /  | *  |-/FN|
 * |----|----|----|----|
 * | 7  | 8  | 9  |    |
 * |----|----|----| +  |
 * | 4  | 5  | 6  |    |
 * |----|----|----|----|
 * | 1  | 2  | 3  |    |
 * |----|----|----| En |
 * | 0  | 00 | .  |    |
 * `-------------------'
 */
  [_BL] = KAM_LAYOUT(
    TO(_DRE),	KC__VOLUP,    KC__VOLDOWN,  KC_ESC,
    C(G(KC_Q)),	KC_MPRV,	      KC_MPLY,	    KC_MNXT,
    LCMD(KC_S),	LCMD(KC_SPC), LCMD(KC_L),   LCMD(KC_EQUAL),
    LCMD(KC_X),	LCMD(KC_C),   LCMD(KC_V),   LCMD(KC_MINUS)
  ),

  [_DRE] = KAM_LAYOUT(
    TO(_DRC),	      _______,      _______,	LSFT(KC_4),
    KC_BSPACE,	      KC_COMMA,     KC_DOT,	LCMD(KC_S),
    LCMD(KC_BSLASH),  LSFT(KC_DEL), LCMD(KC_T),	_______,
    KC_J,	      KC_L,	    KC_SPC,	_______
  ),

  [_DRC] = KAM_LAYOUT(
    TO(_FN),	_______,    _______,  LSFT(KC_6),
    _______,	XXXXXXX,    XXXXXXX,  XXXXXXX,
    SCMD(KC_W),	XXXXXXX,    XXXXXXX,  _______,
    LSA(KC_S),	LALT(KC_S), KC_J,     KC_SPC
  ),

  [_FN] = KAM_LAYOUT(
    TO(_BL),  XXXXXXX,  RGB_TOG,  RGB_MOD,
    XXXXXXX,  XXXXXXX,  RGB_MODE_KNIGHT,  RESET,
    RGB_SAD,  RGB_SAI,  XXXXXXX,  XXXXXXX,
    RGB_VAD,  RGB_VAI,  XXXXXXX,  XXXXXXX
  ),
/*
  [_FN] = KAM_LAYOUT(
    TO(_BL),  RGB_M_P,  RGB_TOG,  RGB_MOD,
    RGB_HUD,  RGB_HUI,  RGB_MODE_KNIGHT,  RESET,
    RGB_SAD,  RGB_SAI,  XXXXXXX,  XXXXXXX,
    RGB_VAD,  RGB_VAI,  XXXXXXX,  XXXXXXX
  ),
*/
};

/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_DBL0:
      if (record->event.pressed) {
        SEND_STRING("00");
      } else {
        // when keycode KC_DBL0 is released
      }
      break;

  }
  return true;
};
*/

#ifdef OLED_DRIVER_ENABLE

/*
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 270 degrees
}
*/

void oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer"), false);
  switch (get_highest_layer(layer_state)) {
    case _BL:
      oled_write_ln_P(PSTR(" BASE"), false);
      break;
    case _DRE:
      oled_write_ln_P(PSTR(" EDIT"), false);
      break;
    case _DRC:
      oled_write_ln_P(PSTR(" COLOR"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR(" FUNCTION"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR(" KAMPAD"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Stats"), false);
  oled_write_P(led_state.num_lock ? PSTR("num:*") : PSTR("num:."), false);
  oled_write_P(led_state.caps_lock ? PSTR("cap:*") : PSTR("cap:."), false);
  oled_write_P(led_state.scroll_lock ? PSTR("scr:*") : PSTR("scr:."), false);

  // Host Keyboard RGB backlight status
  oled_write_P(PSTR("-----"), false);
  oled_write_P(PSTR("Light"), false);

  static char led_buf[30];
  snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\n",
      rgblight_is_enabled() ? '*' : '.', (uint8_t)rgblight_get_mode(),
      (uint8_t)(rgblight_get_hue() / RGBLIGHT_HUE_STEP),
      (uint8_t)(rgblight_get_sat() / RGBLIGHT_SAT_STEP),
      (uint8_t)(rgblight_get_val() / RGBLIGHT_VAL_STEP));
  oled_write(led_buf, false);
}
#endif

// Section to change RGB LED colours and animation when activating layer 1 and 2, and then go back to the default, which I can change on the fly.
// sethsv(h,s,v)

uint32_t layer_state_set_user(uint32_t state)
{
  uint8_t layer = biton32(state);
  //if (prev!=_FN) {
      switch (layer) {
        case _BL:
          rgblight_mode(4);
          rgblight_sethsv(148, 200, 255);
//          reset_hsv();
          break;
        case _DRE:
          rgblight_mode(2);
          rgblight_sethsv(1, 255, 255);
          break;
        case _DRC:
          rgblight_mode(13);
	  reset_hsv();
          break;
        case _FN:
          rgblight_mode(38);
          reset_hsv();
          break;
      }
//  } else {
//      desiredmode = rgblight_get_mode();
//      get_hsv();
//  }
  prev = layer;
  return state;
}
