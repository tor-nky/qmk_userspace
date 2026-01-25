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

#ifdef OS_DETECTION_ENABLE
#   include "os_detection.h"
#endif

// 薙刀式 begin 1
#include "naginata.h"
// 薙刀式 end 1
#include "twpair_on_jis.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _QWERTY,
// 薙刀式 begin 2
  _NAGINATA, // 薙刀式入力レイヤー
// 薙刀式 end 2
  _NUMPAD,
  _LOWER,
  _RAISE,
  _ADJUST,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    US_KEY = NG_SAFE_RANGE,
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
      KC_CAPS,                   KC_LGUI,  LOW_BS,  LS_SPC,         RS_SPC, RAI_ENT, KC_LALT,                    NUMPAD
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
       KC_PWR,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        MS_LEFT, MS_DOWN,   MS_UP, MS_RGHT, MS_BTN1, MS_BTN2, _______,
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
      RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, KC_VOLU, KC_BRIU,        XXXXXXX,  QK_RBT, NG_KOTI, XXXXXXX,  US_KEY,  NK_OFF, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      RM_NEXT, RM_HUED, RM_SATD, RM_VALD, KC_VOLD, KC_BRID,       NGSW_WIN,NGSW_MAC,NGSW_LNX, NG_TAYO, NG_SHOS, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      XXXXXXX,                   XXXXXXX, _______, XXXXXXX,        XXXXXXX, _______, XXXXXXX,                   XXXXXXX
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

// 薙刀式 begin 3
  [_NAGINATA] = LAYOUT(
  //,-----------------------------------------------------.      ,--------------------------------------------------------------.
      _______,    NG_Q,    NG_W,    NG_E,    NG_R,    NG_T,           NG_Y,    NG_U,    NG_I,    NG_O,    NG_P, _______,  KC_OUT,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,    NG_A,    NG_S,    NG_D,    NG_F,    NG_G,           NG_H,    NG_J,    NG_K,    NG_L, NG_SCLN, _______, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------+--------|
      _______,    NG_Z,    NG_X,    NG_C,    NG_V,    NG_B,           NG_N,    NG_M, NG_COMM,  NG_DOT, NG_SLSH, _______,
  //|--------+--------+--------+--------+--------+--------|      |--------+--------+--------+--------+--------+--------|
      _______,                   _______, _______, NG_SHFT,        NG_SHFT, _______, _______,                   _______
  //`--------'                 `--------+--------+--------'      `--------------------------'                 `--------'
  ),
// 薙刀式 end 3
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static bool is_us2jis = true;
#ifdef CONSOLE_ENABLE
  const uint16_t key_timer = timer_read();  // 時間測定開始;
#endif
  const bool pressed = record->event.pressed;
  bool cont = true;

  switch (keycode) {
    case US_KEY:
      if (pressed) {
        is_us2jis = false;
      }
      cont = false;
      break;
    case US2JIS:
      if (pressed) {
        is_us2jis = true;
      }
      cont = false;
      break;
    case KC_PEQL:
      if (naginata_config.os == NG_MAC)
        break;
      if (pressed) {
        if (is_us2jis)
          tap_code16(LSFT(KC_MINS));
        else
          tap_code(KC_EQL);
      }
      cont = false;
      break;
    case KC_PCMM:
      if (naginata_config.os == NG_MAC && is_us2jis)
        break;
      // JISキーボード設定のMacでないなら、「,」を出力する
      if (pressed) {
        tap_code(KC_COMM);
      }
      cont = false;
      break;
#ifdef CONSOLE_ENABLE
    case KC_OUT:
      if (pressed) {
        keyrecord_t release;
        release.event.pressed = false;
// こりゃ (かりょ)  // AVR12ms
        process_naginata(NG_V, record); // こ
        process_naginata(NG_H, record); // りゃ
        process_naginata(NG_E, record); // AVR12ms
        process_naginata(NG_V, &release);
        process_naginata(NG_H, &release);
        process_naginata(NG_E, &release);
// ある情報 // AVR23ms
        // process_naginata(NG_J, record); // あ
        // process_naginata(NG_I, record); // る
        // process_naginata(NG_I, &release); // AVR6ms
        // process_naginata(NG_I, record); // じょ
        // process_naginata(NG_R, record); // AVR6ms
        // process_naginata(NG_L, record); // う
        // process_naginata(NG_R, &release);
        // process_naginata(NG_I, &release);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_L, &release);
        // process_naginata(NG_Z, record); // ほ
        // process_naginata(NG_L, record); // う // AVR5ms
        // process_naginata(NG_Z, &release);
        // process_naginata(NG_L, &release);
// 姶良カルデラ // AVR29ms
        // process_naginata(NG_J, record); // あ
        // process_naginata(NG_K, record); // い
        // process_naginata(NG_DOT, record); // ら // AVR6ms
        // process_naginata(NG_F, record);  // か
        // process_naginata(NG_J, &release);
        // process_naginata(NG_K, &release);
        // process_naginata(NG_DOT, &release); // AVR4ms
        // process_naginata(NG_I, record); // る
        // process_naginata(NG_J, record); // で // AVR6ms
        // process_naginata(NG_E, record); // AVR6ms
        // process_naginata(NG_DOT, record); // ら // AVR5ms
        // // キー上げ
        // process_naginata(NG_E, &release);
        // process_naginata(NG_F, &release);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_I, &release);
        // process_naginata(NG_DOT, &release);  // AVR3ms
// 漁夫の利 // AVR20ms
        // process_naginata(NG_I, record); // ぎょ
        // process_naginata(NG_W, record);
        // process_naginata(NG_J, record);  // AVR6ms
        // process_naginata(NG_SHFT, record);  // ふ
        // process_naginata(NG_SCLN, record);  // AVR5ms
        // process_naginata(NG_J, &release);    // の
        // process_naginata(NG_J, record); // AVR5ms
        // process_naginata(NG_E, record);  // り  // AVR5ms
        // // キー上げ
        // process_naginata(NG_I, &release);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_W, &release);
        // process_naginata(NG_SHFT, &release);
        // process_naginata(NG_SCLN, &release);
        // process_naginata(NG_E, &release);
// 近所においしいおそば屋さんがあります // AVR69ms
        // process_naginata(NG_W, record); // き
        // process_naginata(NG_W, &release);
        // process_naginata(NG_COMM, record); // ん
        // process_naginata(NG_COMM, &release);
        // process_naginata(NG_R, record); // じょ
        // process_naginata(NG_J, record);
        // process_naginata(NG_I, record);
        // process_naginata(NG_I, &release);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_R, &release);
        // process_naginata(NG_SHFT, record);  // に
        // process_naginata(NG_D, record);
        // process_naginata(NG_D, &release);
        // process_naginata(NG_N, record); // お
        // process_naginata(NG_N, &release);
        // process_naginata(NG_SHFT, &release);
        // process_naginata(NG_K, record); // い
        // process_naginata(NG_K, &release);
        // process_naginata(NG_R, record); // し
        // process_naginata(NG_R, &release);
        // process_naginata(NG_K, record); // い
        // process_naginata(NG_K, &release);
        // process_naginata(NG_SHFT, record);  // お
        // process_naginata(NG_N, record);
        // process_naginata(NG_N, &release);
        // process_naginata(NG_SHFT, &release);
        // process_naginata(NG_B, record); // そ
        // process_naginata(NG_B, &release);
        // process_naginata(NG_C, record); // ば
        // process_naginata(NG_J, record);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_C, &release);
        // process_naginata(NG_SHFT, record);  // や
        // process_naginata(NG_H, record);
        // process_naginata(NG_H, &release);
        // process_naginata(NG_U, record); // さ
        // process_naginata(NG_U, &release);
        // process_naginata(NG_SHFT, &release);
        // process_naginata(NG_COMM, record);  // ん
        // process_naginata(NG_COMM, &release);
        // process_naginata(NG_J, record); // が
        // process_naginata(NG_F, record);
        // process_naginata(NG_J, &release);
        // process_naginata(NG_F, &release);
        // process_naginata(NG_J, record); // あ
        // process_naginata(NG_J, &release);
        // process_naginata(NG_SHFT, record);  // り
        // process_naginata(NG_E, record);
        // process_naginata(NG_E, &release);
        // process_naginata(NG_F, record); // ま
        // process_naginata(NG_F, &release);
        // process_naginata(NG_SHFT, &release);
        // process_naginata(NG_O, record); // す
        // process_naginata(NG_O, &release);
      }
      cont = false;
#endif  // CONSOLE_ENABLE
  }

  // 薙刀式 begin 4
  if (cont) {
    cont = process_naginata(keycode, record);
  }
  // 薙刀式 end 4

  // typewriter pairing on jis keyboard
  if (cont && is_us2jis) {
    cont = twpair_on_jis(keycode, record);
  }

#ifdef CONSOLE_ENABLE
  uprintf("%c%ums\n", pressed ? '*' : ' ', timer_elapsed(key_timer));  // 経過時間表示
#endif
  return cont;
}

#ifdef CONSOLE_ENABLE
void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  // debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
#endif

// 全ての QMK 処理の最後に、次の繰り返しを開始する前に呼び出される関数
void housekeeping_task_user(void) {
  // 薙刀式 begin 5
  // 後置シフト待ち処理
  kouchi_shift_loop();
  // 薙刀式 end 5
}

void matrix_init_user(void) {
  // 薙刀式 begin 6
  uint16_t ngonkeys[] = {KC_H, KC_J};
  uint16_t ngoffkeys[] = {KC_F, KC_G};
  set_naginata(_NAGINATA, ngonkeys, ngoffkeys);

  // 自動でOSによってレイヤーや薙刀式の設定を切り替える
#ifdef OS_DETECTION_ENABLE
  wait_ms(400);
  switch (detected_host_os()) {
    case OS_WINDOWS:
      switchOS(NG_WIN);
      break;
    case OS_MACOS:
    case OS_IOS:
      switchOS(NG_MAC);
      break;
    case OS_LINUX:
      switchOS(NG_LINUX);
      break;
    default:
      break;
  }
#endif
  // 薙刀式 end 6
}
