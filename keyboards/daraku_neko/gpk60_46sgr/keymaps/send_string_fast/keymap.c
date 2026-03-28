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
#include "send_string_fast.h"

// ●テスト用キー
//
// KC_3    に                          in_dead を
// KC_DLR  に is_shifted,              in_dead を
// KC_5    に              is_altgred, in_dead を
// あえて設定している


// const uint8_t ascii_to_shift_lut[16] PROGMEM = {
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),

//     KCLUT_ENTRY(0, 1, 1, 1, 1, 1, 1, 0),
//     KCLUT_ENTRY(1, 1, 1, 1, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 1, 0, 1, 0, 1, 1),
//     KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
//     KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
//     KCLUT_ENTRY(1, 1, 1, 1, 1, 1, 1, 1),
//     KCLUT_ENTRY(1, 1, 1, 0, 0, 0, 1, 1),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
//     KCLUT_ENTRY(0, 0, 0, 1, 1, 1, 1, 0)
// };

const uint8_t ascii_to_altgr_lut[16] PROGMEM = {
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),

    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 1, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0)
};

const uint8_t ascii_to_dead_lut[16] PROGMEM = {
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),

    KCLUT_ENTRY(0, 0, 0, 0, 1, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 1, 0, 1, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0),
    KCLUT_ENTRY(0, 0, 0, 0, 0, 0, 0, 0)
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    FOR_WIN = QK_USER_30,
    FOR_MAC,
};

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    QK_BOOT, QK_USER_0,  QK_USER_1,  QK_USER_2,  QK_USER_3,  QK_USER_4,   QK_USER_5,  QK_USER_6,  QK_USER_7,  QK_USER_8,  QK_USER_9,  XXXXXXX, XXXXXXX,
    XXXXXXX, QK_USER_10, QK_USER_11, QK_USER_12, QK_USER_13, QK_USER_14,  QK_USER_15, QK_USER_16, QK_USER_17, QK_USER_18, QK_USER_19, XXXXXXX, XXXXXXX,
    XXXXXXX, QK_USER_20, QK_USER_21, QK_USER_22, QK_USER_23, QK_USER_24,  QK_USER_25, QK_USER_26, QK_USER_27, QK_USER_28, QK_USER_29, XXXXXXX,
    XXXXXXX,                         XXXXXXX,    NK_ON,      NK_OFF,      FOR_WIN,    FOR_MAC,    XXXXXXX,                            XXXXXXX
  ),
};

const PROGMEM char *test_words[] = {
  "This is a test of send_string_fast() function.", // Q
  "abcdefghijklmnopqrstuvwxyz", // W
  "zyxwvutsrqponmlkjihgfedcba", // E
  "irohanihohetotirinuruwowakayotaresotunenaramuuwyinookuyamakehukoeteasakiyumemisiwyehimosesu", // R
  "33$$55"SS_DELAY(200)"....",    // T
  "5$353A",  // Y
  "4$/-",     // U
  "$4xn",       // I
  "35nn",      // O
  "abcdef3-", // P
  "abcde3,",  // A
  "abcd3.",   // S
  "abc3/",    // D
  "ab3c",     // F
  "guxwa",  // G
  "gulwa",  // H
  "kuxwa",  // J
  "kulwa",  // K
  "a 3 bABCDEFGHIHGFEDCBA", // L
  "", // ;
  "", // Z
  "", // X
  "", // C
  "", // V
  "", // B
  "", // N
  "", // M
  "", // ,
  "", // .
  "", // /
  };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static bool is_not_apple = false;
  // #ifdef CONSOLE_ENABLE
  //   const uint16_t key_timer = timer_read();  // 時間測定開始;
  // #endif
  bool cont = true;
  switch (keycode) {
    case FOR_WIN:
      if (record->event.pressed) {
        is_not_apple = true;
      }
      cont = false;
      break;
    case FOR_MAC:
      if (record->event.pressed) {
        is_not_apple = false;
      }
      cont = false;
      break;
    case QK_USER_0 ... QK_USER_29:
      if (record->event.pressed) {
        send_string_fast(PSTR(test_words[keycode - QK_USER_0]), is_not_apple);
      }
      cont = false;
      break;
  }
  // #ifdef CONSOLE_ENABLE
  //   uprintf("%c%ums\n", pressed ? '*' : ' ', timer_elapsed(key_timer));  // 経過時間表示
  // #endif
  return cont;
}
