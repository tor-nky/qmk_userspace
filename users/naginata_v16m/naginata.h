/* Copyright 2018-2019 eswai <@eswai> / Satoru NAKAYA <@tor-nky>
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

#pragma once

// *************** 薙刀式の設定 ***************
// 有効にするとスペースキーを離しても文字キーシフト全復活
// 無効にするとDvorakJ風になる
// #define NG_USE_SHIFT_WHEN_SPACE_UP

// 後置シフトの時間制限(ms) ※ 無効にすれば時間無制限
#define NG_KOUCHI_SHIFT_MS 60

// センターシフトを押しながら2キー以上同時押しをする際の、普通キーの時間制限(ms) ※ 無効にすれば時間無制限
// #define NG_SHIFTED_DOUJI_MS 50

// センターシフトを単独で押し続けて無効になる時間(ミリ秒)
// #define SHIFT_ALONE_TIMEOUT_MS 1000
// ********************************************

// LINUX で編集モードを使うときの間
#define LINUX_WAIT_MS 72

#if defined(NG_BMP)
#   include "bmp_custom_keycodes.h" // For BLE Miro Pro new firmware(>=1.0.0)
#endif

void set_naginata(uint8_t, uint16_t *, uint16_t *);

void naginata_on(void);
void naginata_off(void);
bool naginata_state(void);
void switchOS(uint8_t);
void tategaki_toggle(void);
void tategaki_on(void);
void tategaki_off(void);
void kouchi_shift_toggle(void);
void ng_show_os(void);

void ng_send_unicode_string_P(const char *);

bool process_naginata(uint16_t, keyrecord_t *);
bool naginata_type(uint16_t, keyrecord_t *);
void kouchi_shift_loop(void);

// typedef struct {
//   uint16_t keycode;
//   uint16_t pressTime;
//   uint16_t releaseTime;
// } Keystroke;

// 同時押し等の状態を示す、各ビットがキーに対応する型
typedef uint32_t Ngkey;
// かな定義配列 ngmap[] の添字の型
typedef uint_fast16_t Ngmap_num;

void ng_space_or_enter(void);
void ng_backspace_with_repeat(void);  // {BS}
void ng_delete_with_repeat(void); // {Del}
void ng_cut(void);
void ng_copy(void);
void ng_paste(void);
void ng_move_cursor_with_repeat(bool, uint8_t, uint8_t);
void ng_back_cursor(void);
void ng_forward_cursor(void);
void ng_next_line(void);
void ng_previous_line(void);
void ng_home(void);
void ng_end(void);
void ng_katakana(void);
void ng_save(void);
void ng_hiragana(void);
void ng_redo(void);
void ng_undo(void);
void ng_saihenkan(void);
void ng_eof(void);
void ng_ime_cancel(void);
void ng_ime_complete(void);

#if !defined(QK_NG)
#   define QK_NG QK_USER
#endif

// なぜKC_キーコードを使わず、NG_キーコードを定義するのか
// 1. 英字レイアウトがQWERTYでない場合でもOK
// 2. 薙刀式レイヤーでもKC_を定義すれば、かな変換せず出力できる
typedef enum naginata_keycodes {
  NG_Q = QK_NG, // 薙刀式シフトキー
  NG_W,
  NG_E,
  NG_R,
  NG_T,
  NG_Y,
  NG_U,
  NG_I,
  NG_O,
  NG_P,

  NG_A,
  NG_S,
  NG_D,
  NG_F,
  NG_G,
  NG_H,
  NG_J,
  NG_K,
  NG_L,
  NG_SCLN,

  NG_Z,
  NG_X,
  NG_C,
  NG_V,
  NG_B,
  NG_N,
  NG_M,
  NG_COMM,
  NG_DOT,
  NG_SLSH,

  NG_SHFT,
  NG_SHFT2,

  NGSW_WIN,
  NGSW_MAC,
  NGSW_LNX,
#if defined(NG_BMP)
  NGSW_IOS,
#else
  NG_ON,
  NG_OFF,
#endif
  NG_SHOS,
  NG_KOTI,
  // NG_TAYO,
} NGKEYS;

// EEPROMに保存する設定
typedef union {
  uint32_t raw;
  struct {
    uint8_t os;
    bool kouchi_shift :1;
    // bool tategaki :1;
  };
} user_config_t;

user_config_t naginata_config;

#define NG_SAFE_RANGE (NG_KOTI + 1)

#define NG_WIN 1
#define NG_MAC 2
#define NG_LINUX 3
#define NG_IOS 4
