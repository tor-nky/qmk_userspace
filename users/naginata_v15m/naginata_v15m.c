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
#include "unicode_P.h"
#if defined(NG_BMP)
#   include "bmp_host_driver.h"
#   include "keyboards/ble_micro_pro/keymaps/naginata_v15m/bmp_send_string.h"
#   define BMP_DELAY 40
#endif

#include <string.h>

// 修正: naginata.hはextern宣言のみになったので、実体はここで定義する
user_config_t naginata_config;

static bool naginata_type(uint16_t, keyrecord_t *);

static bool is_naginata = false; // 薙刀式がオンかオフか
static uint8_t naginata_layer = 0; // NG_*を配置しているレイヤー番号
static uint16_t ngon_keys[2]; // 薙刀式をオンにするキー(通常HJ)
static uint16_t ngoff_keys[2]; // 薙刀式をオフにするキー(通常FG)

#if defined(NG_KOUCHI_SHIFT_MS) || defined (NG_SHIFTED_DOUJI_MS) || defined (SHIFT_ALONE_TIMEOUT_MS)
  static uint16_t ng_last_pressed_ms = 0; // 薙刀式のキーを最後に押した時間
#endif
static Ngkey pressed_key = 0; // 同時押しの状態を示す。各ビットがキーに対応する。
static uint_fast8_t waiting_count = 0; // 文字キーを数える
static uint_fast8_t center_shift_count = 0;
static uint8_t ng_center_keycode = KC_NO;
static enum ReuseKeyState { Off, Run } reuse_key_state = Off;

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
// B_Q..B_SLSHのビット位置はNG_Q..NG_SLSHの並び順と一致しているので計算で求まる
// (NG_SHFT2だけは例外でNG_SHFTと同じB_SHFTを使う)
#define NG_BIT(kc) (1UL << ((kc) - NG_Q))

// カナ変換テーブル
typedef struct {
  Ngkey key;
  void (*func)(void);
} naginata_keymap;

// リピート中を示す変数
static struct {
  uint8_t code; // リピート中の文字コード
  uint8_t mod;  // リピート中の装飾キーの文字コード
} repeating = { KC_NO, KC_NO };

// キーリピート解除
static void end_repeating_key(void) {
#if defined(NG_BMP)
  if (repeating.code != KC_NO) {
    // 参考: quantum/send_string/send_string_keycodes.h
    char str[4] = "\1\3\0\0";
    str[2] = repeating.code;
    bmp_send_string(str);   // SS_UP(repeating.code)
    if (repeating.mod != KC_NO) {
      str[2] = repeating.mod;
      bmp_send_string(str); // SS_UP(repeating.mod)
    }
  }
#else
  if (repeating.code != KC_NO) {
    unregister_code(repeating.code);
    if (repeating.mod != KC_NO) {
      unregister_code(repeating.mod);
    }
  }
#endif
  repeating.code = repeating.mod = KC_NO;
}

// バッファをクリアする
static void naginata_clear(void) {
  end_repeating_key();
  pressed_key = 0;
  waiting_count = 0; // 文字キーを数える
  reuse_key_state = Off;
}

void ng_send_unicode_string_P(const char *str) {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code(KC_GRAVE);
      send_unicode_string_P(str);
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_LINUX:
      tap_code16(LCTL(KC_SPACE));
      send_unicode_string_P(str);
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_MAC:
#if defined(NG_USE_KAWASEMI)
      // かわせみ専用
      tap_code16(MEH(KC_LANGUAGE_1));   // Control+Option+Shift+(Mac)かな
      send_unicode_string_P(str);
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
#else
      // Ishizuki https://github.com/eswai/Ishizuki が必要
      send_unicode_string_P(str);
#endif
      break;
  }
}

#if defined(NG_BMP)
#       define NG_SEND_KANA(string) bmp_send_string(string)
#elif defined(NG_KANA_FAST)
// 文字列をできるかぎりロールオーバーしながら高速出力
// 出力できる文字は、英小文字、数字、空白、一部の記号、一部の制御文字
//      \b\t\n ,-./0123456789;abcdefghijklmnopqrstuvwxyz
// 出力原理:
//    6KRO(Macを除く) なら、押したキーを最大6個ためて一度に送出し、離すのも一度に出力（最速）
//    6KRO で Mac なら、USBキーコード順にキーが続くかぎり最大6個ためる（やや高速）
//    NKRO なら、USBキーコード順にキーが続くかぎりためて一度に送出し、離すのも一度に出力（やや高速）
static void ng_send_kana_fast(const char *str) {
    if (naginata_config.os == NG_MAC) {
        send_string_P(str);
        return;
    }

    // 文字を取り出しながら最大限まとめて出力
    bool is_nkro = false;
    #ifdef NKRO_ENABLE
        // is_nkro = keyboard_protocol && keymap_config.nkro;   // QMK Firmware 0.23 〜 0.26.x
        // is_nkro = usb_device_state_get_protocol() && keymap_config.nkro;   // QMK Firmware 0.27 〜 0.28.x
        is_nkro = host_can_send_nkro() && keymap_config.nkro;   // QMK Firmware 0.29 〜
    #endif
    #ifdef CONSOLE_ENABLE
        uint keys = 0;
        uint ms = 0;
    #endif

    while (pgm_read_byte(str) != 0) {
        uint8_t len = 0;
        {
            // バッファに入っていない文字が(NKROまたはMacではUSBキーコード昇順で)いくつ続くか探索
            const char *str_copy = str;
            char ascii_code;
            uint8_t last_keycode = 0;
            for ( ; (ascii_code = pgm_read_byte(str_copy++)) != 0; len++) {
                uint8_t keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
                // 送信を区切るべきか
                if (is_key_pressed(keycode) // バッファにある   // QMK Firmware 0.23 〜
                    || ((is_nkro) && keycode < last_keycode)
                    || (!is_nkro && len >= KEYBOARD_REPORT_KEYS)) {
                        break;
                }
                last_keycode = keycode;
            }
        }
        clear_keys();
        for (uint8_t i = 0; i < len; i++, str++) {
            char ascii_code = pgm_read_byte(str);
            uint8_t keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
            // バッファに同じキーがある
            if (i > 0 && is_key_pressed(keycode)) { // QMK Firmware 0.23 〜
                break;
            }
            // バッファにためる
            add_key(keycode);
            #ifdef CONSOLE_ENABLE
                keys++;
                uprintf("%c", ascii_code);
            #endif
        }
        send_keyboard_report();
        #ifdef CONSOLE_ENABLE
            ms += (TAP_CODE_DELAY) > 0 ? (TAP_CODE_DELAY) : 1;
            print(" ");
        #endif
    }
    clear_keys();
    send_keyboard_report();
    #ifdef CONSOLE_ENABLE
        ms += (TAP_CODE_DELAY) > 0 ? (TAP_CODE_DELAY) : 1;
        uprintf("\nSend %u key(s) in %u ms.\n", keys, ms);
    #endif
}
#   define NG_SEND_KANA(string) ng_send_kana_fast(PSTR(string))
#endif

// static void ng_null(void) {}

#if defined(NG_BMP) || defined(NG_KANA_FAST)
// アルファベット
static void ng_send_a(void) { NG_SEND_KANA("a"); } // あ
// static void ng_send_b(void) { NG_SEND_KANA("b"); }
// static void ng_send_c(void) { NG_SEND_KANA("c"); }
// static void ng_send_d(void) { NG_SEND_KANA("d"); }
static void ng_send_e(void) { NG_SEND_KANA("e"); } // え
// static void ng_send_f(void) { NG_SEND_KANA("f"); }
// static void ng_send_g(void) { NG_SEND_KANA("g"); }
// static void ng_send_h(void) { NG_SEND_KANA("h"); }
static void ng_send_i(void) { NG_SEND_KANA("i"); } // い
// static void ng_send_j(void) { NG_SEND_KANA("j"); }
// static void ng_send_k(void) { NG_SEND_KANA("k"); }
// static void ng_send_l(void) { NG_SEND_KANA("l"); }
// static void ng_send_m(void) { NG_SEND_KANA("m"); }
// static void ng_send_n(void) { NG_SEND_KANA("n"); }
static void ng_send_o(void) { NG_SEND_KANA("o"); } // お
// static void ng_send_p(void) { NG_SEND_KANA("p"); }
// static void ng_send_q(void) { NG_SEND_KANA("q"); }
// static void ng_send_r(void) { NG_SEND_KANA("r"); }
// static void ng_send_s(void) { NG_SEND_KANA("s"); }
// static void ng_send_t(void) { NG_SEND_KANA("t"); }
static void ng_send_u(void) { NG_SEND_KANA("u"); } // う
// static void ng_send_v(void) { NG_SEND_KANA("v"); }
// static void ng_send_w(void) { NG_SEND_KANA("w"); }
// static void ng_send_x(void) { NG_SEND_KANA("x"); }
// static void ng_send_y(void) { NG_SEND_KANA("y"); }
// static void ng_send_z(void) { NG_SEND_KANA("z"); }
// 清音
static void ng_send_ka(void) { NG_SEND_KANA("ka"); } // か
static void ng_send_ki(void) { NG_SEND_KANA("ki"); } // き
static void ng_send_ku(void) { NG_SEND_KANA("ku"); } // く
static void ng_send_ke(void) { NG_SEND_KANA("ke"); } // け
static void ng_send_ko(void) { NG_SEND_KANA("ko"); } // こ
static void ng_send_sa(void) { NG_SEND_KANA("sa"); } // さ
static void ng_send_si(void) { NG_SEND_KANA("si"); } // し  ci は Mac日本語入力 で使えない
static void ng_send_su(void) { NG_SEND_KANA("su"); } // す
static void ng_send_se(void) { NG_SEND_KANA("se"); } // せ  ce は Mac日本語入力 で使えない
static void ng_send_so(void) { NG_SEND_KANA("so"); } // そ
static void ng_send_ta(void) { NG_SEND_KANA("ta"); } // た
static void ng_send_ti(void) { NG_SEND_KANA("ti"); } // ち
static void ng_send_tu(void) { NG_SEND_KANA("tu"); } // つ
static void ng_send_te(void) { NG_SEND_KANA("te"); } // て
static void ng_send_to(void) { NG_SEND_KANA("to"); } // と
static void ng_send_na(void) { NG_SEND_KANA("na"); } // な
static void ng_send_ni(void) { NG_SEND_KANA("ni"); } // に
static void ng_send_nu(void) { NG_SEND_KANA("nu"); } // ぬ
static void ng_send_ne(void) { NG_SEND_KANA("ne"); } // ね
static void ng_send_no(void) { NG_SEND_KANA("no"); } // の
static void ng_send_ha(void) { NG_SEND_KANA("ha"); } // は
static void ng_send_hi(void) { NG_SEND_KANA("hi"); } // ひ
static void ng_send_hu(void) { NG_SEND_KANA("hu"); } // ふ
static void ng_send_he(void) { NG_SEND_KANA("he"); } // へ
static void ng_send_ho(void) { NG_SEND_KANA("ho"); } // ほ
static void ng_send_ma(void) { NG_SEND_KANA("ma"); } // ま
static void ng_send_mi(void) { NG_SEND_KANA("mi"); } // み
static void ng_send_mu(void) { NG_SEND_KANA("mu"); } // む
static void ng_send_me(void) { NG_SEND_KANA("me"); } // め
static void ng_send_mo(void) { NG_SEND_KANA("mo"); } // も
static void ng_send_ya(void) { NG_SEND_KANA("ya"); } // や
static void ng_send_yu(void) { NG_SEND_KANA("yu"); } // ゆ
static void ng_send_yo(void) { NG_SEND_KANA("yo"); } // よ
static void ng_send_ra(void) { NG_SEND_KANA("ra"); } // ら
static void ng_send_ri(void) { NG_SEND_KANA("ri"); } // り
static void ng_send_ru(void) { NG_SEND_KANA("ru"); } // る
static void ng_send_re(void) { NG_SEND_KANA("re"); } // れ
static void ng_send_ro(void) { NG_SEND_KANA("ro"); } // ろ
static void ng_send_wa(void) { NG_SEND_KANA("wa"); } // わ
static void ng_send_wo(void) { NG_SEND_KANA("wo"); } // を
static void ng_send_nn(void) { // ん
    switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
        NG_SEND_KANA("xn");
        break;
    default:
        NG_SEND_KANA("nn");
        break;
    }
}
static void ng_send_minus(void) { NG_SEND_KANA("-"); } // ー
// 濁音
static void ng_send_ga(void) { NG_SEND_KANA("ga"); } // が
static void ng_send_gi(void) { NG_SEND_KANA("gi"); } // ぎ
static void ng_send_gu(void) { NG_SEND_KANA("gu"); } // ぐ
static void ng_send_ge(void) { NG_SEND_KANA("ge"); } // げ
static void ng_send_go(void) { NG_SEND_KANA("go"); } // ご
static void ng_send_za(void) { NG_SEND_KANA("za"); } // ざ
static void ng_send_zi(void) { NG_SEND_KANA("zi"); } // じ
static void ng_send_zu(void) { NG_SEND_KANA("zu"); } // ず
static void ng_send_ze(void) { NG_SEND_KANA("ze"); } // ぜ
static void ng_send_zo(void) { NG_SEND_KANA("zo"); } // ぞ
static void ng_send_da(void) { NG_SEND_KANA("da"); } // だ
static void ng_send_di(void) { NG_SEND_KANA("di"); } // ぢ
static void ng_send_du(void) { NG_SEND_KANA("du"); } // づ
static void ng_send_de(void) { NG_SEND_KANA("de"); } // で
static void ng_send_do(void) { NG_SEND_KANA("do"); } // ど
static void ng_send_ba(void) { NG_SEND_KANA("ba"); } // ば
static void ng_send_bi(void) { NG_SEND_KANA("bi"); } // び
static void ng_send_bu(void) { NG_SEND_KANA("bu"); } // ぶ
static void ng_send_be(void) { NG_SEND_KANA("be"); } // べ
static void ng_send_bo(void) { NG_SEND_KANA("bo"); } // ぼ
static void ng_send_vu(void) { NG_SEND_KANA("vu"); } // ヴ
// 半濁音
static void ng_send_pa(void) { NG_SEND_KANA("pa"); } // ぱ
static void ng_send_pi(void) { NG_SEND_KANA("pi"); } // ぴ
static void ng_send_pu(void) { NG_SEND_KANA("pu"); } // ぷ
static void ng_send_pe(void) { NG_SEND_KANA("pe"); } // ぺ
static void ng_send_po(void) { NG_SEND_KANA("po"); } // ぽ
// 小書き
static void ng_send_xya(void) { NG_SEND_KANA("xya"); } // ゃ
static void ng_send_xyu(void) { NG_SEND_KANA("xyu"); } // ゅ
static void ng_send_xyo(void) { NG_SEND_KANA("xyo"); } // ょ
static void ng_send_xa(void)  { NG_SEND_KANA("xa");  } // ぁ
static void ng_send_xi(void)  { NG_SEND_KANA("xi");  } // ぃ
static void ng_send_xu(void)  { NG_SEND_KANA("lu");  } // ぅ
static void ng_send_xe(void)  { NG_SEND_KANA("xe");  } // ぇ
static void ng_send_xo(void)  { NG_SEND_KANA("lo");  } // ぉ
static void ng_send_xwa(void) { NG_SEND_KANA("lwa"); } // ゎ
static void ng_send_xtu(void) { NG_SEND_KANA("ltu"); } // っ
// static void ng_send_xke(void) { NG_SEND_KANA("xke"); } // ヶ
// static void ng_send_xka(void) { NG_SEND_KANA("xka"); } // ヵ
// 清音拗音 濁音拗音 半濁拗音
static void ng_send_sya(void) { NG_SEND_KANA("sya"); } // しゃ
static void ng_send_syu(void) { NG_SEND_KANA("syu"); } // しゅ
static void ng_send_syo(void) { NG_SEND_KANA("syo"); } // しょ
static void ng_send_ja(void)  { NG_SEND_KANA("ja");  } // じゃ
static void ng_send_ju(void)  { NG_SEND_KANA("ju");  } // じゅ
static void ng_send_jo(void)  { NG_SEND_KANA("jo");  } // じょ
static void ng_send_kya(void) { NG_SEND_KANA("kya"); } // きゃ
static void ng_send_kyu(void) { NG_SEND_KANA("kyu"); } // きゅ
static void ng_send_kyo(void) { NG_SEND_KANA("kyo"); } // きょ
static void ng_send_gya(void) { NG_SEND_KANA("gya"); } // ぎゃ
static void ng_send_gyu(void) { NG_SEND_KANA("gyu"); } // ぎゅ
static void ng_send_gyo(void) { NG_SEND_KANA("gyo"); } // ぎょ
static void ng_send_tya(void) { NG_SEND_KANA("tya"); } // ちゃ
static void ng_send_tyu(void) { NG_SEND_KANA("chu"); } // ちゅ
static void ng_send_tyo(void) { NG_SEND_KANA("cho"); } // ちょ
static void ng_send_dya(void) { NG_SEND_KANA("dya"); } // ぢゃ
static void ng_send_dyu(void) { NG_SEND_KANA("dyu"); } // ぢゅ
static void ng_send_dyo(void) { NG_SEND_KANA("dyo"); } // ぢょ
static void ng_send_nya(void) { NG_SEND_KANA("nya"); } // にゃ
static void ng_send_nyu(void) { NG_SEND_KANA("nyu"); } // にゅ
static void ng_send_nyo(void) { NG_SEND_KANA("nyo"); } // にょ
static void ng_send_hya(void) { NG_SEND_KANA("hya"); } // ひゃ
static void ng_send_hyu(void) { NG_SEND_KANA("hyu"); } // ひゅ
static void ng_send_hyo(void) { NG_SEND_KANA("hyo"); } // ひょ
static void ng_send_bya(void) { NG_SEND_KANA("bya"); } // びゃ
static void ng_send_byu(void) { NG_SEND_KANA("byu"); } // びゅ
static void ng_send_byo(void) { NG_SEND_KANA("byo"); } // びょ
static void ng_send_pya(void) { NG_SEND_KANA("pya"); } // ぴゃ
static void ng_send_pyu(void) { NG_SEND_KANA("pyu"); } // ぴゅ
static void ng_send_pyo(void) { NG_SEND_KANA("pyo"); } // ぴょ
static void ng_send_mya(void) { NG_SEND_KANA("mya"); } // みゃ
static void ng_send_myu(void) { NG_SEND_KANA("myu"); } // みゅ
static void ng_send_myo(void) { NG_SEND_KANA("myo"); } // みょ
static void ng_send_rya(void) { NG_SEND_KANA("rya"); } // りゃ
static void ng_send_ryu(void) { NG_SEND_KANA("ryu"); } // りゅ
static void ng_send_ryo(void) { NG_SEND_KANA("ryo"); } // りょ
// 清音外来音 濁音外来音
static void ng_send_ye(void)  { NG_SEND_KANA("ye");  } // いぇ
static void ng_send_thi(void) { NG_SEND_KANA("thi"); } // てぃ
static void ng_send_thu(void) { NG_SEND_KANA("thu"); } // てゅ
static void ng_send_dhi(void) { NG_SEND_KANA("dhi"); } // でぃ
static void ng_send_dhu(void) { NG_SEND_KANA("dhu"); } // でゅ
static void ng_send_twu(void) { NG_SEND_KANA("twu"); } // とぅ
static void ng_send_dwu(void) { NG_SEND_KANA("dwu"); } // どぅ
static void ng_send_sye(void) { NG_SEND_KANA("sye"); } // しぇ
static void ng_send_tye(void) { NG_SEND_KANA("tye"); } // ちぇ
static void ng_send_je(void)  { NG_SEND_KANA("je");  } // じぇ
static void ng_send_dye(void) { NG_SEND_KANA("dye"); } // ぢぇ
static void ng_send_fa(void)  { NG_SEND_KANA("fa");  } // ふぁ
static void ng_send_fi(void)  { NG_SEND_KANA("fi");  } // ふぃ
static void ng_send_fe(void)  { NG_SEND_KANA("fe");  } // ふぇ
static void ng_send_fo(void)  { NG_SEND_KANA("fo");  } // ふぉ
static void ng_send_fyu(void) { NG_SEND_KANA("fyu"); } // ふゅ
static void ng_send_wi(void)  { NG_SEND_KANA("wi");  } // うぃ
static void ng_send_we(void)  { NG_SEND_KANA("we");  } // うぇ
static void ng_send_who(void) { NG_SEND_KANA("who"); } // うぉ
static void ng_send_va(void)  { NG_SEND_KANA("va");  } // ヴぁ
static void ng_send_vi(void)  { NG_SEND_KANA("vi");  } // ヴぃ
static void ng_send_ve(void)  { NG_SEND_KANA("ve");  } // ヴぇ
static void ng_send_vo(void)  { NG_SEND_KANA("vo");  } // ヴぉ
static void ng_send_vyu(void) { NG_SEND_KANA("vyu"); } // ヴゅ
static void ng_send_qa(void)  { NG_SEND_KANA("qa");  } // くぁ
static void ng_send_qi(void)  { NG_SEND_KANA("qi");  } // くぃ
static void ng_send_qe(void)  { NG_SEND_KANA("qe");  } // くぇ
static void ng_send_qo(void)  { NG_SEND_KANA("qo");  } // くぉ
static void ng_send_kuxwa(void) { NG_SEND_KANA("kulwa"); } // くゎ
static void ng_send_gwa(void) { NG_SEND_KANA("gwa"); } // ぐぁ
static void ng_send_gwi(void) { NG_SEND_KANA("gwi"); } // ぐぃ
static void ng_send_gwe(void) { NG_SEND_KANA("gwe"); } // ぐぇ
static void ng_send_gwo(void) { NG_SEND_KANA("gwo"); } // ぐぉ
static void ng_send_guxwa(void) { NG_SEND_KANA("gulwa"); } // ぐゎ
static void ng_send_tsa(void) { NG_SEND_KANA("tsa"); } // つぁ
#else
// アルファベット
static void ng_send_a(void) { tap_code(KC_A); } // あ
static void ng_send_b(void) { tap_code(KC_B); }
// static void ng_send_c(void) { tap_code(KC_C); }
static void ng_send_d(void) { tap_code(KC_D); }
static void ng_send_e(void) { tap_code(KC_E); } // え
static void ng_send_f(void) { tap_code(KC_F); }
static void ng_send_g(void) { tap_code(KC_G); }
static void ng_send_h(void) { tap_code(KC_H); }
static void ng_send_i(void) { tap_code(KC_I); } // い
static void ng_send_j(void) { tap_code(KC_J); }
static void ng_send_k(void) { tap_code(KC_K); }
// static void ng_send_l(void) { tap_code(KC_L); }
static void ng_send_m(void) { tap_code(KC_M); }
static void ng_send_n(void) { tap_code(KC_N); }
static void ng_send_o(void) { tap_code(KC_O); } // お
static void ng_send_p(void) { tap_code(KC_P); }
static void ng_send_q(void) { tap_code(KC_Q); }
static void ng_send_r(void) { tap_code(KC_R); }
static void ng_send_s(void) { tap_code(KC_S); }
static void ng_send_t(void) { tap_code(KC_T); }
static void ng_send_u(void) { tap_code(KC_U); } // う
static void ng_send_v(void) { tap_code(KC_V); }
static void ng_send_w(void) { tap_code(KC_W); }
static void ng_send_x(void) { tap_code(KC_X); }
static void ng_send_y(void) { tap_code(KC_Y); }
static void ng_send_z(void) { tap_code(KC_Z); }
// 清音
static void ng_send_ka(void) { ng_send_k(); ng_send_a(); } // か
static void ng_send_ki(void) { ng_send_k(); ng_send_i(); } // き
static void ng_send_ku(void) { ng_send_k(); ng_send_u(); } // く
static void ng_send_ke(void) { ng_send_k(); ng_send_e(); } // け
static void ng_send_ko(void) { ng_send_k(); ng_send_o(); } // こ
static void ng_send_sa(void) { ng_send_s(); ng_send_a(); } // さ
static void ng_send_si(void) { ng_send_s(); ng_send_i(); } // し
static void ng_send_su(void) { ng_send_s(); ng_send_u(); } // す
static void ng_send_se(void) { ng_send_s(); ng_send_e(); } // せ
static void ng_send_so(void) { ng_send_s(); ng_send_o(); } // そ
static void ng_send_ta(void) { ng_send_t(); ng_send_a(); } // た
static void ng_send_ti(void) { ng_send_t(); ng_send_i(); } // ち
static void ng_send_tu(void) { ng_send_t(); ng_send_u(); } // つ
static void ng_send_te(void) { ng_send_t(); ng_send_e(); } // て
static void ng_send_to(void) { ng_send_t(); ng_send_o(); } // と
static void ng_send_na(void) { ng_send_n(); ng_send_a(); } // な
static void ng_send_ni(void) { ng_send_n(); ng_send_i(); } // に
static void ng_send_nu(void) { ng_send_n(); ng_send_u(); } // ぬ
static void ng_send_ne(void) { ng_send_n(); ng_send_e(); } // ね
static void ng_send_no(void) { ng_send_n(); ng_send_o(); } // の
static void ng_send_ha(void) { ng_send_h(); ng_send_a(); } // は
static void ng_send_hi(void) { ng_send_h(); ng_send_i(); } // ひ
static void ng_send_hu(void) { ng_send_h(); ng_send_u(); } // ふ
static void ng_send_he(void) { ng_send_h(); ng_send_e(); } // へ
static void ng_send_ho(void) { ng_send_h(); ng_send_o(); } // ほ
static void ng_send_ma(void) { ng_send_m(); ng_send_a(); } // ま
static void ng_send_mi(void) { ng_send_m(); ng_send_i(); } // み
static void ng_send_mu(void) { ng_send_m(); ng_send_u(); } // む
static void ng_send_me(void) { ng_send_m(); ng_send_e(); } // め
static void ng_send_mo(void) { ng_send_m(); ng_send_o(); } // も
static void ng_send_ya(void) { ng_send_y(); ng_send_a(); } // や
static void ng_send_yu(void) { ng_send_y(); ng_send_u(); } // ゆ
static void ng_send_yo(void) { ng_send_y(); ng_send_o(); } // よ
static void ng_send_ra(void) { ng_send_r(); ng_send_a(); } // ら
static void ng_send_ri(void) { ng_send_r(); ng_send_i(); } // り
static void ng_send_ru(void) { ng_send_r(); ng_send_u(); } // る
static void ng_send_re(void) { ng_send_r(); ng_send_e(); } // れ
static void ng_send_ro(void) { ng_send_r(); ng_send_o(); } // ろ
static void ng_send_wa(void) { ng_send_w(); ng_send_a(); } // わ
static void ng_send_wo(void) { ng_send_w(); ng_send_o(); } // を
static void ng_send_nn(void) { ng_send_n(); ng_send_n(); } // ん
static void ng_send_minus(void) { tap_code(KC_MINUS); } // ー
// 濁音
static void ng_send_ga(void) { ng_send_g(); ng_send_a(); } // が
static void ng_send_gi(void) { ng_send_g(); ng_send_i(); } // ぎ
static void ng_send_gu(void) { ng_send_g(); ng_send_u(); } // ぐ
static void ng_send_ge(void) { ng_send_g(); ng_send_e(); } // げ
static void ng_send_go(void) { ng_send_g(); ng_send_o(); } // ご
static void ng_send_za(void) { ng_send_z(); ng_send_a(); } // ざ
static void ng_send_zi(void) { ng_send_z(); ng_send_i(); } // じ
static void ng_send_zu(void) { ng_send_z(); ng_send_u(); } // ず
static void ng_send_ze(void) { ng_send_z(); ng_send_e(); } // ぜ
static void ng_send_zo(void) { ng_send_z(); ng_send_o(); } // ぞ
static void ng_send_da(void) { ng_send_d(); ng_send_a(); } // だ
static void ng_send_di(void) { ng_send_d(); ng_send_i(); } // ぢ
static void ng_send_du(void) { ng_send_d(); ng_send_u(); } // づ
static void ng_send_de(void) { ng_send_d(); ng_send_e(); } // で
static void ng_send_do(void) { ng_send_d(); ng_send_o(); } // ど
static void ng_send_ba(void) { ng_send_b(); ng_send_a(); } // ば
static void ng_send_bi(void) { ng_send_b(); ng_send_i(); } // び
static void ng_send_bu(void) { ng_send_b(); ng_send_u(); } // ぶ
static void ng_send_be(void) { ng_send_b(); ng_send_e(); } // べ
static void ng_send_bo(void) { ng_send_b(); ng_send_o(); } // ぼ
static void ng_send_vu(void) { ng_send_v(); ng_send_u(); } // ヴ
// 半濁音
static void ng_send_pa(void) { ng_send_p(); ng_send_a(); } // ぱ
static void ng_send_pi(void) { ng_send_p(); ng_send_i(); } // ぴ
static void ng_send_pu(void) { ng_send_p(); ng_send_u(); } // ぷ
static void ng_send_pe(void) { ng_send_p(); ng_send_e(); } // ぺ
static void ng_send_po(void) { ng_send_p(); ng_send_o(); } // ぽ
// 小書き
static void ng_send_xya(void) { ng_send_x(); ng_send_ya(); } // ゃ
static void ng_send_xyu(void) { ng_send_x(); ng_send_yu(); } // ゅ
static void ng_send_xyo(void) { ng_send_x(); ng_send_yo(); } // ょ
static void ng_send_xa(void)  { ng_send_x(); ng_send_a();  } // ぁ
static void ng_send_xi(void)  { ng_send_x(); ng_send_i();  } // ぃ
static void ng_send_xu(void)  { ng_send_x(); ng_send_u();  } // ぅ
static void ng_send_xe(void)  { ng_send_x(); ng_send_e();  } // ぇ
static void ng_send_xo(void)  { ng_send_x(); ng_send_o();  } // ぉ
static void ng_send_xwa(void) { ng_send_x(); ng_send_wa(); } // ゎ
static void ng_send_xtu(void) { ng_send_x(); ng_send_tu(); } // っ
static void ng_send_xke(void) { ng_send_x(); ng_send_ke(); } // ヶ
static void ng_send_xka(void) { ng_send_x(); ng_send_ka(); } // ヵ
// 清音拗音 濁音拗音 半濁拗音
static void ng_send_sya(void) { ng_send_s(); ng_send_ya(); } // しゃ
static void ng_send_syu(void) { ng_send_s(); ng_send_yu(); } // しゅ
static void ng_send_syo(void) { ng_send_s(); ng_send_yo(); } // しょ
static void ng_send_ja(void)  { ng_send_j(); ng_send_a();  } // じゃ
static void ng_send_ju(void)  { ng_send_j(); ng_send_u();  } // じゅ
static void ng_send_jo(void)  { ng_send_j(); ng_send_o();  } // じょ
static void ng_send_kya(void) { ng_send_k(); ng_send_ya(); } // きゃ
static void ng_send_kyu(void) { ng_send_k(); ng_send_yu(); } // きゅ
static void ng_send_kyo(void) { ng_send_k(); ng_send_yo(); } // きょ
static void ng_send_gya(void) { ng_send_g(); ng_send_ya(); } // ぎゃ
static void ng_send_gyu(void) { ng_send_g(); ng_send_yu(); } // ぎゅ
static void ng_send_gyo(void) { ng_send_g(); ng_send_yo(); } // ぎょ
static void ng_send_tya(void) { ng_send_t(); ng_send_ya(); } // ちゃ
static void ng_send_tyu(void) { ng_send_t(); ng_send_yu(); } // ちゅ
static void ng_send_tyo(void) { ng_send_t(); ng_send_yo(); } // ちょ
static void ng_send_dya(void) { ng_send_d(); ng_send_ya(); } // ぢゃ
static void ng_send_dyu(void) { ng_send_d(); ng_send_yu(); } // ぢゅ
static void ng_send_dyo(void) { ng_send_d(); ng_send_yo(); } // ぢょ
static void ng_send_nya(void) { ng_send_n(); ng_send_ya(); } // にゃ
static void ng_send_nyu(void) { ng_send_n(); ng_send_yu(); } // にゅ
static void ng_send_nyo(void) { ng_send_n(); ng_send_yo(); } // にょ
static void ng_send_hya(void) { ng_send_h(); ng_send_ya(); } // ひゃ
static void ng_send_hyu(void) { ng_send_h(); ng_send_yu(); } // ひゅ
static void ng_send_hyo(void) { ng_send_h(); ng_send_yo(); } // ひょ
static void ng_send_bya(void) { ng_send_b(); ng_send_ya(); } // びゃ
static void ng_send_byu(void) { ng_send_b(); ng_send_yu(); } // びゅ
static void ng_send_byo(void) { ng_send_b(); ng_send_yo(); } // びょ
static void ng_send_pya(void) { ng_send_p(); ng_send_ya(); } // ぴゃ
static void ng_send_pyu(void) { ng_send_p(); ng_send_yu(); } // ぴゅ
static void ng_send_pyo(void) { ng_send_p(); ng_send_yo(); } // ぴょ
static void ng_send_mya(void) { ng_send_m(); ng_send_ya(); } // みゃ
static void ng_send_myu(void) { ng_send_m(); ng_send_yu(); } // みゅ
static void ng_send_myo(void) { ng_send_m(); ng_send_yo(); } // みょ
static void ng_send_rya(void) { ng_send_r(); ng_send_ya(); } // りゃ
static void ng_send_ryu(void) { ng_send_r(); ng_send_yu(); } // りゅ
static void ng_send_ryo(void) { ng_send_r(); ng_send_yo(); } // りょ
// 清音外来音 濁音外来音
static void ng_send_ye(void)  { ng_send_y(); ng_send_e();  } // いぇ
static void ng_send_thi(void) { ng_send_t(); ng_send_hi(); } // てぃ
static void ng_send_thu(void) { ng_send_t(); ng_send_hu(); } // てゅ
static void ng_send_dhi(void) { ng_send_d(); ng_send_hi(); } // でぃ
static void ng_send_dhu(void) { ng_send_d(); ng_send_hu(); } // でゅ
static void ng_send_twu(void) { ng_send_t(); ng_send_w(); ng_send_u(); } // とぅ
static void ng_send_dwu(void) { ng_send_d(); ng_send_w(); ng_send_u(); } // どぅ
static void ng_send_sye(void) { ng_send_s(); ng_send_ye(); } // しぇ
static void ng_send_tye(void) { ng_send_t(); ng_send_ye(); } // ちぇ
static void ng_send_je(void)  { ng_send_j(); ng_send_e();  } // じぇ
static void ng_send_dye(void) { ng_send_d(); ng_send_ye(); } // ぢぇ
static void ng_send_fa(void)  { ng_send_f(); ng_send_a();  } // ふぁ
static void ng_send_fi(void)  { ng_send_f(); ng_send_i();  } // ふぃ
static void ng_send_fe(void)  { ng_send_f(); ng_send_e();  } // ふぇ
static void ng_send_fo(void)  { ng_send_f(); ng_send_o();  } // ふぉ
static void ng_send_fyu(void) { ng_send_f(); ng_send_yu(); } // ふゅ
static void ng_send_wi(void)  { ng_send_w(); ng_send_i();  } // うぃ
static void ng_send_we(void)  { ng_send_w(); ng_send_e();  } // うぇ
static void ng_send_who(void) { ng_send_w(); ng_send_ho(); } // うぉ
static void ng_send_va(void)  { ng_send_v(); ng_send_a();  } // ヴぁ
static void ng_send_vi(void)  { ng_send_v(); ng_send_i();  } // ヴぃ
static void ng_send_ve(void)  { ng_send_v(); ng_send_e();  } // ヴぇ
static void ng_send_vo(void)  { ng_send_v(); ng_send_o();  } // ヴぉ
static void ng_send_vyu(void) { ng_send_v(); ng_send_yu(); } // ヴゅ
static void ng_send_qa(void)  { ng_send_q(); ng_send_a();  } // くぁ
static void ng_send_qi(void)  { ng_send_q(); ng_send_i();  } // くぃ
static void ng_send_qe(void)  { ng_send_q(); ng_send_e();  } // くぇ
static void ng_send_qo(void)  { ng_send_q(); ng_send_o();  } // くぉ
static void ng_send_kuxwa(void) { ng_send_ku(); ng_send_xwa(); } // くゎ
static void ng_send_gwa(void) { ng_send_g(); ng_send_wa(); } // ぐぁ
static void ng_send_gwi(void) { ng_send_g(); ng_send_wi(); } // ぐぃ
static void ng_send_gwe(void) { ng_send_g(); ng_send_we(); } // ぐぇ
static void ng_send_gwo(void) { ng_send_g(); ng_send_wo(); } // ぐぉ
static void ng_send_guxwa(void) { ng_send_gu(); ng_send_xwa(); } // ぐゎ
static void ng_send_tsa(void) { ng_send_t(); ng_send_sa(); } // つぁ
#endif

static void ng_ime_complete(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_IOS:
      // (Mac)英数 → (Mac)かな
      bmp_send_string(SS_TAP(X_LANGUAGE_2)SS_TAP(X_LANGUAGE_1));
      break;
    case NG_LINUX:
      bmp_send_string(SS_TAP(X_LANGUAGE_2)SS_TAP(X_INTERNATIONAL_2));
      break;
    default:
      bmp_send_string("/\n\b");
      break;
  }
#elif defined(NG_USE_DIC)
  switch (naginata_config.os) {
    case NG_LINUX:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    default:
      tap_code(KC_SLASH);
      tap_code(KC_ENTER);
      tap_code(KC_BACKSPACE);
      break;
  }
#else
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
# if defined(NG_USE_KAWASEMI)
      tap_code16(MEH(KC_LANGUAGE_1));   // Control+Option+Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
# else
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code16(LSFT(KC_LANGUAGE_1));  // Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
# endif
      break;
  }
#endif
}

static void ng_ime_cancel(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      for (uint8_t c = 0; c < 4; c++) {
        bmp_send_string(SS_TAP(X_ESCAPE));
      }
      break;
    case NG_MAC:
      bmp_send_string(SS_TAP(X_NUM_LOCK));
      break;
    case NG_IOS:
      bmp_send_string(SS_TAP(X_ESCAPE));
      break;
  }
#elif defined(NG_USE_DIC)
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      for (uint8_t c = 0; c < 4; c++) {
        tap_code(KC_ESCAPE);
      }
      break;
    case NG_MAC:
      tap_code(KC_NUM_LOCK);
      break;
  }
#else
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));  // Shift+Ctrl+無変換x2
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));
      break;
    case NG_MAC:
      tap_code(KC_NUM_LOCK);
# if !defined(NG_USE_KAWASEMI)
      // ライブ変換対応
      tap_code(KC_NUM_LOCK);
      tap_code(KC_NUM_LOCK);
# endif
      break;
  }
#endif
}

#if defined(NG_BMP) || defined(NG_USE_DIC)
// 辞書式用
static void dic_send_string(const char *str) {
#   if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(str);
        bmp_send_string(SS_DELAY(120));
        bmp_send_string(" ");
        ng_ime_complete();
        break;
    default:
        bmp_send_string(str);
        bmp_send_string(" \n");
        break;
    }
#   else
    if (naginata_config.os == NG_LINUX) {
        send_string_with_delay_P(str, 8);
    } else {
        send_string_P(str);
    }
    tap_code(KC_SPACE);
    tap_code(KC_ENTER);
#   endif
}
#endif

// ---- カーソル移動・編集 ----

static void ng_space_or_enter(void) {
#if defined (SHIFT_ALONE_TIMEOUT_MS)
  if (ng_center_keycode == KC_NO || timer_elapsed(ng_last_pressed_ms) >= (SHIFT_ALONE_TIMEOUT_MS)) return;
#else
  if (ng_center_keycode == KC_NO) return;
#endif
#if defined(NG_BMP)
  char str[4] = "\1\1\0\0";
  str[2] = ng_center_keycode;
  if (center_shift_count > 1) {
    bmp_send_string(SS_DOWN(X_LSFT));
    bmp_send_string(str); // SS_TAP(ng_center_keycode);
    bmp_send_string(SS_UP(X_LSFT));
  } else {
    bmp_send_string(str); // SS_TAP(ng_center_keycode);
  }
#else
  if (center_shift_count > 1) {
    tap_code16(LSFT(ng_center_keycode));
  } else {
    tap_code(ng_center_keycode);
  }
#endif
  ng_center_keycode = KC_NO;
}

static void ng_backspace_with_repeat(void) { // {BS}
  repeating.code = KC_BACKSPACE;
#if defined(NG_BMP)
  bmp_send_string(SS_DOWN(X_BACKSPACE));
#else
  register_code(repeating.code);
#endif
}

static void ng_delete_with_repeat(void) { // {Del}
  repeating.code = KC_DELETE;
#if defined(NG_BMP)
  bmp_send_string(SS_DOWN(X_DELETE));
#else
  register_code(repeating.code);
#endif
}

static void ng_enter(void) { // {Enter}
#if defined(NG_BMP)
    bmp_send_string("\n");
#else
    tap_code(KC_ENTER);
#endif
}

// リピート対応の方向キー移動
// リピート中を示す変数を更新
static void ng_move_cursor_with_repeat(bool shift, uint8_t code, uint8_t count) {
  if (code == KC_NO)  return;

  if (shift) {
    repeating.mod = KC_LEFT_SHIFT;
#if defined(NG_BMP)
    bmp_send_string(SS_DOWN(X_LSFT));
#else
    register_code(repeating.mod);
#endif
  }
  repeating.code = code;
  for (uint8_t i = count; i > 1; i--) {
#if defined(NG_BMP)
    char str[4] = "\1\1\0\0";
    str[2] = code;
    bmp_send_string(str); // SS_TAP(code)
#else
    tap_code(code);
#endif
  }
#if defined(NG_BMP)
  char str[4] = "\1\2\0\0";
  str[2] = code;
  bmp_send_string(str); // SS_DOWN(code)
#else
  register_code(code);
#endif
}

static uint8_t convert_ty(uint8_t code) {
  if (!naginata_config.tategaki) {
    switch (code) {
      case KC_LEFT:   code = KC_DOWN;   break;
      case KC_DOWN:   code = KC_RIGHT;  break;
      case KC_RIGHT:  code = KC_UP;     break;
      case KC_UP:     code = KC_LEFT;   break;
      default:    break;
    }
  }
  return code;
}
// リピート対応の縦横変換を伴う方向キー移動
static void ng_move_cursor_with_ty_repeat(bool shift, uint8_t code, uint8_t count) {
  ng_move_cursor_with_repeat(shift, convert_ty(code), count);
}
static void ng_1_back_cursor_r(void) { // {↑}
    ng_move_cursor_with_ty_repeat(false, KC_UP, 1);
}
static void ng_s1_back_cursor_r(void) { // +{↑}
    ng_move_cursor_with_ty_repeat(true, KC_UP, 1);
}
static void ng_s7_back_cursor_r(void) { // +{↑ 7}
    ng_move_cursor_with_ty_repeat(true, KC_UP, 7);
}
static void ng_1_forward_cursor_r(void) { // {↓}
    ng_move_cursor_with_ty_repeat(false, KC_DOWN, 1);
}
static void ng_s1_forward_cursor_r(void) { // +{↓}
    ng_move_cursor_with_ty_repeat(true, KC_DOWN, 1);
}
static void ng_s7_forward_cursor_r(void) { // +{↓ 7}
    ng_move_cursor_with_ty_repeat(true, KC_DOWN, 7);
}
static void ng_1_previous_line_r(void) { // {→}
    ng_move_cursor_with_ty_repeat(false, KC_RIGHT, 1);
}
static void ng_s1_previous_line_r(void) { // +{→}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 1);
}
static void ng_s5_previous_line_r(void) { // +{→ 5}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 5);
}
static void ng_s20_previous_line_r(void) { // +{→ 20}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 20);
}
static void ng_1_next_line_r(void) { // {←}
    ng_move_cursor_with_ty_repeat(false, KC_LEFT, 1);
}
static void ng_s1_next_line_r(void) { // +{←}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 1);
}
static void ng_s5_next_line_r(void) { // +{← 5}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 5);
}
static void ng_s20_next_line_r(void) { // +{← 20}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 20);
}

static void ng_back_cursor(void) {
  ng_1_back_cursor_r();
  end_repeating_key();
}
static void ng_forward_cursor(void) {
  ng_1_forward_cursor_r();
  end_repeating_key();
}
static void ng_next_line(void) {
  ng_1_next_line_r();
  end_repeating_key();
}
static void ng_previous_line(void) {
  ng_1_previous_line_r();
  end_repeating_key();
}
static void ng_home(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_TAP(X_HOME));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_HOME)SS_DELAY(LINUX_WAIT_MS)SS_UP(X_HOME));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_LCTL("a"));
      break;
  }
#else
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
#endif
}
static void ng_end(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_TAP(X_END));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_END)SS_DELAY(LINUX_WAIT_MS)SS_UP(X_END));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_LCTL("e"));
      break;
  }
#else
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
#endif
}

static void ng_cut(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LCTL("x"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_LCTL)"x"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
      bmp_send_string(SS_LCMD("x"));
      break;
    case NG_IOS:
      bmp_send_string(SS_LCMD("x")SS_DELAY(60));
      break;
  }
#else
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
#endif
}

static void ng_copy(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LCTL("c"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_LCTL)"c"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_LCMD("c"));
      break;
  }
#else
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
#endif
}

static void ng_paste(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_DOWN(X_LCTL)"v"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_LINUX:
      if (!get_usb_enabled()) {
        bmp_send_string(SS_DELAY(16));
      }
      bmp_send_string(SS_DOWN(X_LCTL)"v"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
      break;
    case NG_MAC:
      bmp_send_string(SS_LCMD("v")SS_DELAY(135));
      break;
    case NG_IOS:
      bmp_send_string(SS_LCMD("v")SS_DELAY(220));
      break;
  }
#elif defined(NG_USE_DIC)
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16_delay(LCTL(KC_V), LINUX_WAIT_MS);
      break;
    case NG_LINUX:
      wait_ms(48);
      tap_code16_delay(LCTL(KC_V), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_V));
      wait_ms(305);
      break;
  }
#else
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
#endif
}

static void copy_spc_to_clipboard(void) {
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_LINUX:
        if (get_usb_enabled()) {
            bmp_send_string(" "SS_DELAY(16));
            bmp_send_string(SS_DOWN(X_LSFT));
            ng_back_cursor();
            bmp_send_string(SS_UP(X_LSFT));
            bmp_send_string(SS_DELAY(16));
            ng_cut();
        }
        // LinuxとBluetooth接続した時は動作しないので省略
        break;
    case NG_IOS:
        bmp_send_string(" "SS_DELAY(BMP_DELAY));
        bmp_send_string(SS_DOWN(X_LSFT));
        ng_back_cursor();
        bmp_send_string(SS_UP(X_LSFT));
        ng_cut();
        break;
    default:
        bmp_send_string(" "SS_DOWN(X_LSFT));
        ng_back_cursor();
        bmp_send_string(SS_UP(X_LSFT));
        ng_cut();
        break;
    }
#else
    switch (naginata_config.os) {
    case NG_LINUX:
        wait_ms(8);
        tap_code_delay(KC_SPACE, 16);
        add_mods(MOD_BIT(KC_LEFT_SHIFT));
        ng_back_cursor();
        del_mods(MOD_BIT(KC_LEFT_SHIFT));
        wait_ms(16); // 確実に動作させるため
        ng_cut();
        break;
    default:
        tap_code(KC_SPACE);
        add_mods(MOD_BIT(KC_LEFT_SHIFT));
        ng_back_cursor();
        del_mods(MOD_BIT(KC_LEFT_SHIFT));
        ng_cut();
        break;
    }
#endif
}

static void ng_katakana(void) {
#if defined(NG_BMP)
  bmp_send_string(SS_TAP(X_F7));
#else
  tap_code(KC_F7);
#endif
}

static void ng_hiragana(void) {
#if defined(NG_BMP)
  bmp_send_string(SS_TAP(X_F6));
#else
  tap_code(KC_F6);
#endif
}

static void ng_save(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LCTL("s"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_LCTL)"s"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
      bmp_send_string(SS_LCMD("s"));
      break;
    case NG_IOS:
      break;
  }
#else
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
#endif
}

static void ng_undo(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LCTL("z"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_LCTL)"z"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_LCMD("z"));
      break;
  }
#else
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
#endif
}

static void ng_redo(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LCTL("y"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_DOWN(X_LCTL)"y"SS_DELAY(LINUX_WAIT_MS)SS_UP(X_LCTL));
        // 無線接続時、2秒以上キーを押していない状態で出力するとSS_DELAY()が働かないが、
        // 薙刀式の使用には問題ない
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_LCMD(SS_LSFT("z")));
      break;
  }
#else
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
#endif
}

static void ng_eof(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LSFT(SS_LCTL(SS_TAP(X_INTERNATIONAL_4)SS_TAP(X_INTERNATIONAL_4)))); // Shift+Ctrl+変換 x2
      bmp_send_string(SS_LCTL(SS_TAP(X_END)));
      break;
    case NG_LINUX:
      ng_ime_complete();
      bmp_send_string(SS_LCTL(SS_TAP(X_END)));
      break;
    case NG_MAC:
      // (Mac)英数 → Shift+(Mac)かな → (Mac)かな
      bmp_send_string(SS_TAP(X_LANGUAGE_2)SS_LSFT(SS_TAP(X_LANGUAGE_1))SS_TAP(X_LANGUAGE_1));
      bmp_send_string(SS_LCMD(SS_TAP(X_LEFT)SS_TAP(X_DOWN)));
      break;
    case NG_IOS:
      // (Mac)英数 → (Mac)かな
      ng_ime_complete();
      bmp_send_string(SS_LCMD(SS_TAP(X_LEFT)SS_TAP(X_DOWN)));
      break;
  }
#elif defined(NG_USE_DIC)
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code16(LCTL(KC_END));
      break;
    case NG_LINUX:
      ng_ime_complete();
      tap_code16_delay(LCTL(KC_END), LINUX_WAIT_MS);
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code16(LSFT(KC_LANGUAGE_1));  // Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      tap_code16(LCMD(KC_LEFT));
      tap_code16(LCMD(KC_DOWN));
      break;
  }
#else
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
#endif
}

static void ng_saihenkan(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string(SS_LWIN("/"));
      break;
    case NG_LINUX:
      bmp_send_string(SS_TAP(X_INTERNATIONAL_4)); // 変換
      break;
    case NG_MAC:
    case NG_IOS:
      bmp_send_string(SS_TAP(X_LANGUAGE_1)SS_TAP(X_LANGUAGE_1));
      break;
  }
#else
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
#endif
}

static void ng_edit_touten(void) { // 、
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(",");
        ng_ime_complete();
        break;
    default:
        bmp_send_string(",\n");
        break;
    }
#else
    tap_code(KC_COMMA);
    tap_code(KC_ENTER);
#endif
}
static void ng_edit_kuten(void) { // 。
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(".");
        ng_ime_complete();
        break;
    default:
        bmp_send_string(".\n");
        break;
    }
#else
    tap_code(KC_DOT);
    tap_code(KC_ENTER);
#endif
}

// 編集モード
static void ng_middle_dot(void) { // ・
#if defined(NG_BMP)
    bmp_send_string("/");
#else
    tap_code(KC_SLASH);
#endif
}
static void ng_question_mark(void) { // ？{改行}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(SS_LSFT("/"));
        ng_ime_complete();
        break;
    default:
        bmp_send_string(SS_LSFT("/")"\n");
        break;
    }
#else
    tap_code16(LSFT(KC_SLASH));
    tap_code(KC_ENTER);
#endif
}
static void ng_exclamation_mark(void) { // ！{改行}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(SS_LSFT("1"));
        ng_ime_complete();
        break;
    default:
        bmp_send_string(SS_LSFT("1")"\n");
        break;
    }
#else
    tap_code16(LSFT(KC_1));
    tap_code(KC_ENTER);
#endif
}
static void ng_solidus(void) { // ／{改行}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(SS_LOPT("/"));
        ng_ime_complete();
        break;
    default:
        ng_ime_complete();
        bmp_send_string("/"SS_TAP(X_F9)"\n");
        break;
    }
#elif defined(NG_USE_DIC)
    ng_ime_complete();
    tap_code(KC_SLASH);
    tap_code(KC_F9);
    tap_code(KC_ENTER);
#else
    ng_send_unicode_string_P(PSTR("／"));
#endif
}
static void ng_white_circle(void) { // ○{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagimaru")); // "○"
#else
    ng_send_unicode_string_P(PSTR("○"));
#endif
}
static void ng_vertical_line(void) { // ｜{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagitase")); // "｜"
#else
    ng_send_unicode_string_P(PSTR("｜"));
#endif
}
static void ng_ellipsis(void) { // ……{改行}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_IOS:
        bmp_send_string(SS_LOPT(";;"));
        ng_ime_complete();
        break;
    default:
        ng_ime_complete();
        dic_send_string(PSTR("nagitete")); // "……"
        break;
    }
#elif defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagitete")); // "……"
#else
    ng_send_unicode_string_P(PSTR("……"));
#endif
}
static void ng_bar(void) { // ――{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagiyose")); // "──"
#else
    ng_send_unicode_string_P(PSTR("──"));
#endif
}

static void ng_edit_s_home(void) { // +{Home}
#if defined(NG_BMP)
    bmp_send_string(SS_DOWN(X_LSFT));
    ng_home();
    bmp_send_string(SS_UP(X_LSFT));
#else
    add_mods(MOD_BIT(KC_LEFT_SHIFT));
    ng_home();
    del_mods(MOD_BIT(KC_LEFT_SHIFT));
#endif
}
static void ng_edit_s_end(void) { // +{End}
#if defined(NG_BMP)
    bmp_send_string(SS_DOWN(X_LSFT));
    ng_end();
    bmp_send_string(SS_UP(X_LSFT));
#else
    add_mods(MOD_BIT(KC_LEFT_SHIFT));
    ng_end();
    del_mods(MOD_BIT(KC_LEFT_SHIFT));
#endif
}
static void ng_edit_kakutei_end(void) { // {Enter}{End}
    ng_ime_complete();
    ng_end();
}
static void ng_edit_delete_to_end(void) { // +{End}{BS}
#if defined(NG_BMP)
    ng_edit_s_end();    // +{End}
    bmp_send_string("\b");
#else
    ng_edit_s_end();    // +{End}
    tap_code(KC_BACKSPACE);
#endif
}

static void ng_edit_3_space(void) { // {Space 3}
#if defined(NG_BMP)
    ng_ime_complete();
    bmp_send_string("   ");
#else
    ng_ime_complete();
    tap_code(KC_SPACE);
    tap_code(KC_SPACE);
    tap_code(KC_SPACE);
#endif
}
static void ng_edit_separate_line(void) { // 　　　×　　　×　　　×{改行 2}
#if defined(NG_BMP)
    ng_edit_3_space();  // {Space 3}
    dic_send_string(PSTR("nagibatu")); // "　　　×　　　×　　　×"
    bmp_send_string("\n");
#elif defined(NG_USE_DIC)
    ng_edit_3_space();  // {Space 3}
    dic_send_string(PSTR("nagibatu")); // "　　　×　　　×　　　×"
    tap_code(KC_ENTER);
#else
    ng_send_unicode_string_P(PSTR("　　　×　　　×　　　×"));
    tap_code(KC_ENTER);
#endif
}
static void ng_edit_togaki(void) { // {Home}{改行}{Space 3}{←}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_LINUX:
        ng_home();
        bmp_send_string("\n"SS_DELAY(BMP_DELAY)"   ");
        ng_next_line();
        break;
    default:
        ng_home();
        bmp_send_string("\n   ");
        ng_next_line();
        break;
    }
#else
    ng_home();
    tap_code_delay(KC_ENTER, 16);
    tap_code(KC_SPACE);
    tap_code(KC_SPACE);
    tap_code(KC_SPACE);
    ng_next_line();
#endif
}
static void ng_edit_serifu(void) { // {Home}{改行}{Space 1}{←}
#if defined(NG_BMP)
    switch (naginata_config.os) {
    case NG_LINUX:
        ng_home();
        bmp_send_string("\n"SS_DELAY(BMP_DELAY)" ");
        ng_next_line();
        break;
    default:
        ng_home();
        bmp_send_string("\n ");
        ng_next_line();
        break;
    }
#else
    ng_home();
    tap_code_delay(KC_ENTER, 16);
    tap_code(KC_SPACE);
    ng_next_line();
#endif
}
static void ng_edit_togaki_zengyo(void) { // {Home}{→}{End}{Del 4}{←}
#if defined(NG_BMP)
    ng_home();
    ng_previous_line();
    ng_end();
    bmp_send_string(SS_TAP(X_DELETE)SS_TAP(X_DELETE)SS_TAP(X_DELETE)SS_TAP(X_DELETE));
    ng_next_line();
#else
    ng_home();
    ng_previous_line();
    ng_end();
    tap_code(KC_DELETE);
    tap_code(KC_DELETE);
    tap_code(KC_DELETE);
    tap_code(KC_DELETE);
    ng_next_line();
#endif
}
static void ng_edit_serifu_zengyo(void) { // {Home}{→}{End}{Del 2}{←}
#if defined(NG_BMP)
    ng_home();
    ng_previous_line();
    ng_end();
    bmp_send_string(SS_TAP(X_DELETE)SS_TAP(X_DELETE));
    ng_next_line();
#else
    ng_home();
    ng_previous_line();
    ng_end();
    tap_code(KC_DELETE);
    tap_code(KC_DELETE);
    ng_next_line();
#endif
}

static void ng_edit_kakutei_down(void) { // {改行}{↓}
    ng_ime_complete();
    ng_forward_cursor();
}
static void ng_edit_kakutei_left(void) { // {改行}{←}
    ng_ime_complete();
    ng_next_line();
}

static void ng_parenthesis(void) { // (){改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagimaka")); // "（）"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("（）"));
    ng_back_cursor();
#endif
}
static void ng_corner_bracket(void) { // 「」{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagikaka")); // "「」"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("「」"));
    ng_back_cursor();
#endif
}
static void ng_white_corner_bracket(void) { // 『』{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("naginika")); // "『』"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("『』"));
    ng_back_cursor();
#endif
}
static void ng_black_lenticular_bracket(void) { // 【】{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagisuka")); // "【】"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("【】"));
    ng_back_cursor();
#endif
}
static void ng_double_angle_bracket(void) { // 《》{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("naginiya")); // "《》"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("《》"));
    ng_back_cursor();
#endif
}
static void ng_edit_next_line_corner_bracket(void) { // {改行}{End}{改行}「」{改行}{↑}
#if defined(NG_BMP)
    ng_ime_complete();
    ng_end();
    bmp_send_string("\n");
    ng_corner_bracket(); // 「」{改行}{↑}
#else
    ng_ime_complete();
    ng_end();
    tap_code(KC_ENTER);
    ng_corner_bracket(); // 「」{改行}{↑}
#endif
}
static void ng_edit_next_line_space(void) { // {改行}{End}{改行}{Space}
#if defined(NG_BMP)
    ng_ime_complete();
    ng_end();
    bmp_send_string("\n ");
#else
    ng_ime_complete();
    ng_end();
    tap_code(KC_ENTER);
    tap_code(KC_SPACE);
#endif
}

static void ng_edit_surround_parenthesis(void) { // ^x(^v){改行}{Space}+{↑}^x
    ng_cut();
    ng_parenthesis(); // (){改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
static void ng_edit_surround_corner_bracket(void) { // ^x「^v」{改行}{Space}+{↑}^x
    ng_cut();
    ng_corner_bracket(); // 「」{改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
static void ng_edit_surround_white_corner_bracket(void) { // ^x『^v』{改行}{Space}+{↑}^x
    ng_cut();
    ng_white_corner_bracket(); // 『』{改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
static void ng_edit_surround_black_lenticular_bracket(void) { // ^x【^v】{改行}{Space}+{↑}^x
    ng_cut();
    ng_black_lenticular_bracket(); // 【】{改行}{↑}
    ng_paste();
    ng_forward_cursor();    // 1文字進む
    copy_spc_to_clipboard();
}
static void ng_edit_surround_ruby(void) { // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
    ng_cut();
    ng_vertical_line();  // "｜"
    ng_paste();
    copy_spc_to_clipboard();
#if defined(NG_BMP)
    bmp_send_string(SS_DELAY(8));
#elif defined(NG_USE_DIC)
    wait_ms(32);
#elif !defined(NG_USE_KAWASEMI)
    wait_ms(40);
#endif
    ng_double_angle_bracket(); // 《》{改行}{↑}
}

// かな定義の要素数
#define NGMAP_COUNT (sizeof ngmap / sizeof ngmap[0])

#define NKEYS 3 // 1同時押しの最大キー数、薙刀式なら3
                // (最大何キーまでバッファに貯めるか)

// かな定義
// 順不同なのだが、3キー同時 → 2キー同時 → センターシフト → 単打 の順を推奨
// 単打とセンターシフトの並び順に「100万字日本語かなn-gramデータ」(https://kouy.exblog.jp/9731073/) を用いる
// (頻度が多いのを下にしたほうが負荷が減る)
static const PROGMEM naginata_keymap ngmap[] = {
  // ********** 3キー同時 **********
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
  {.key = B_D|B_F|B_P   , .func = ng_ime_cancel}, // {Esc 3}
  {.key = B_D|B_F|B_SCLN, .func = ng_katakana}, // ^i
  {.key = B_D|B_F|B_SLSH, .func = ng_hiragana}, // ^u
  {.key = B_J|B_K|B_Q   , .func = ng_eof}, // ^{End}
  {.key = B_J|B_K|B_A   , .func = ng_ellipsis}, // ……{改行}
  {.key = B_J|B_K|B_Z   , .func = ng_bar}, // ――{改行}
  {.key = B_J|B_K|B_W   , .func = ng_white_corner_bracket}, // 『』{改行}{↑}
  {.key = B_J|B_K|B_S   , .func = ng_parenthesis}, // (){改行}{↑}
  {.key = B_J|B_K|B_X   , .func = ng_black_lenticular_bracket}, // 【】{改行}{↑}
  {.key = B_J|B_K|B_D   , .func = ng_question_mark}, // ？{改行}
  {.key = B_J|B_K|B_C   , .func = ng_exclamation_mark}, // ！{改行}
  {.key = B_J|B_K|B_R   , .func = ng_save}, // ^s
  {.key = B_J|B_K|B_F   , .func = ng_corner_bracket}, // 「」{改行}{↑}
  {.key = B_J|B_K|B_V   , .func = ng_edit_kakutei_down}, // {改行}{↓}
  {.key = B_J|B_K|B_T   , .func = ng_middle_dot}, // ・
  {.key = B_J|B_K|B_G   , .func = ng_double_angle_bracket}, // 《》{改行}{↑}
  {.key = B_J|B_K|B_B   , .func = ng_edit_kakutei_left}, // {改行}{←}
  {.key = B_C|B_V|B_Y   , .func = ng_edit_s_home}, // +{Home}
  {.key = B_C|B_V|B_H   , .func = ng_copy}, // ^c
  {.key = B_C|B_V|B_N   , .func = ng_edit_s_end}, // +{End}
  {.key = B_C|B_V|B_U   , .func = ng_cut}, // ^x
  {.key = B_C|B_V|B_J   , .func = ng_1_previous_line_r}, // {→}
  {.key = B_C|B_V|B_M   , .func = ng_1_next_line_r}, // {←}
  {.key = B_C|B_V|B_I   , .func = ng_paste}, // ^v
  {.key = B_C|B_V|B_K   , .func = ng_s1_previous_line_r}, // +{→}
  {.key = B_C|B_V|B_COMM, .func = ng_s1_next_line_r}, // +{←}
  {.key = B_C|B_V|B_O   , .func = ng_redo}, // ^y
  {.key = B_C|B_V|B_L   , .func = ng_s5_previous_line_r}, // +{→ 5}
  {.key = B_C|B_V|B_DOT , .func = ng_s5_next_line_r}, // +{← 5}
  {.key = B_C|B_V|B_P   , .func = ng_undo}, // ^z
  {.key = B_C|B_V|B_SCLN, .func = ng_s20_previous_line_r}, // +{→ 20}
  {.key = B_C|B_V|B_SLSH, .func = ng_s20_next_line_r}, // +{← 20}
  {.key = B_M|B_COMM|B_Q, .func = ng_edit_togaki_zengyo}, // {Home}{→}{End}{Del 4}{←}
  {.key = B_M|B_COMM|B_A, .func = ng_edit_serifu_zengyo}, // {Home}{→}{End}{Del 2}{←}
  {.key = B_M|B_COMM|B_Z, .func = ng_edit_separate_line}, // 　　　×　　　×　　　×{改行 2}
  {.key = B_M|B_COMM|B_W, .func = ng_edit_surround_white_corner_bracket}, // ^x『^v』{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_S, .func = ng_edit_surround_parenthesis}, // ^x(^v){改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_X, .func = ng_edit_surround_black_lenticular_bracket}, // ^x【^v】{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_E, .func = ng_edit_togaki}, // {Home}{改行}{Space 3}{←}
  {.key = B_M|B_COMM|B_D, .func = ng_edit_serifu}, // {Home}{改行}{Space 1}{←}
  {.key = B_M|B_COMM|B_C, .func = ng_solidus}, // ／{改行}
  {.key = B_M|B_COMM|B_R, .func = ng_edit_3_space}, // {Space 3}
  {.key = B_M|B_COMM|B_F, .func = ng_edit_surround_corner_bracket}, // ^x「^v」{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_V, .func = ng_edit_next_line_corner_bracket}, // {改行}{End}{改行}「」{改行}{↑}
  {.key = B_M|B_COMM|B_T, .func = ng_white_circle}, // ○{改行}
  {.key = B_M|B_COMM|B_G, .func = ng_edit_surround_ruby}, // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_B, .func = ng_edit_next_line_space}, // {改行}{End}{改行}{Space}

  // 拗音、外来音
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
  {.key = B_M|B_X|B_I   , .func = ng_send_pyo   },  // ぴょ
  {.key = B_M|B_X|B_P   , .func = ng_send_pyu   },  // ぴゅ
  {.key = B_M|B_X|B_H   , .func = ng_send_pya   },  // ぴゃ
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
  {.key = B_M|B_Q|B_O   , .func = ng_send_ve    },  // ヴぇ
  {.key = B_M|B_Q|B_P   , .func = ng_send_vyu   },  // ヴゅ
  {.key = B_M|B_Q|B_J   , .func = ng_send_va    },  // ヴぁ
  {.key = B_M|B_Q|B_K   , .func = ng_send_vi    },  // ヴぃ
  {.key = B_M|B_Q|B_N   , .func = ng_send_vo    },  // ヴぉ
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
  // 非標準の変換
//  {.key = B_X|B_C|B_M    , .func = ng_send_pyu   },  // ピュ

  // ********** 2キー同時 **********
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
  {.key = B_Q|B_H       , .func = ng_send_xya   },  // ゃ
  {.key = B_Q|B_J       , .func = ng_send_xa    },  // ぁ
  {.key = B_Q|B_K       , .func = ng_send_xi    },  // ぃ
  {.key = B_Q|B_L       , .func = ng_send_xu    },  // ぅ
  {.key = B_Q|B_N       , .func = ng_send_xo    },  // ぉ
  {.key = B_Q|B_DOT     , .func = ng_send_xwa   },  // ゎ
  // {.key = B_Q|B_S       , .func = ng_send_xke   }, // ヶ
  // {.key = B_Q|B_F       , .func = ng_send_xka   }, // ヵ

  // 拗音、外来音
  {.key = B_W|B_H       , .func = ng_send_kya   },  // きゃ
  {.key = B_E|B_H       , .func = ng_send_rya   },  // りゃ
  {.key = B_R|B_H       , .func = ng_send_sya   },  // しゃ
  {.key = B_D|B_H       , .func = ng_send_nya   },  // にゃ
  {.key = B_G|B_H       , .func = ng_send_tya   },  // ちゃ
  {.key = B_X|B_H       , .func = ng_send_hya   },  // ひゃ
  {.key = B_B|B_H       , .func = ng_send_mya   },  // みゃ
  {.key = B_W|B_P       , .func = ng_send_kyu   },  // きゅ
  {.key = B_E|B_P       , .func = ng_send_ryu   },  // りゅ
  {.key = B_R|B_P       , .func = ng_send_syu   },  // しゅ
  {.key = B_D|B_P       , .func = ng_send_nyu   },  // にゅ
  {.key = B_G|B_P       , .func = ng_send_tyu   },  // ちゅ
  {.key = B_X|B_P       , .func = ng_send_hyu   },  // ひゅ
  {.key = B_B|B_P       , .func = ng_send_myu   },  // みゅ
  {.key = B_W|B_I       , .func = ng_send_kyo   },  // きょ
  {.key = B_E|B_I       , .func = ng_send_ryo   },  // りょ
  {.key = B_R|B_I       , .func = ng_send_syo   },  // しょ
  {.key = B_D|B_I       , .func = ng_send_nyo   },  // にょ
  {.key = B_G|B_I       , .func = ng_send_tyo   },  // ちょ
  {.key = B_X|B_I       , .func = ng_send_hyo   },  // ひょ
  {.key = B_B|B_I       , .func = ng_send_myo   },  // みょ

  // 機能部分
  {.key = B_H|B_J       , .func = naginata_on},   //　かなオン
  {.key = B_F|B_G       , .func = naginata_off},  //　かなオフ
  {.key = B_V|B_M       , .func = ng_enter},

//   {.key = B_Q|B_W       , .func = tategaki_off   },  // 横書き
//   {.key = B_Q|B_A       , .func = tategaki_on    },  // 縦書き

  // センターシフト
  {.key = B_SHFT|B_T    , .func = ng_s1_next_line_r}, // +{←}
  {.key = B_SHFT|B_Y    , .func = ng_s1_previous_line_r}, // +{→}
  {.key = B_SHFT|B_Q    , .func = ng_send_vu    },  // ヴ
  {.key = B_SHFT|B_W    , .func = ng_send_nu    },  // ぬ
  {.key = B_SHFT|B_P    , .func = ng_send_yu    },  // ゆ
  {.key = B_SHFT|B_COMM , .func = ng_send_mu    },  // む
  {.key = B_SHFT|B_R    , .func = ng_send_ne    },  // ね
  {.key = B_SHFT|B_SCLN , .func = ng_send_hu    },  // ふ
  {.key = B_SHFT|B_Z    , .func = ng_send_ho    },  // ほ
  {.key = B_SHFT|B_H    , .func = ng_send_ya    },  // や
  {.key = B_SHFT|B_X    , .func = ng_send_hi    },  // ひ
  {.key = B_SHFT|B_S    , .func = ng_send_me    },  // め
  {.key = B_SHFT|B_B    , .func = ng_send_mi    },  // み
  {.key = B_SHFT|B_DOT  , .func = ng_send_wa    },  // わ
  {.key = B_SHFT|B_O    , .func = ng_send_e     },  // え
  {.key = B_SHFT|B_I    , .func = ng_send_yo    },  // よ
  {.key = B_SHFT|B_A    , .func = ng_send_se    },  // せ
  {.key = B_SHFT|B_G    , .func = ng_send_ti    },  // ち
  {.key = B_SHFT|B_SLSH , .func = ng_send_re    },  // れ
  {.key = B_SHFT|B_U    , .func = ng_send_sa    },  // さ
  {.key = B_SHFT|B_C    , .func = ng_send_wo    },  // を
  {.key = B_SHFT|B_N    , .func = ng_send_o     },  // お
  {.key = B_SHFT|B_L    , .func = ng_send_tu    },  // つ
  {.key = B_SHFT|B_K    , .func = ng_send_mo    },  // も
  {.key = B_SHFT|B_E    , .func = ng_send_ri    },  // り
  {.key = B_SHFT|B_F    , .func = ng_send_ma    },  // ま
  {.key = B_SHFT|B_M    , .func = ng_edit_kuten },  // 。
  {.key = B_SHFT|B_D    , .func = ng_send_ni    },  // に
  {.key = B_SHFT|B_V    , .func = ng_edit_touten},  // 、
  {.key = B_SHFT|B_J    , .func = ng_send_no    },  // の

  // ********** 単打 **********
  {.key = B_T           , .func = ng_1_next_line_r}, // {←}
  {.key = B_Y           , .func = ng_1_previous_line_r}, // {→}
  {.key = B_Q           , .func = ng_send_vu    },  // ヴ
  {.key = B_P           , .func = ng_send_he    },  // へ
  {.key = B_Z           , .func = ng_send_ho    },  // ほ
  {.key = B_A           , .func = ng_send_ro    },  // ろ
  {.key = B_X           , .func = ng_send_hi    },  // ひ
  {.key = B_B           , .func = ng_send_so    },  // そ
  {.key = B_SCLN        , .func = ng_send_minus },  // ー
  {.key = B_S           , .func = ng_send_ke    },  // け
  {.key = B_SLSH        , .func = ng_send_re    },  // れ
  {.key = B_J           , .func = ng_send_a     },  // あ
  {.key = B_DOT         , .func = ng_send_ra    },  // ら
  {.key = B_O           , .func = ng_send_su    },  // す
  {.key = B_G           , .func = ng_send_xtu   },  // っ
  {.key = B_I           , .func = ng_send_ru    },  // る
  {.key = B_V           , .func = ng_send_ko    },  // こ
  {.key = B_C           , .func = ng_send_ha    },  // は
  {.key = B_W           , .func = ng_send_ki    },  // き
  {.key = B_M           , .func = ng_send_na    },  // な
  {.key = B_H           , .func = ng_send_ku    },  // く
  {.key = B_E           , .func = ng_send_te    },  // て
  {.key = B_N           , .func = ng_send_ta    },  // た
  {.key = B_D           , .func = ng_send_to    },  // と
  {.key = B_F           , .func = ng_send_ka    },  // か
  {.key = B_R           , .func = ng_send_si    },  // し
  {.key = B_COMM        , .func = ng_send_nn    },  // ん
  {.key = B_L           , .func = ng_send_u     },  // う
  {.key = B_K           , .func = ng_send_i     },  // い
  {.key = B_U           , .func = ng_backspace_with_repeat  },  // {BS}
  {.key = B_SHFT        , .func = ng_space_or_enter},
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
#if defined(NG_BMP)
    case NG_WIN ... NG_IOS:
#else
    case NG_WIN ...  NG_LINUX:
#endif
      break;
    default:
      naginata_config.os = NG_WIN;
      naginata_config.tategaki = 1;
      naginata_config.kouchi_shift = 0;
      break;
  }
  switchOS(naginata_config.os);
}

static int8_t n_modifier = 0;

// 薙刀式をオン
void naginata_on(void) {
  is_naginata = true;
  n_modifier = 0;
  layer_on(naginata_layer);

#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_LINUX:
      bmp_send_string(SS_TAP(X_INTERNATIONAL_2)SS_TAP(X_INTERNATIONAL_2)); // ひらがなx2
      break;
    default:
      bmp_send_string(SS_TAP(X_LANGUAGE_1));  // (Mac)かな
      break;
  }
#elif defined(NG_USE_DIC)
  switch (naginata_config.os) {
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_INTERNATIONAL_2);
      break;
    default:
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      break;
  }
#else
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
#endif
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  naginata_clear();
  layer_off(naginata_layer);

  uint8_t mods = get_mods();
  clear_mods();
#if defined(NG_BMP)
  bmp_send_string(SS_TAP(X_LANGUAGE_2));  // (Mac)英数
#elif defined(NG_USE_DIC)
  wait_ms(8); // キーを出力してから確定やIME操作までに、間を空ける
  tap_code(KC_LANGUAGE_2);  // (Mac)英数
#else
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
#endif
  set_mods(mods);
}

// 薙刀式のon/off状態を返す
bool naginata_state(void) {
  return is_naginata;
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

void switchOS(uint8_t os) {
  naginata_config.os = os;
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

void ng_show_os(void) {
#if defined(NG_BMP)
  switch (naginata_config.os) {
    case NG_WIN:
      bmp_send_string("win");
      break;
    case NG_MAC:
      bmp_send_string("mac");
      break;
    case NG_LINUX:
      bmp_send_string("linux");
      break;
    case NG_IOS:
      bmp_send_string("ios");
      break;
  }
  if (naginata_config.tategaki) {
    bmp_send_string("/tate");
  } else {
    bmp_send_string("/yoko");
  }
  if (naginata_config.kouchi_shift) {
    bmp_send_string("/"SS_TAP(X_KP_PLUS)"kouchi");
  }
#else
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
  if (naginata_config.tategaki) {
    SEND_STRING("/tate");
  } else {
    SEND_STRING("/yoko");
  }
  if (naginata_config.kouchi_shift) {
    SEND_STRING("/"SS_TAP(X_KP_PLUS)"kouchi");
  }
#endif
}

void tategaki_toggle(void) {
  naginata_config.tategaki ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

void tategaki_on(void) {
  naginata_config.tategaki = 1;
  // eeconfig_update_user(naginata_config.raw);
}

void tategaki_off(void) {
  naginata_config.tategaki = 0;
  // eeconfig_update_user(naginata_config.raw);
}

void kouchi_shift_toggle(void) {
  naginata_config.kouchi_shift ^= 1;
  eeconfig_update_user(naginata_config.raw);
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
#if defined(NG_BMP)
      case NGSW_IOS:
        switchOS(NG_IOS);
        return false;
#else
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
#endif
      case NG_SHOS:
        ng_show_os();
        return false;
      case NG_KOTI:
        kouchi_shift_toggle();
        return false;
      case NG_TAYO:
        tategaki_toggle();
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

// ngmap[num].key を返す
static Ngkey ngmap_key_sub(Ngmap_num num) {
#if defined(__AVR__)
  Ngkey key;
  memcpy_P(&key, &ngmap[num].key, sizeof(key));
  return key;
#else
  return ngmap[num].key;
#endif
}

// かな定義を探し、配列の添え字を返す
// 見つからないと NGMAP_COUNT を返す
static Ngmap_num ng_search(Ngkey searching_key) {
  // if (!searching_key)  return false;
  for (Ngmap_num num = NGMAP_COUNT; num-- > 0; ) {  // 逆順で検索
    if (searching_key == ngmap_key_sub(num)) {
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
#if defined(__AVR__)
    void (*func)(void);
    memcpy_P(&func, &ngmap[num].func, sizeof(func));
    func();
#else
    ngmap[num].func();
#endif
    return true;
  }
  return false;
}

// 変換してよいか調べる
// None: なし, One: 一つしかない, Multipul: しぼれない,
// WaitShift: 時限後置シフトを待つ, WaitDouji: センターシフトを押しながらの同時押しの制限時間を待つ
enum TransState { None, One, Multipul, WaitShift, WaitDouji };

static enum TransState which_trans_state(Ngkey search) {
  enum TransState state = None;
#if defined (NG_SHIFTED_DOUJI_MS)
  bool shifted = (search & B_SHFT) && search != B_SHFT;
#endif

  for (Ngmap_num num = 0; num < NGMAP_COUNT; num++) {
    Ngkey key;
    key = ngmap_key_sub(num);
    // search を含む
    if ((key & search) == search) {
#if defined (NG_SHIFTED_DOUJI_MS)
      // センターシフトを押しながらの同時押しの判定
      if (shifted) {
        if (key == search) {
          state = One;
        } else {
          return WaitDouji;
        }
      } else
#endif
      {
        Ngkey remains = key ^ search;
        switch (remains) {
          case 0:
            if (state == None) {
              state = One;
            }
            break;
#if defined (NG_KOUCHI_SHIFT_MS)  // 後置シフトの時間制限あり
          case B_SHFT:
            if (naginata_config.kouchi_shift) {
              state = WaitShift;
            }
            break;
          default:
            if (!(remains & B_SHFT)) {
              return Multipul;
            } else if (naginata_config.kouchi_shift) {
              state = WaitShift;
            }
            break;
#else
          default:
            if (naginata_config.kouchi_shift || !(remains & B_SHFT)) {
              return Multipul;
            }
            break;
#endif
        }
      }
    }
  }
  return state;
}

static enum TransState trans_state = None;

// キー入力を文字に変換して出力する
// 薙刀式のキー入力だったなら false を返す
// そうでない時は未出力を全て出力、true を返してQMKにまかせる
static bool naginata_type(uint16_t keycode, keyrecord_t *record) {
  static Ngkey waiting_keys[NKEYS]; // 各ビットがキーに対応する
  static Ngkey repeating_key = 0;
  static bool center_shift = false; // センターシフトの連続用

  Ngkey recent_key = 0;  // 各ビットがキーに対応する
  const bool pressing = record->event.pressed;
  bool store_key_later = false;

  trans_state = None;  // キー待ちを解除

  switch (keycode) {
    case NG_Q ... NG_SLSH:
      recent_key = NG_BIT(keycode);
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
#if defined(NG_KOUCHI_SHIFT_MS)
    // センターシフト(前置シフト限定か制限時間外の後置シフトの場合)
    if (recent_key == B_SHFT && (!naginata_config.kouchi_shift
        || record->event.time - ng_last_pressed_ms > (NG_KOUCHI_SHIFT_MS)
        || center_shift_count > 1)) {
#else
    // センターシフト(前置シフト限定)
    if (recent_key == B_SHFT && (!naginata_config.kouchi_shift || center_shift_count > 1)) {
#endif
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
#if defined(NG_KOUCHI_SHIFT_MS) || defined (NG_SHIFTED_DOUJI_MS) || defined (SHIFT_ALONE_TIMEOUT_MS)
    ng_last_pressed_ms = record->event.time;
#endif
  }

  // 何かキーを押したか、リピート中のキーを離した時
  if (pressing || (repeating_key & recent_key)) {
    end_repeating_key();  // キーリピート解除
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
      // (後置シフトにならないセンターシフトの時は全て出力する)
      if (searching_count == waiting_count && !store_key_later) {
        // 薙刀式のキーを押した時(同時押し定義の最大数に達していたら変換するため飛ばす)
        if (pressing && recent_key && waiting_count < NKEYS) {
          // 変換してよいか調べる
          trans_state = which_trans_state(searching_key);
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
        // センターシフトの連続用
        center_shift = (bool)(searching_key & B_SHFT);
        // 1回出力したらキー再利用は終わり
        reuse_key_state = Off;
        // 関数naginata_clear()が実行されていたら早期終了
        if (!waiting_count) {
          return false;
        }
        // 見つかった分のキーを配列から取り除く
        waiting_count -= searching_count;
        for (uint_fast8_t i = 0; i < waiting_count; i++) {
          waiting_keys[i] = waiting_keys[i + searching_count];
        }
        searching_count = waiting_count;
        // キーを離した時や、探すキーが残っている時はキーリピートしない
        if (!pressing || searching_count || store_key_later) {
          end_repeating_key();  // キーリピート解除
        // リピートするキーを保存
        } else {
          repeating_key = searching_key;
        }
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
#if defined(NG_USE_SHIFT_WHEN_SPACE_UP)
    pressed_key &= ~recent_key; // キーを取り除く
    if (waiting_count || pressed_key & B_SHFT || !pressed_key) {
      reuse_key_state = Off;
    // スペースを押していないなら次回、キー再利用可能
    } else {
      reuse_key_state = Run;
    }
#else
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
#endif
  }
  return (recent_key == 0);
}

// 後置シフト待ち処理
void kouchi_shift_loop(void) {
#if defined (NG_KOUCHI_SHIFT_MS) && defined (NG_SHIFTED_DOUJI_MS)
  // 後置シフトを待ったが時間切れ
  // センターシフトを押しながらの同時押しの制限時間を待ったが時間切れ
  if ((trans_state == WaitShift && timer_elapsed(ng_last_pressed_ms) >= (NG_KOUCHI_SHIFT_MS))
      || (trans_state == WaitDouji && timer_elapsed(ng_last_pressed_ms) >= (NG_SHIFTED_DOUJI_MS))) {
#elif defined (NG_KOUCHI_SHIFT_MS)
  // 後置シフトを待ったが時間切れ
  if (trans_state == WaitShift && timer_elapsed(ng_last_pressed_ms) >= (NG_KOUCHI_SHIFT_MS)) {
#elif defined (NG_SHIFTED_DOUJI_MS)
  // センターシフトを押しながらの同時押しの制限時間を待ったが時間切れ
  if (trans_state == WaitDouji && timer_elapsed(ng_last_pressed_ms) >= (NG_SHIFTED_DOUJI_MS)) {
#endif
#if defined (NG_KOUCHI_SHIFT_MS) || defined (NG_SHIFTED_DOUJI_MS)
    keyrecord_t record;
    record.event.pressed = true;
    record.event.time = timer_read();
    naginata_type(KC_NO, &record); // 未出力キーを処理
  }
#endif
}
