/* Copyright 2020 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    CUSTOM_KEYCODE1 = SAFE_RANGE,
    EISU,
    KANA,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] =  LAYOUT(
     A(KC_PSCR),   KC_F1,  KC_F2,  KC_F3,  KC_F4,    KC_F5,  KC_F6,  KC_F7,  KC_F8,    KC_F9, KC_F10, KC_F11, KC_F12,  C(KC_X),C(KC_C),C(KC_V),
         KC_ESC,    JP_1,   JP_2,   JP_3,   JP_4,   JP_5,   JP_6,   JP_7,   JP_8,   JP_9,   JP_0,JP_MINS,JP_CIRC, JP_YEN,KC_BSPC,   KC_INS,KC_HOME,KC_PGUP,A(KC_TAB),KC_PSLS,KC_PAST,KC_PMNS,
         KC_TAB,     JP_Q,   JP_W,   JP_E,   JP_R,   JP_T,   JP_Y,   JP_U,   JP_I,   JP_O,   JP_P,  JP_AT,JP_LBRC,        KC_ENT,   KC_DEL, KC_END,KC_PGDN,    KC_P7,  KC_P8,  KC_P9,KC_PPLS,
        JP_CAPS,      JP_A,   JP_S,   JP_D,   JP_F,   JP_G,   JP_H,   JP_J,   JP_K,   JP_L,JP_SCLN,JP_COLN,JP_RBRC,                                            KC_P4,  KC_P5,  KC_P6,
        KC_LSFT,       JP_Z,   JP_X,   JP_C,   JP_V,   JP_B,   JP_N,   JP_M,JP_COMM, JP_DOT,JP_SLSH,JP_BSLS,             KC_RSFT,            KC_UP,            KC_P1,  KC_P2,  KC_P3,KC_PENT,
        KC_LCTL,KC_LGUI,   KC_LALT,       EISU,          KC_SPC,       JP_HENK,      KANA,   KC_RALT,  KC_RGUI, KC_APP,  KC_RCTL,  KC_LEFT,KC_DOWN,KC_RGHT,    KC_P0,        KC_PDOT
    )
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case EISU:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_INT2)SS_TAP(X_GRV));
        }
        break;
    case KANA:
        if (record->event.pressed) {
            SEND_STRING(SS_TAP(X_INT2)SS_TAP(X_INT2));
        }
        break;
    }
    return true;
}

#ifdef OLED_ENABLE
#    include "rev2.h"
#    include "oled_driver.h"

bool oled_task_user(void) {
    render_logo();
    return false;
}
#endif
