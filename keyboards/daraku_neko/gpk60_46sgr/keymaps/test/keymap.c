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
// #include "users/naginata_v15m/twpair_on_jis.h"

#define T_C 1   // キーを同時押しするときの間隔
#define T_S 1   // スペースキーから通常キーを押すまでの間隔
#define T_R 5   // ロールオーバーの間隔
#define T_P 20  // キーを押している時間
#define T_U 1   // キーを複数上げるときの間隔
#define T_N 5   // キーを上げてから次のキーを押すまでの間隔
#define T_POL 31
#define T_IME 128

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
    SF1,
    SF2,
    SF3,
    SF4,
    SF5,
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
#define OP_KANA LALT_T(KC_LNG1)
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
       OP_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_MINS,     SF1,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      CTL_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,     SF2,
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
       KC_PWR,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        MS_LEFT, MS_DOWN,  MS_UP, MS_RGHT, MS_BTN1, MS_BTN2, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,        KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, MS_BTN3,  WSHOOT, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,  KC_F11,  KC_F12, KC_PSCR, KC_SCRL, KC_PAUS,        KC_HOME, KC_PGDN, KC_PGUP,  KC_END, MS_WHLD, MS_WHLU,
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // static bool is_us2jis = false;

  static uint_fast8_t spc_count = 0;
  if (keycode == KC_SPC) {
    if (record->event.pressed) {
      if (!spc_count)
        register_code(keycode);
      spc_count++;
    } else if (spc_count) {
      spc_count--;
      if (!spc_count)
        unregister_code(keycode);
    }
    return false;
  }

  if (record->event.pressed) {
    switch (keycode) {
      // case US_KEY:
      //   is_us2jis = false;
      //   return false;
      // case US2JIS:
      //   is_us2jis = true;
      //   return false;
      case SF1:
        // (薙刀式)近所に美味しいお蕎麦屋さんがあります
        SEND_STRING(SS_DOWN(X_W)SS_DELAY(T_P)SS_UP(X_W)                                                             // き
          SS_DELAY(T_R)SS_DOWN(X_COMM)SS_DELAY(T_P)SS_UP(X_COMM)                                                  // ん
          SS_DELAY(T_N)SS_DOWN(X_R)SS_DELAY(T_C)SS_DOWN(X_J)SS_DELAY(T_C)SS_DOWN(X_I)SS_DELAY(T_P)SS_UP(X_I)SS_DELAY(T_U)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_R)    // じょ
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_D)SS_DELAY(T_P)SS_UP(X_D)                             // に
          SS_DELAY(T_R)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N)SS_DELAY(T_U)SS_UP(X_SPC)                               // お
          SS_DELAY(T_N)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_K)                                                        // い
          SS_DELAY(T_N)SS_DOWN(X_R)SS_DELAY(T_P)SS_UP(X_R)                                                        // し
          SS_DELAY(T_R)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_K)                                                        // い
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N)SS_DELAY(T_U)SS_UP(X_SPC)    // お
          SS_DELAY(T_N)SS_DOWN(X_B)SS_DELAY(T_P)SS_UP(X_B)                                                        // そ
          SS_DELAY(T_N)SS_DOWN(X_C)SS_DELAY(T_C)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_C)        // ば
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_H)SS_DELAY(T_P)SS_UP(X_H)                             // や
          SS_DELAY(T_N)SS_DOWN(X_U)SS_DELAY(T_P)SS_UP(X_U)SS_DELAY(T_U)SS_UP(X_SPC)                               // さ
          SS_DELAY(T_N)SS_DOWN(X_COMM)SS_DELAY(T_P)SS_UP(X_COMM)                                                  // ん
          SS_DELAY(T_N)SS_DOWN(X_J)SS_DELAY(T_C)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_F)        // が
          SS_DELAY(T_N)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)                                                        // あ
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_E)SS_DELAY(T_P)SS_UP(X_E)                             // り
          SS_DELAY(T_R)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_F)SS_DELAY(T_U)SS_UP(X_SPC)                               // ま
          SS_DELAY(T_N)SS_DOWN(X_O)SS_DELAY(T_P)SS_UP(X_O));                                                      // す
        break;
      case SF2:
        // (薙刀式)窓を開けると、店の前には魔物たちが大行列を作っていた
        SEND_STRING(SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_F)SS_DELAY(T_U)SS_UP(X_SPC)       // ま
          SS_DELAY(T_N)SS_DOWN(X_D)SS_DELAY(T_C)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_D)        // ど
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_C)SS_DELAY(T_P)SS_UP(X_SPC)SS_DELAY(T_U)SS_UP(X_C)    // を
          SS_DELAY(T_N)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)                                                        // あ
          SS_DELAY(T_N)SS_DOWN(X_S)SS_DELAY(T_P)SS_UP(X_S)                                                        // け
          SS_DELAY(T_R)SS_DOWN(X_I)SS_DELAY(T_P)SS_UP(X_I)                                                        // る
          SS_DELAY(T_N)SS_DOWN(X_D)SS_DELAY(T_P)SS_UP(X_D)                                                        // と
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_V)SS_DELAY(T_P)SS_UP(X_V)                             // 、
          SS_DELAY(T_N)SS_DOWN(X_B)SS_DELAY(T_P)SS_UP(X_B)                                                        // み
          SS_DELAY(T_N)SS_DOWN(X_A)SS_DELAY(T_P)SS_UP(X_A)                                                        // せ
          SS_DELAY(T_R)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)                                                        // の
          SS_DELAY(T_R)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_F)                                                        // ま
          SS_DELAY(T_R)SS_DOWN(X_O)SS_DELAY(T_P)SS_UP(X_O)                                                        // え
          SS_DELAY(T_R)SS_DOWN(X_D)SS_DELAY(T_P)SS_UP(X_D)SS_DELAY(T_U)SS_UP(X_SPC)                               // に
          SS_DELAY(T_N)SS_DOWN(X_C)SS_DELAY(T_P)SS_UP(X_C)                                                        // は
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_F)                             // ま
          SS_DELAY(T_R)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_K)                                                        // も
          SS_DELAY(T_R)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_SPC)                               // の
          SS_DELAY(T_N)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N)                                                        // た
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_G)SS_DELAY(T_P)SS_UP(X_G)SS_DELAY(T_U)SS_UP(X_SPC)    // ち
          SS_DELAY(T_N)SS_DOWN(X_J)SS_DELAY(T_C)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_J)                               // が
          SS_DELAY(T_N)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N)SS_DELAY(T_U)SS_UP(X_F)                                 // だ
          SS_DELAY(T_N)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_K)                                                        // い
          SS_DELAY(T_N)SS_DOWN(X_W)SS_DELAY(T_C)SS_DOWN(X_I)SS_DELAY(T_C)SS_DOWN(X_J)                             // ぎょ
          SS_DELAY(T_R)SS_DOWN(X_L)                                                                               // う
          SS_DELAY(T_R)SS_DOWN(X_SLSH)SS_DELAY(T_P)SS_UP(X_SLSH)SS_DELAY(T_U)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_W)
            SS_DELAY(T_U)SS_UP(X_L)SS_DELAY(T_U)SS_UP(X_I)                                                        // れ
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_L)SS_DELAY(T_P)SS_UP(X_L)                             // つ
          SS_DELAY(T_N)SS_DOWN(X_C)SS_DELAY(T_P)SS_UP(X_C)                                                        // を
          SS_DELAY(T_N)SS_DOWN(X_L)SS_DELAY(T_P)SS_UP(X_SPC)SS_DELAY(T_U)SS_UP(X_L)                               // つ
          SS_DELAY(T_N)SS_DOWN(X_H)SS_DELAY(T_P)SS_UP(X_H)                                                        // く
          SS_DELAY(T_N)SS_DOWN(X_G)SS_DELAY(T_P)SS_UP(X_G)                                                        // っ
          SS_DELAY(T_N)SS_DOWN(X_E)                                                                               // て
          SS_DELAY(T_R)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_E)SS_DELAY(T_U)SS_UP(X_K)                                 // い
          SS_DELAY(T_N)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N));                                                      // た
        break;
      case SF3:
        // (薙刀式)おはようございます
        SEND_STRING(SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_N)SS_DELAY(T_P)SS_UP(X_N)SS_DELAY(T_U)SS_UP(X_SPC)         // お
          SS_DELAY(T_N)SS_DOWN(X_C)SS_DELAY(T_P)SS_UP(X_C)                                                        // は
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_I)SS_DELAY(T_P)SS_UP(X_I)SS_DELAY(T_U)SS_UP(X_SPC)    // よ
          SS_DELAY(T_N)SS_DOWN(X_L)SS_DELAY(T_P)SS_UP(X_L)                                                        // う
          SS_DELAY(T_N)SS_DOWN(X_V)SS_DELAY(T_C)SS_DOWN(X_J)SS_DELAY(T_P)SS_UP(X_J)SS_DELAY(T_U)SS_UP(X_V)        // ご
          SS_DELAY(T_N)SS_DOWN(X_F)SS_DELAY(T_C)SS_DOWN(X_U)SS_DELAY(T_P)SS_UP(X_U)SS_DELAY(T_U)SS_UP(X_F)        // ざ
          SS_DELAY(T_N)SS_DOWN(X_K)SS_DELAY(T_P)SS_UP(X_K)                                                        // い
          SS_DELAY(T_N)SS_DOWN(X_SPC)SS_DELAY(T_S)SS_DOWN(X_F)SS_DELAY(T_P)SS_UP(X_F)SS_DELAY(T_U)SS_UP(X_SPC)    // ま
          SS_DELAY(T_N)SS_DOWN(X_O)SS_DELAY(T_P)SS_UP(X_O));                                                      // す
        break;
      case SF4:
        tap_code(KC_GRV); // 半角/全角
        tap_code16(C(S(KC_U)));
        tap_code(KC_3); wait_ms(T_POL); // 【
        tap_code(KC_0); wait_ms(T_POL);
        tap_code(KC_1); wait_ms(T_POL);
        tap_code(KC_0); wait_ms(T_POL);
        tap_code(KC_SPC); wait_ms(T_POL);
        tap_code16(C(S(KC_U)));
        tap_code(KC_3); wait_ms(T_POL); // 】
        tap_code(KC_0); wait_ms(T_POL);
        tap_code(KC_1); wait_ms(T_POL);
        tap_code(KC_1); wait_ms(T_POL);
        tap_code(KC_SPC); wait_ms(T_POL);
        tap_code(KC_INTERNATIONAL_2); // ひらがな
        tap_code(KC_LEFT);  //左キー
        break;
      case SF5:
        for (int i = 0; i < 10; i++) {
          for (int j = 0; j < 20; j++) {
            tap_code(KC_J); wait_ms(T_IME);
            tap_code(KC_K); wait_ms(T_IME);
            tap_code(KC_L); wait_ms(T_IME);
            register_code(KC_SPC);
            tap_code(KC_O); wait_ms(T_IME);
            tap_code(KC_N);
            unregister_code(KC_SPC);  wait_ms(T_IME);
            register_code(KC_V);
            tap_code(KC_M);
            unregister_code(KC_V);  wait_ms(T_IME);
          }
          register_code(KC_V);
          tap_code(KC_M);
          unregister_code(KC_V);  wait_ms(T_IME);
        }
        break;
    }

  }

//   // typewriter pairing on jis keyboard
//   if (is_us2jis && !twpair_on_jis(keycode, record))
//     return false;

  return true;
}
