/* Copyright eswai <@eswai> / Satoru NAKAYA <@tor-nky>
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
#include "naginata.h"
#include "naginata_parts.h"

#include <string.h>

static void ng_set_unicode_mode(uint8_t);
static void naginata_clear(void);

static bool is_naginata = false; // 薙刀式がオンかオフか
static uint8_t naginata_layer = 0; // NG_*を配置しているレイヤー番号
static uint16_t ngon_keys[2]; // 薙刀式をオンにするキー(通常HJ)
static uint16_t ngoff_keys[2]; // 薙刀式をオフにするキー(通常FG)

// 31キーを32bitの各ビットに割り当てる
#define B_Q    (1UL<<0)
#define B_W    (1UL<<1)
#define B_E    (1UL<<2)
#define B_R    (1UL<<3)
#define B_T    (1UL<<4)

#define B_Y    (1UL<<5)
#define B_U    (1UL<<6)
#define B_I    (1UL<<7)
#define B_O    (1UL<<8)
#define B_P    (1UL<<9)

#define B_A    (1UL<<10)
#define B_S    (1UL<<11)
#define B_D    (1UL<<12)
#define B_F    (1UL<<13)
#define B_G    (1UL<<14)

#define B_H    (1UL<<15)
#define B_J    (1UL<<16)
#define B_K    (1UL<<17)
#define B_L    (1UL<<18)
#define B_SCLN (1UL<<19)

#define B_Z    (1UL<<20)
#define B_X    (1UL<<21)
#define B_C    (1UL<<22)
#define B_V    (1UL<<23)
#define B_B    (1UL<<24)

#define B_N    (1UL<<25)
#define B_M    (1UL<<26)
#define B_COMM (1UL<<27)
#define B_DOT  (1UL<<28)
#define B_SLSH (1UL<<29)

#define B_SHFT (1UL<<30)

// キーコードとキービットの対応
// メモリ削減のため配列はNG_Qを0にしている
const Ngkey ng_key[] = {
  [NG_Q    - NG_Q] = B_Q,
  [NG_W    - NG_Q] = B_W,
  [NG_E    - NG_Q] = B_E,
  [NG_R    - NG_Q] = B_R,
  [NG_T    - NG_Q] = B_T,

  [NG_Y    - NG_Q] = B_Y,
  [NG_U    - NG_Q] = B_U,
  [NG_I    - NG_Q] = B_I,
  [NG_O    - NG_Q] = B_O,
  [NG_P    - NG_Q] = B_P,

  [NG_A    - NG_Q] = B_A,
  [NG_S    - NG_Q] = B_S,
  [NG_D    - NG_Q] = B_D,
  [NG_F    - NG_Q] = B_F,
  [NG_G    - NG_Q] = B_G,

  [NG_H    - NG_Q] = B_H,
  [NG_J    - NG_Q] = B_J,
  [NG_K    - NG_Q] = B_K,
  [NG_L    - NG_Q] = B_L,
  [NG_SCLN - NG_Q] = B_SCLN,

  [NG_Z    - NG_Q] = B_Z,
  [NG_X    - NG_Q] = B_X,
  [NG_C    - NG_Q] = B_C,
  [NG_V    - NG_Q] = B_V,
  [NG_B    - NG_Q] = B_B,

  [NG_N    - NG_Q] = B_N,
  [NG_M    - NG_Q] = B_M,
  [NG_COMM - NG_Q] = B_COMM,
  [NG_DOT  - NG_Q] = B_DOT,
  [NG_SLSH - NG_Q] = B_SLSH,

  [NG_SHFT - NG_Q] = B_SHFT,
  [NG_SHFT2 - NG_Q] = B_SHFT,
};

#define NKEYS 3 // 組み合わせにある同時押しするキーの数、薙刀式なら3
                // (最大何キーまでバッファに貯めるか)

// カナ変換テーブル
typedef struct {
  Ngkey key;
  void (*func)(void);
} naginata_keymap;

// かな定義の要素数
#define NGMAP_COUNT (sizeof ngmap / sizeof ngmap[0])

// かな定義
// 順不同なのだが、3キー同時 → 2キー同時 → センターシフト → 単打 の順を推奨
// 単打とセンターシフトの並び順に「100万字日本語かなn-gramデータ」(https://kouy.exblog.jp/9731073/) を用いる
// (頻度が多いのを下にしたほうが負荷が減る)
const PROGMEM naginata_keymap ngmap[] = {
  {.key = B_SHFT        , .func = ng_space_or_enter},

  // 単打
  // {.key = B_Q           , .func = ng_null       },
  {.key = B_W           , .func = ng_send_ki    },  // き
  {.key = B_E           , .func = ng_send_te    },  // て
  {.key = B_R           , .func = ng_send_si    },  // し
  {.key = B_T           , .func = ng_1_next_line_r}, // {←}
  {.key = B_Y           , .func = ng_1_previous_line_r}, // {→}
  {.key = B_U           , .func = ng_backspace  },  // {BS}
  {.key = B_I           , .func = ng_send_ru    },  // る
  {.key = B_O           , .func = ng_send_su    },  // す
  {.key = B_P           , .func = ng_send_he    },  // へ
  {.key = B_A           , .func = ng_send_ro    },  // ろ
  {.key = B_S           , .func = ng_send_ke    },  // け
  {.key = B_D           , .func = ng_send_to    },  // と
  {.key = B_F           , .func = ng_send_ka    },  // か
  {.key = B_G           , .func = ng_send_xtu   },  // っ
  {.key = B_H           , .func = ng_send_ku    },  // く
  {.key = B_J           , .func = ng_send_a     },  // あ
  {.key = B_K           , .func = ng_send_i     },  // い
  {.key = B_L           , .func = ng_send_u     },  // う
  {.key = B_SCLN        , .func = ng_send_minus },  // ー
  {.key = B_Z           , .func = ng_send_ho    },  // ほ
  {.key = B_X           , .func = ng_send_hi    },  // ひ
  {.key = B_C           , .func = ng_send_ha    },  // は
  {.key = B_V           , .func = ng_send_ko    },  // こ
  {.key = B_B           , .func = ng_send_so    },  // そ
  {.key = B_N           , .func = ng_send_ta    },  // た
  {.key = B_M           , .func = ng_send_na    },  // な
  {.key = B_COMM        , .func = ng_send_nn    },  // ん
  {.key = B_DOT         , .func = ng_send_ra    },  // ら
  {.key = B_SLSH        , .func = ng_send_re    },  // れ

  // センターシフト
  {.key = B_SHFT|B_Q    , .func = ng_null       },
  {.key = B_SHFT|B_W    , .func = ng_send_me    },  // め
  {.key = B_SHFT|B_E    , .func = ng_send_ri    },  // り
  {.key = B_SHFT|B_R    , .func = ng_send_ne    },  // ね
  {.key = B_SHFT|B_T    , .func = ng_s1_next_line_r}, // +{←}
  {.key = B_SHFT|B_Y    , .func = ng_s1_previous_line_r}, // +{→}
  {.key = B_SHFT|B_U    , .func = ng_send_sa    },  // さ
  {.key = B_SHFT|B_I    , .func = ng_send_yo    },  // よ
  {.key = B_SHFT|B_O    , .func = ng_send_e     },  // え
  {.key = B_SHFT|B_P    , .func = ng_send_yu    },  // ゆ
  {.key = B_SHFT|B_A    , .func = ng_send_se    },  // せ
  {.key = B_SHFT|B_S    , .func = ng_send_mi    },  // み
  {.key = B_SHFT|B_D    , .func = ng_send_ni    },  // に
  {.key = B_SHFT|B_F    , .func = ng_send_ma    },  // ま
  {.key = B_SHFT|B_G    , .func = ng_send_ti    },  // ち
  {.key = B_SHFT|B_H    , .func = ng_send_ya    },  // や
  {.key = B_SHFT|B_J    , .func = ng_send_no    },  // の
  {.key = B_SHFT|B_K    , .func = ng_send_mo    },  // も
  {.key = B_SHFT|B_L    , .func = ng_send_tu    },  // つ
  {.key = B_SHFT|B_SCLN , .func = ng_send_hu    },  // ふ
  {.key = B_SHFT|B_Z    , .func = ng_send_ho    },  // ほ
  {.key = B_SHFT|B_X    , .func = ng_send_hi    },  // ひ
  {.key = B_SHFT|B_C    , .func = ng_send_wo    },  // を
  {.key = B_SHFT|B_V    , .func = ng_edit_touten},  // 、
  {.key = B_SHFT|B_B    , .func = ng_send_nu    },  // ぬ
  {.key = B_SHFT|B_N    , .func = ng_send_o     },  // お
  {.key = B_SHFT|B_M    , .func = ng_edit_kuten },  // 。
  {.key = B_SHFT|B_COMM , .func = ng_send_mu    },  // む
  {.key = B_SHFT|B_DOT  , .func = ng_send_wa    },  // わ
  {.key = B_SHFT|B_SLSH , .func = ng_send_re    },  // れ

  // 濁音
  {.key = B_F|B_U       , .func = ng_send_za    },  // ざ
  {.key = B_F|B_O       , .func = ng_send_zu    },  // ず
  {.key = B_F|B_P       , .func = ng_send_be    },  // べ
  {.key = B_F|B_H       , .func = ng_send_gu    },  // ぐ
  {.key = B_F|B_L       , .func = ng_send_du    },  // づ
  {.key = B_F|B_SCLN    , .func = ng_send_bu    },  // ぶ
  {.key = B_F|B_N       , .func = ng_send_da    },  // だ
  {.key = B_J|B_W       , .func = ng_send_gi    },  // ぎ
  {.key = B_J|B_E       , .func = ng_send_de    },  // で
  {.key = B_J|B_R       , .func = ng_send_zi    },  // じ
  {.key = B_J|B_A       , .func = ng_send_ze    },  // ぜ
  {.key = B_J|B_S       , .func = ng_send_ge    },  // げ
  {.key = B_J|B_D       , .func = ng_send_do    },  // ど
  {.key = B_J|B_F       , .func = ng_send_ga    },  // が
  {.key = B_J|B_G       , .func = ng_send_di    },  // ぢ
  {.key = B_J|B_Z       , .func = ng_send_bo    },  // ぼ
  {.key = B_J|B_X       , .func = ng_send_bi    },  // び
  {.key = B_J|B_C       , .func = ng_send_ba    },  // ば
  {.key = B_J|B_V       , .func = ng_send_go    },  // ご
  {.key = B_J|B_B       , .func = ng_send_zo    },  // ぞ

  // 半濁音
  {.key = B_V|B_P       , .func = ng_send_pe    },  // ぺ
  {.key = B_V|B_SCLN    , .func = ng_send_pu    },  // ぷ
  {.key = B_M|B_Z       , .func = ng_send_po    },  // ぽ
  {.key = B_M|B_X       , .func = ng_send_pi    },  // ぴ
  {.key = B_M|B_C       , .func = ng_send_pa    },  // ぱ

  // 小書き
  {.key = B_Q|B_I       , .func = ng_send_xyo   },  // ょ
  {.key = B_Q|B_O       , .func = ng_send_xe    },  // ぇ
  {.key = B_Q|B_P       , .func = ng_send_xyu   },  // ゅ
  {.key = B_Q|B_S       , .func = ng_send_xke   },  // ヶ
  {.key = B_Q|B_F       , .func = ng_send_xka   },  // ヵ
  {.key = B_Q|B_H       , .func = ng_send_xya   },  // ゃ
  {.key = B_Q|B_J       , .func = ng_send_xa    },  // ぁ
  {.key = B_Q|B_K       , .func = ng_send_xi    },  // ぃ
  {.key = B_Q|B_L       , .func = ng_send_xu    },  // ぅ
  {.key = B_Q|B_N       , .func = ng_send_xo    },  // ぉ
  {.key = B_Q|B_DOT     , .func = ng_send_xwa   },  // ゎ

  // 清音拗音
  {.key = B_W|B_H       , .func = ng_send_kya   },  // きゃ
  {.key = B_E|B_H       , .func = ng_send_rya   },  // りゃ
  {.key = B_R|B_H       , .func = ng_send_sya   },  // しゃ
  {.key = B_S|B_H       , .func = ng_send_mya   },  // みゃ
  {.key = B_D|B_H       , .func = ng_send_nya   },  // にゃ
  {.key = B_G|B_H       , .func = ng_send_tya   },  // ちゃ
  {.key = B_X|B_H       , .func = ng_send_hya   },  // ひゃ
  {.key = B_W|B_P       , .func = ng_send_kyu   },  // きゅ
  {.key = B_E|B_P       , .func = ng_send_ryu   },  // りゅ
  {.key = B_R|B_P       , .func = ng_send_syu   },  // しゅ
  {.key = B_S|B_P       , .func = ng_send_myu   },  // みゅ
  {.key = B_D|B_P       , .func = ng_send_nyu   },  // にゅ
  {.key = B_G|B_P       , .func = ng_send_tyu   },  // ちゅ
  {.key = B_X|B_P       , .func = ng_send_hyu   },  // ひゅ
  {.key = B_W|B_I       , .func = ng_send_kyo   },  // きょ
  {.key = B_E|B_I       , .func = ng_send_ryo   },  // りょ
  {.key = B_R|B_I       , .func = ng_send_syo   },  // しょ
  {.key = B_S|B_I       , .func = ng_send_myo   },  // みょ
  {.key = B_D|B_I       , .func = ng_send_nyo   },  // にょ
  {.key = B_G|B_I       , .func = ng_send_tyo   },  // ちょ
  {.key = B_X|B_I       , .func = ng_send_hyo   },  // ひょ
  // 濁音拗音
  {.key = B_J|B_W|B_H   , .func = ng_send_gya   },  // ぎゃ
  {.key = B_J|B_R|B_H   , .func = ng_send_ja    },  // じゃ
  {.key = B_J|B_G|B_H   , .func = ng_send_dya   },  // ぢゃ
  {.key = B_J|B_X|B_H   , .func = ng_send_bya   },  // びゃ
  {.key = B_J|B_W|B_P   , .func = ng_send_gyu   },  // ぎゅ
  {.key = B_J|B_R|B_P   , .func = ng_send_ju    },  // じゅ
  {.key = B_J|B_G|B_P   , .func = ng_send_dyu   },  // ぢゅ
  {.key = B_J|B_X|B_P   , .func = ng_send_byu   },  // びゅ
  {.key = B_J|B_W|B_I   , .func = ng_send_gyo   },  // ぎょ
  {.key = B_J|B_R|B_I   , .func = ng_send_jo    },  // じょ
  {.key = B_J|B_G|B_I   , .func = ng_send_dyo   },  // ぢょ
  {.key = B_J|B_X|B_I   , .func = ng_send_byo   },  // びょ
  // 半濁音拗音
  {.key = B_M|B_X|B_I   , .func = ng_send_pyo   },  // ぴょ
  {.key = B_M|B_X|B_P   , .func = ng_send_pyu   },  // ぴゅ
  {.key = B_M|B_X|B_H   , .func = ng_send_pya   },  // ぴゃ
  // 外来音
  {.key = B_M|B_E|B_P   , .func = ng_send_thu   },  // てゅ
  {.key = B_M|B_E|B_K   , .func = ng_send_thi   },  // てぃ
  {.key = B_J|B_E|B_P   , .func = ng_send_dhu   },  // でゅ
  {.key = B_J|B_E|B_K   , .func = ng_send_dhi   },  // でぃ
  {.key = B_M|B_D|B_L   , .func = ng_send_twu   },  // とぅ
  {.key = B_J|B_D|B_L   , .func = ng_send_dwu   },  // どぅ
  {.key = B_M|B_R|B_O   , .func = ng_send_sye   },  // しぇ
  {.key = B_M|B_G|B_O   , .func = ng_send_tye   },  // ちぇ
  {.key = B_J|B_R|B_O   , .func = ng_send_je    },  // じぇ
  {.key = B_J|B_G|B_O   , .func = ng_send_dye   },  // ぢぇ
  {.key = B_V|B_SCLN|B_O, .func = ng_send_fe    },  // ふぇ
  {.key = B_V|B_SCLN|B_P, .func = ng_send_fyu   },  // ふゅ
  {.key = B_V|B_SCLN|B_J, .func = ng_send_fa    },  // ふぁ
  {.key = B_V|B_SCLN|B_K, .func = ng_send_fi    },  // ふぃ
  {.key = B_V|B_SCLN|B_N, .func = ng_send_fo    },  // ふぉ
  {.key = B_F|B_L|B_O   , .func = ng_send_ve    },  // ヴぇ
  {.key = B_F|B_L|B_P   , .func = ng_send_vyu   },  // ヴゅ
  {.key = B_F|B_L|B_J   , .func = ng_send_va    },  // ヴぁ
  {.key = B_F|B_L|B_K   , .func = ng_send_vi    },  // ヴぃ
  {.key = B_F|B_L|B_SCLN, .func = ng_send_vu    },  // ヴ
  {.key = B_F|B_L|B_N   , .func = ng_send_vo    },  // ヴぉ
  {.key = B_V|B_L|B_O   , .func = ng_send_we    },  // うぇ
  {.key = B_V|B_L|B_K   , .func = ng_send_wi    },  // うぃ
  {.key = B_V|B_L|B_N   , .func = ng_send_who   },  // うぉ
  {.key = B_V|B_K|B_O   , .func = ng_send_ye    },  // いぇ
  {.key = B_V|B_L|B_J   , .func = ng_send_tsa   },  // つぁ
  {.key = B_V|B_H|B_O   , .func = ng_send_qe    },  // くぇ
  {.key = B_V|B_H|B_J   , .func = ng_send_qa    },  // くぁ
  {.key = B_V|B_H|B_K   , .func = ng_send_qi    },  // くぃ
  {.key = B_V|B_H|B_N   , .func = ng_send_qo    },  // くぉ
  {.key = B_V|B_H|B_DOT , .func = ng_send_kuxwa },  // くゎ
  {.key = B_F|B_H|B_O   , .func = ng_send_gwe   },  // ぐぇ
  {.key = B_F|B_H|B_J   , .func = ng_send_gwa   },  // ぐぁ
  {.key = B_F|B_H|B_K   , .func = ng_send_gwi   },  // ぐぃ
  {.key = B_F|B_H|B_N   , .func = ng_send_gwo   },  // ぐぉ
  {.key = B_F|B_H|B_DOT , .func = ng_send_guxwa },  // ぐゎ

  // 機能部分
  {.key = B_H|B_J       , .func = naginata_on},   //　かなオン
  {.key = B_F|B_G       , .func = naginata_off},  //　かなオフ
  {.key = B_V|B_M       , .func = ng_enter},

// 編集モード
  {.key = B_D|B_F|B_Y   , .func = ng_home}, // {Home}
  {.key = B_D|B_F|B_H   , .func = ng_edit_kakutei_end}, // {Enter}{End}
  {.key = B_D|B_F|B_N   , .func = ng_end}, // {End}
  {.key = B_D|B_F|B_U   , .func = ng_edit_delete_to_end}, // +{End}{BS}
  {.key = B_D|B_F|B_J   , .func = ng_1_back_cursor_r}, // {↑}
  {.key = B_D|B_F|B_M   , .func = ng_1_forward_cursor_r}, // {↓}
  {.key = B_D|B_F|B_I   , .func = ng_saihenkan}, // {vk1Csc079}
  {.key = B_D|B_F|B_K   , .func = ng_s1_back_cursor_r}, // +{↑}
  {.key = B_D|B_F|B_COMM, .func = ng_s1_forward_cursor_r}, // +{↓}
  {.key = B_D|B_F|B_O   , .func = ng_delete_with_repeat}, // {Del}
  {.key = B_D|B_F|B_L   , .func = ng_s7_back_cursor_r}, // +{↑ 7}
  {.key = B_D|B_F|B_DOT , .func = ng_s7_forward_cursor_r}, // +{↓ 7}
  {.key = B_D|B_F|B_P   , .func = ng_ime_cancel}, // +{Esc 3}
  {.key = B_D|B_F|B_SCLN, .func = ng_katakana}, // ^i
  {.key = B_D|B_F|B_SLSH, .func = ng_hiragana}, // ^u
  {.key = B_J|B_K|B_Q   , .func = ng_eof}, // ^{End}
  {.key = B_J|B_K|B_A   , .func = ng_ellipsis}, // ……{改行}
  {.key = B_J|B_K|B_Z   , .func = ng_bar}, // ――{改行}
  {.key = B_J|B_K|B_W   , .func = ng_solidus}, // ／{改行}
  {.key = B_J|B_K|B_S   , .func = ng_left_white_corner_bracket}, // 『{改行}
  {.key = B_J|B_K|B_X   , .func = ng_right_white_corner_bracket}, // 』{改行}
  {.key = B_J|B_K|B_D   , .func = ng_question_mark}, // ？{改行}
  {.key = B_J|B_K|B_C   , .func = ng_exclamation_mark}, // ！{改行}
  {.key = B_J|B_K|B_R   , .func = ng_save}, // ^s
  {.key = B_J|B_K|B_F   , .func = ng_left_corner_bracket}, // 「{改行}
  {.key = B_J|B_K|B_V   , .func = ng_right_corner_bracket}, // 」{改行}
  {.key = B_J|B_K|B_T   , .func = ng_middle_dot}, // ・
  {.key = B_J|B_K|B_G   , .func = ng_left_parenthesis}, // ({改行}
  {.key = B_J|B_K|B_B   , .func = ng_right_parenthesis}, // ){改行}
  {.key = B_C|B_V|B_Y   , .func = ng_edit_s_home}, // +{Home}
  {.key = B_C|B_V|B_H   , .func = ng_copy}, // ^c
  {.key = B_C|B_V|B_N   , .func = ng_edit_s_end}, // +{End}
  {.key = B_C|B_V|B_U   , .func = ng_cut}, // ^x
  {.key = B_C|B_V|B_J   , .func = ng_1_next_line_r}, // {←}
  {.key = B_C|B_V|B_M   , .func = ng_s1_next_line_r}, // +{←}
  {.key = B_C|B_V|B_I   , .func = ng_saihenkan}, // {vk1Csc079}
  {.key = B_C|B_V|B_K   , .func = ng_1_previous_line_r}, // {→}
  {.key = B_C|B_V|B_COMM, .func = ng_s1_previous_line_r}, // +{→}
  {.key = B_C|B_V|B_O   , .func = ng_paste}, // ^v
  {.key = B_C|B_V|B_L   , .func = ng_s7_next_line_r}, // +{← 7}
  {.key = B_C|B_V|B_DOT , .func = ng_delete_current_line}, // {End}+{Home}
  {.key = B_C|B_V|B_P   , .func = ng_undo}, // ^z
  {.key = B_C|B_V|B_SCLN, .func = ng_s7_previous_line_r}, // +{→ 7}
  {.key = B_C|B_V|B_SLSH, .func = ng_redo}, // ^y
  {.key = B_M|B_COMM|B_Q, .func = ng_vertical_line}, // ｜{改行}
  {.key = B_M|B_COMM|B_A, .func = ng_left_double_angle_bracket}, // 《{改行}
  {.key = B_M|B_COMM|B_Z, .func = ng_right_double_angle_bracket}, // 》{改行}
  {.key = B_M|B_COMM|B_W, .func = ng_edit_separate_line}, // 　　　×　　　×　　　×{改行 2}
  {.key = B_M|B_COMM|B_S, .func = ng_left_black_lenticular_bracket}, // 【{改行}
  {.key = B_M|B_COMM|B_X, .func = ng_right_black_lenticular_bracket}, // 】{改行}
  {.key = B_M|B_COMM|B_E, .func = ng_edit_serifu_zengyo}, // {Home}{→}{End}{Del 2}{←}
  {.key = B_M|B_COMM|B_D, .func = ng_edit_togaki_zengyo}, // {Home}{→}{End}{Del 4}{←}
  {.key = B_M|B_COMM|B_C, .func = ng_right_corner_bracket_next_line}, // 」{改行}{改行}
  {.key = B_M|B_COMM|B_R, .func = ng_edit_serifu}, // {Home}{改行}{Space 1}{←}
  {.key = B_M|B_COMM|B_F, .func = ng_edit_togaki}, // {Home}{改行}{Space 3}{←}
  {.key = B_M|B_COMM|B_V, .func = ng_right_corner_bracket_next_line_left_corner_bracket}, // 」{改行}{改行}「{改行}
  {.key = B_M|B_COMM|B_T, .func = ng_white_circle}, // ○{改行}
  {.key = B_M|B_COMM|B_G, .func = ng_edit_3_space}, // {Space 3}
  {.key = B_M|B_COMM|B_B, .func = ng_right_corner_bracket_next_line_space}, // 」{改行}{改行}{Space}
};

// 薙刀式のレイヤー、オンオフするキー
void set_naginata(uint8_t layer, uint16_t *onk, uint16_t *offk) {
  naginata_layer = layer;
  ngon_keys[0] = *onk;
  ngon_keys[1] = *(onk+1);
  ngoff_keys[0] = *offk;
  ngoff_keys[1] = *(offk+1);

  naginata_config.raw = eeconfig_read_user();
  switch (naginata_config.os) {
    case NG_WIN ...  NG_LINUX:
      break;
    default:
      naginata_config.os = NG_WIN;
      break;
  }
  switchOS(naginata_config.os);
}

// 薙刀式のon/off状態を返す
bool naginata_state(void) {
  return is_naginata;
}

void switchOS(uint8_t os) {
  naginata_config.os = os;
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

static void ng_set_unicode_mode(uint8_t os) {
  switch (os) {
    case NG_WIN:
      set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
      break;
    case NG_MAC:
      set_unicode_input_mode(UNICODE_MODE_MACOS);
      break;
    case NG_LINUX:
      set_unicode_input_mode(UNICODE_MODE_LINUX);
      break;
  }
}

void ng_show_os(void) {
  switch (naginata_config.os) {
    case NG_WIN:
      SEND_STRING("win");
      break;
    case NG_MAC:
      SEND_STRING("mac");
      break;
    case NG_LINUX:
      SEND_STRING("linux");
      break;
  }
}

#define NG_SEND_UNICODE_STRING(string) ng_send_unicode_string(PSTR(string))

void ng_send_unicode_string(const char *str) {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code(KC_GRAVE);
      send_unicode_string(str);
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_LINUX:
      tap_code16(LCTL(KC_SPACE));
      send_unicode_string(str);
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_MAC:
      // Karabiner-Elementsが必要
      tap_code(KC_LANGUAGE_2);  // 未確定文字を確定する
      tap_code16_delay(LCTL(KC_F20), 56); // Unicode HEX Inputへ切り替え
      send_unicode_string(str);
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      tap_code(KC_NUM_LOCK);  // IME Cancel
      break;
  }
}

// Shift+英字 で IMEオフ
static bool process_shifted_alphabet(uint16_t keycode, keyrecord_t *record) {
  uint8_t mods = get_mods();
  // 装飾キーにシフトだけを押していて、アルファベットが押されたとき
  if ((mods & (MOD_BIT(KC_LEFT_SHIFT) | MOD_BIT(KC_RIGHT_SHIFT)))
      && !(mods & ~(MOD_BIT(KC_LEFT_SHIFT) | MOD_BIT(KC_RIGHT_SHIFT)))
      && record->event.pressed && keycode >= KC_A && keycode <= KC_Z) {
    naginata_off();
    return true;
  }
  return false;
}

// modifierが押されたら薙刀式レイヤーをオフしてベースレイヤーに戻す
// get_mods()がうまく動かない
static int8_t n_modifier = 0;

static bool process_modifier(uint16_t keycode, keyrecord_t *record) {
  if (IS_MODIFIER_KEYCODE(keycode) || IS_QK_MOD_TAP(keycode)) {
    if (record->event.pressed) {
      naginata_type(KC_NO, record); // 未出力キーを処理
      n_modifier++;
      layer_off(naginata_layer);
    } else {
      n_modifier--;
      if (n_modifier <= 0) {
        n_modifier = 0;
        layer_on(naginata_layer);
      }
    }
    return true;
  }
  return false;
}

static uint16_t fghj_buf = KC_NO; // 押しているJかKのキーコード

// 薙刀式の起動処理(容量が大きいCOMBOを使わない)
static bool enable_naginata(uint16_t keycode, keyrecord_t *record) {
  // キープレス
  if (record->event.pressed) {
    // １キー目がかなオン・オフキーだった
    if (fghj_buf != KC_NO) {
      // ２キー目、１キー目、両方ともかなオンキー
      if ((keycode == ngon_keys[0] && fghj_buf == ngon_keys[1]) ||
          (keycode == ngon_keys[1] && fghj_buf == ngon_keys[0])) {
        fghj_buf = KC_NO;
        naginata_on();
        return false;
      // ２キー目、１キー目、両方ともかなオフキー
      } else if ((keycode == ngoff_keys[0] && fghj_buf == ngoff_keys[1]) ||
          (keycode == ngoff_keys[1] && fghj_buf == ngoff_keys[0])) {
        fghj_buf = KC_NO;
        naginata_off();
        return false;
      }
      // どちらでもなければ、1キー目を出力
      tap_code(fghj_buf);
      fghj_buf = KC_NO;
    }
    // 今度のキーがかなオン・オフキーの場合
    if (keycode == ngon_keys[0] || keycode == ngon_keys[1] || keycode == ngoff_keys[0] || keycode == ngoff_keys[1]) {
      fghj_buf = keycode;
      return false;
    }
  // かなオン・オフキーが離された
  } else if (keycode == fghj_buf) {
    tap_code(fghj_buf);
    fghj_buf = KC_NO;
    // Shift + Jで、先にShiftを外した場合にShiftがリリースされない不具合対策
    // unregister_code16(keycode);
    // return false;
  }
  return true;  // QMKにまかせる
}

// 薙刀式をオン
void naginata_on(void) {
  is_naginata = true;
  n_modifier = 0;
  layer_on(naginata_layer);

  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_INTERNATIONAL_2);
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      break;
  }
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  naginata_clear();
  layer_off(naginata_layer);

  uint8_t mods = get_mods();
  clear_mods();
  wait_ms(8); // キーを出力してから確定やIME操作までに、間を空ける
  switch (naginata_config.os) {
    case NG_WIN:
      // 確定→ひらがな→半角/全角
      tap_code16_delay(LSFT(LCTL(KC_INTERNATIONAL_4)), 8);  // Shift+Ctrl+変換
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_GRAVE); // 半角/全角
      break;
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code16(LCTL(KC_SPACE));
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      break;
  }
  set_mods(mods);
}

// #define LOG_PROCESS_NAGINATA
// 薙刀式の入力処理
bool process_naginata(uint16_t keycode, keyrecord_t *record) {
  // // まれに薙刀モードオンのまま、レイヤーがオフになることがあるので、対策
  // if (n_modifier == 0 && is_naginata && !layer_state_is(naginata_layer))
  //   layer_on(naginata_layer);
  // if (n_modifier == 0 && !is_naginata && layer_state_is(naginata_layer))
  //   layer_off(naginata_layer);
  // if (n_modifier > 0 && layer_state_is(naginata_layer))
  //   layer_off(naginata_layer);

  // OS切り替え(UNICODE出力)
  if (record->event.pressed) {
    switch (keycode) {
      case NGSW_WIN:
        switchOS(NG_WIN);
        return false;
      case NGSW_MAC:
        switchOS(NG_MAC);
        return false;
      case NGSW_LNX:
        switchOS(NG_LINUX);
        return false;
      case NG_ON:
        // 起動判定中のキーを出力
        if (fghj_buf != KC_NO) {
          tap_code(fghj_buf);
          fghj_buf = KC_NO;
        }
        naginata_on();
        return false;
      case NG_OFF:
        naginata_type(keycode, record); // キーリピート解除、未出力キーを処理
        naginata_off();
        return false;
      case NG_SHOS:
        ng_show_os();
        return false;
    }
  }
  if (!is_naginata)
    return enable_naginata(keycode, record);

  if (process_shifted_alphabet(keycode, record))
    return true;

  if (process_modifier(keycode, record))
    return true;

  // キー入力を文字に変換して出力する
  return naginata_type(keycode, record);
}

// かな定義を探し、配列の添え字を返す
// 見つからないと NGMAP_COUNT を返す
static Ngmap_num ng_search(Ngkey searching_key) {
  // if (!searching_key)  return false;
  for (Ngmap_num num = NGMAP_COUNT; num-- > 0; ) {  // 逆順で検索
    if (searching_key == ngmap[num].key) {
      return num;
    }
  }
  return NGMAP_COUNT;
}

// かな定義を探し出力する
// 成功すれば true を返す
static bool ng_search_and_send(Ngkey searching_key) {
  // if (!searching_key)  return false;
  Ngmap_num num = ng_search(searching_key);
  if (num < NGMAP_COUNT) {
    ngmap[num].func();
    return true;
  }
  return false;
}

// 変換してよいか調べる
// None: なし, One: 一つしかない, Multipul: しぼれない,
enum TransState { None, One, Multipul };

static enum TransState which_trans_state(Ngkey search) {
  enum TransState state = None;
  for (Ngmap_num num = 0; num < NGMAP_COUNT; num++) {
    Ngkey key = ngmap[num].key;
    // search を含む
    if ((key & search) == search) {
      Ngkey remains = key ^ search;
      switch (remains) {
        case 0:
          if (state == None) {
            state = One;
          }
          break;
        default:
          if (!(remains & B_SHFT)) {
            return Multipul;
          }
          break;
      }
    }
  }
  return state;
}

static Ngkey pressed_key = 0; // 同時押しの状態を示す。各ビットがキーに対応する。
static uint_fast8_t waiting_count = 0; // 文字キーを数える
static uint_fast8_t center_shift_count = 0;
static uint8_t ng_center_keycode = KC_NO;
static enum ReuseKeyState { Off, Run } reuse_key_state = Off;

// キー入力を文字に変換して出力する
// 薙刀式のキー入力だったなら false を返す
// そうでない時は未出力を全て出力、true を返してQMKにまかせる
bool naginata_type(uint16_t keycode, keyrecord_t *record) {
  static Ngkey waiting_keys[NKEYS]; // 各ビットがキーに対応する
  static bool center_shift = false; // センターシフトの連続用

  Ngkey recent_key = 0;  // 各ビットがキーに対応する
  const bool pressing = record->event.pressed;
  bool store_key_later = false;

  switch (keycode) {
    case NG_Q ... NG_SLSH:
      recent_key = ng_key[keycode - NG_Q];
      break;
    case NG_SHFT: // スペースキー
    case NG_SHFT2:  // エンターキー
      if (pressing) {
        center_shift_count++;
        recent_key = B_SHFT;
        ng_center_keycode = (keycode == NG_SHFT ? KC_SPACE : KC_ENTER);
        reuse_key_state = Off;
      } else if (center_shift_count) {
        center_shift_count--;
        if (!center_shift_count) {
          recent_key = B_SHFT;
        }
      }
      break;
    default:
      reuse_key_state = Off;
      break;
  }

  // センターシフトを押していなければ解除
  if (!(pressed_key & B_SHFT)) {
    center_shift = false;
  }

  // キーを押した
  if (pressing) {
    pressed_key |= recent_key;  // キーを加える
    // センターシフト(前置シフト限定)
    if (recent_key == B_SHFT) {
      store_key_later = true;
    } else if (recent_key) {
      // 配列に押したキーを保存
      waiting_keys[waiting_count++] = recent_key;
      // キー再利用処理
      if (reuse_key_state == Run && ng_search(pressed_key) < NGMAP_COUNT) {
        reuse_key_state = Off;
        waiting_keys[0] = pressed_key;
      }
    }
  }

  // 出力
  {
    uint_fast8_t searching_count = waiting_count;
    while (searching_count) {
      // バッファ内のキーを組み合わせる
      Ngkey searching_key = 0;
      if (center_shift) {
        searching_key |= B_SHFT; // センターキー
      }
      for (uint_fast8_t i = 0; i < searching_count; i++) {
        searching_key |= waiting_keys[i];
      }

      // バッファ内の全てのキーを組み合わせている場合
      // (前置センターシフトの時は全て出力する)
      if (searching_count == waiting_count && !store_key_later) {
        // 薙刀式のキーを押した時(同時押し定義の最大数に達していたら変換するため飛ばす)
        if (pressing && recent_key && waiting_count < NKEYS) {
          // 変換してよいか調べる
          enum TransState trans_state = which_trans_state(searching_key);
          // 組み合わせがなくなった
          if (trans_state == None && searching_count > 1) {
            searching_count--;  // 最後のキーを減らして検索
            continue;
          // まだ変換できない
          } else if (!(trans_state == None || trans_state == One)) {
            break;
          }
        // キーを離した時は、そのキーが関わるところまで出力する
        // (薙刀式以外のキーを離したのなら出力しない)
        } else if (!pressing && !(searching_key & recent_key)) {
          break;
        }
      }

      // かな定義を探して出力する
      // 1キーで何も定義がないキーもここで配列から取り除く
      if (ng_search_and_send(searching_key) || searching_count == 1) {
        // 関数naginata_clear()が実行されていたら早期終了
        if (!waiting_count) {
          return false;
        }
        // センターシフトの連続用
        center_shift = (bool)(searching_key & B_SHFT);
        // 1回出力したらキー再利用は終わり
        reuse_key_state = Off;
        // 見つかった分のキーを配列から取り除く
        waiting_count -= searching_count;
        for (uint_fast8_t i = 0; i < waiting_count; i++) {
          waiting_keys[i] = waiting_keys[i + searching_count];
        }
        searching_count = waiting_count;
      // 見つからなかったら最後のキーを減らして再検索
      } else {
        searching_count--;
      }
    }
  }

  // センターシフト(前置シフト限定)
  if (store_key_later) {
    // 配列に押したキーを保存
    waiting_keys[waiting_count++] = recent_key;
  // キーを離した時
  } else if (!pressing) {
    reuse_key_state = Off;
    if (recent_key == B_SHFT) {
      pressed_key = 0;
    } else {
      pressed_key &= ~recent_key; // キーを取り除く
      if (!(waiting_count || pressed_key & B_SHFT || !pressed_key)) {
        // スペースを押していないなら次回、キー再利用可能
        reuse_key_state = Run;
      }
    }
  }
  return (recent_key == 0);
}

// バッファをクリアする
static void naginata_clear(void) {
  pressed_key = 0;
  waiting_count = 0; // 文字キーを数える
  reuse_key_state = Off;
}

void ng_space_or_enter(void) {
  if (ng_center_keycode == KC_NO) return;

  if (center_shift_count > 1) {
    tap_code16(LSFT(ng_center_keycode));
  } else {
    tap_code(ng_center_keycode);
  }
  ng_center_keycode = KC_NO;
}

void ng_backspace(void) { // {BS}
  tap_code(KC_BACKSPACE);
}

void ng_delete_with_repeat(void) { // {Del}
  register_code(KC_DELETE);
}

void ng_cut() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_X));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_X), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16_delay(LCMD(KC_X), 8);
      break;
  }
}

void ng_copy() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_C));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_C), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_C));
      break;
  }
}

void ng_paste() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16_delay(LCTL(KC_V), LINUX_WAIT_MS);
      break;
    case NG_LINUX:
      wait_ms(48);
      tap_code16_delay(LCTL(KC_V), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16_delay(LCMD(KC_V), 135);
      break;
  }
}

// リピート対応の方向キー移動
// リピート中を示す変数を更新
void ng_move_cursor(bool shift, uint8_t code, uint8_t count) {
  if (shift) {
    register_code(KC_LEFT_SHIFT);
  }
  for ( ; count > 0; count--) {
    tap_code(code);
  }
  if (shift) {
    unregister_code(KC_LEFT_SHIFT);
  }
}

void ng_back_cursor(void) {
  ng_1_back_cursor_r();
}
void ng_forward_cursor(void) {
  ng_1_forward_cursor_r();
}
void ng_next_line(void) {
  ng_1_next_line_r();
}
void ng_previous_line(void) {
  ng_1_previous_line_r();
}

void ng_home() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code(KC_HOME);
      break;
    case NG_LINUX:
      tap_code_delay(KC_HOME, LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCTL(KC_A));
      break;
  }
}

void ng_end() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code(KC_END);
      break;
    case NG_LINUX:
      tap_code_delay(KC_END, LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCTL(KC_E));
      break;
  }
}

void ng_katakana() {
  tap_code(KC_F7);
}

void ng_save() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_S));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_S), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_S));
      break;
  }
}

void ng_hiragana() {
  tap_code(KC_F6);
}

void ng_redo() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_Y));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_Y), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(LSFT(KC_Z)));
      break;
  }
}

void ng_undo() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_Z));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_Z), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_Z));
      break;
  }
}

void ng_saihenkan() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LWIN(KC_SLASH));
      break;
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_4); // 変換
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_1);
      tap_code(KC_LANGUAGE_1);
      break;
  }
}

void ng_eof() {
  ng_ime_complete();
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_END));
      break;
    case NG_LINUX:
      tap_code16_delay(LCTL(KC_END), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_LEFT));
      tap_code16(LCMD(KC_DOWN));
      break;
  }
}

void ng_ime_cancel() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));  // Shift+Ctrl+無変換x2
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));
      break;
    case NG_MAC:
      tap_code(KC_NUM_LOCK);
      // ライブ変換対応
      tap_code(KC_NUM_LOCK);
      tap_code(KC_NUM_LOCK);
      break;
  }
}

void ng_ime_complete() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      break;
    case NG_LINUX:
      tap_code16(LCTL(KC_SPACE));
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code16(LSFT(KC_LANGUAGE_1));  // Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      break;
  }
}
