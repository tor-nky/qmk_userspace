/* Copyright 2021 daraku-neko
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
// #include "twpair_on_jis.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _QWERTY,
  _NUMPAD,
  _LOWER,
  _RAISE,
  _ADJUST,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    US_KEY = SAFE_RANGE,
    US2JIS,
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)
#define NUMPAD TG(_NUMPAD)
// [_QWERTY]
#define OP_ESC LALT_T(KC_ESC)
#define CTL_TAB LCTL_T(KC_TAB)
#define CM_EISU LGUI_T(KC_LNG2)
#define LOW_BS LT(_LOWER, KC_BSPC)
#define LS_SPC LSFT_T(KC_SPC)
#define RS_SPC RSFT_T(KC_SPC)
#define RAI_ENT LT(_RAISE, KC_ENT)
#define OP_KANA RALT_T(KC_LNG1)
#define SFT_DEL RSFT_T(KC_DEL)
// [_LOWER]
#define ADJ_ENT LT(_ADJUST, KC_PENT)
// [_RAISE]
#define CM_LBRC LGUI(KC_LBRC)
#define CM_RBRC LGUI(KC_RBRC)
#define WSHOOT  SGUI(KC_5)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
       OP_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_MINS, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      CTL_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,           KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, SFT_DEL,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      KC_CAPS,                   CM_EISU,  LOW_BS,  KC_SPC,         KC_SPC, RAI_ENT, OP_KANA,                    NUMPAD
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),

  [_LOWER] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
       KC_GRV, KC_CIRC, KC_RBRC, KC_AMPR, KC_LBRC, KC_BSLS,        KC_COLN,    KC_7,    KC_8,    KC_9, KC_PSLS, KC_PMNS, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______, KC_EXLM,   KC_AT, KC_PIPE, KC_LPRN, KC_RPRN,         KC_EQL,    KC_4,    KC_5,    KC_6, KC_PPLS, KC_PAST, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______, KC_TILD,  KC_DLR, KC_HASH, KC_LCBR, KC_RCBR,           KC_0,    KC_1,    KC_2,    KC_3, KC_PDOT, KC_PERC,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      _______,                   _______, _______, _______,        _______, ADJ_ENT, _______,                   _______
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),

  [_RAISE] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
       KC_PWR,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_BTN1, KC_BTN2, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,        KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_BTN3,  WSHOOT, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,  KC_F11,  KC_F12, KC_PSCR, KC_SCRL, KC_PAUS,        KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_WH_D, KC_WH_U,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      _______,                   _______,  ADJUST, _______,        _______, _______, _______,                   _______
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),

  [_ADJUST] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  US2JIS,   NK_ON, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, KC_VOLU, KC_BRIU,        XXXXXXX,  QK_RBT, XXXXXXX, XXXXXXX,  US_KEY,  NK_OFF, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      RM_NEXT, RM_HUED, RM_SATD, RM_VALD, KC_VOLD, KC_BRID,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      XXXXXXX,                   _______, _______, _______,        _______, _______, _______,                   XXXXXXX
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),

    [_NUMPAD] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
      _______, _______, _______, _______, _______, _______,        KC_COLN,   KC_P7,   KC_P8,   KC_P9, KC_PSLS, KC_PMNS, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,        KC_PEQL,   KC_P4,   KC_P5,   KC_P6, KC_PPLS, KC_PAST, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,          KC_P0,   KC_P1,   KC_P2,   KC_P3, KC_PDOT, KC_PERC,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      _______,                   _______, _______, _______,        _______, _______, KC_PCMM,                   _______
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),
};

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   static bool is_us2jis = false;

//   switch (keycode) {
//     case US_KEY:
//       if (record->event.pressed)
//         is_us2jis = false;
//       return false;
//     case US2JIS:
//       if (record->event.pressed)
//         is_us2jis = true;
//       return false;
//   }

//   // typewriter pairing on jis keyboard
//   if (is_us2jis && !twpair_on_jis(keycode, record))
//     return false;

//   return true;
// }
