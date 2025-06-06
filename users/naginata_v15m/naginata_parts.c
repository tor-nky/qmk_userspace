/* Copyright Satoru NAKAYA <@tor-nky>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "naginata.h"
#include "naginata_parts.h"
#if defined(NG_BMP)
#   include "bmp_host_driver.h"
#   include "keyboards/ble_micro_pro/keymaps/naginata_v15m/bmp_send_string.h"
#   define BMP_DELAY 40
#endif

void ng_null(void) {}

#if defined(__AVR__)
// アルファベット
void ng_send_a(void) {  // あ
    tap_code(KC_A);
}
void ng_send_b(void) {
    tap_code(KC_B);
}
void ng_send_c(void) {
    tap_code(KC_C);
}
void ng_send_d(void) {
    tap_code(KC_D);
}
void ng_send_e(void) {  // え
    tap_code(KC_E);
}
void ng_send_f(void) {
    tap_code(KC_F);
}
void ng_send_g(void) {
    tap_code(KC_G);
}
void ng_send_h(void) {
    tap_code(KC_H);
}
void ng_send_i(void) {  // い
    tap_code(KC_I);
}
void ng_send_j(void) {
    tap_code(KC_J);
}
void ng_send_k(void) {
    tap_code(KC_K);
}
void ng_send_l(void) {
    tap_code(KC_L);
}
void ng_send_m(void) {
    tap_code(KC_M);
}
void ng_send_n(void) {
    tap_code(KC_N);
}
void ng_send_o(void) {  // お
    tap_code(KC_O);
}
void ng_send_p(void) {
    tap_code(KC_P);
}
void ng_send_q(void) {
    tap_code(KC_Q);
}
void ng_send_r(void) {
    tap_code(KC_R);
}
void ng_send_s(void) {
    tap_code(KC_S);
}
void ng_send_t(void) {
    tap_code(KC_T);
}
void ng_send_u(void) {  // う
    tap_code(KC_U);
}
void ng_send_v(void) {
    tap_code(KC_V);
}
void ng_send_w(void) {
    tap_code(KC_W);
}
void ng_send_x(void) {
    tap_code(KC_X);
}
void ng_send_y(void) {
    tap_code(KC_Y);
}
void ng_send_z(void) {
    tap_code(KC_Z);
}

// 清音
void ng_send_ka(void) { // か
    ng_send_k();
    ng_send_a();
}
void ng_send_ki(void) { // き
    ng_send_k();
    ng_send_i();
}
void ng_send_ku(void) { // く
    ng_send_k();
    ng_send_u();
}
void ng_send_ke(void) { // け
    ng_send_k();
    ng_send_e();
}
void ng_send_ko(void) { // こ
    ng_send_k();
    ng_send_o();
}
void ng_send_sa(void) { // さ
    ng_send_s();
    ng_send_a();
}
void ng_send_si(void) { // し
    ng_send_s();
    ng_send_i();
}
void ng_send_su(void) { // す
    ng_send_s();
    ng_send_u();
}
void ng_send_se(void) { // せ
    ng_send_s();
    ng_send_e();
}
void ng_send_so(void) { // そ
    ng_send_s();
    ng_send_o();
}
void ng_send_ta(void) { // た
    ng_send_t();
    ng_send_a();
}
void ng_send_ti(void) { // ち
    ng_send_t();
    ng_send_i();
}
void ng_send_tu(void) { // つ
    ng_send_t();
    ng_send_u();
}
void ng_send_te(void) { // て
    ng_send_t();
    ng_send_e();
}
void ng_send_to(void) { // と
    ng_send_t();
    ng_send_o();
}
void ng_send_na(void) { // な
    ng_send_n();
    ng_send_a();
}
void ng_send_ni(void) { // に
    ng_send_n();
    ng_send_i();
}
void ng_send_nu(void) { // ぬ
    ng_send_n();
    ng_send_u();
}
void ng_send_ne(void) { // ね
    ng_send_n();
    ng_send_e();
}
void ng_send_no(void) { // の
    ng_send_n();
    ng_send_o();
}
void ng_send_ha(void) { // は
    ng_send_h();
    ng_send_a();
}
void ng_send_hi(void) { // ひ
    ng_send_h();
    ng_send_i();
}
void ng_send_hu(void) { // ふ
    ng_send_h();
    ng_send_u();
}
void ng_send_he(void) { // へ
    ng_send_h();
    ng_send_e();
}
void ng_send_ho(void) { // ほ
    ng_send_h();
    ng_send_o();
}
void ng_send_ma(void) { // ま
    ng_send_m();
    ng_send_a();
}
void ng_send_mi(void) { // み
    ng_send_m();
    ng_send_i();
}
void ng_send_mu(void) { // む
    ng_send_m();
    ng_send_u();
}
void ng_send_me(void) { // め
    ng_send_m();
    ng_send_e();
}
void ng_send_mo(void) { // も
    ng_send_m();
    ng_send_o();
}
void ng_send_ya(void) { // や
    ng_send_y();
    ng_send_a();
}
void ng_send_yu(void) { // ゆ
    ng_send_y();
    ng_send_u();
}
void ng_send_yo(void) { // よ
    ng_send_y();
    ng_send_o();
}
void ng_send_ra(void) { // ら
    ng_send_r();
    ng_send_a();
}
void ng_send_ri(void) { // り
    ng_send_r();
    ng_send_i();
}
void ng_send_ru(void) { // る
    ng_send_r();
    ng_send_u();
}
void ng_send_re(void) { // れ
    ng_send_r();
    ng_send_e();
}
void ng_send_ro(void) { // ろ
    ng_send_r();
    ng_send_o();
}
void ng_send_wa(void) { // わ
    ng_send_w();
    ng_send_a();
}
void ng_send_wo(void) { // を
    ng_send_w();
    ng_send_o();
}
void ng_send_nn(void) { // ん
    ng_send_n();
    ng_send_n();
}
void ng_send_minus(void) {  // ー
    tap_code(KC_MINUS);
}

// 濁音
void ng_send_ga(void) { // が
    ng_send_g();
    ng_send_a();
}
void ng_send_gi(void) { // ぎ
    ng_send_g();
    ng_send_i();
}
void ng_send_gu(void) { // ぐ
    ng_send_g();
    ng_send_u();
}
void ng_send_ge(void) { // げ
    ng_send_g();
    ng_send_e();
}
void ng_send_go(void) { // ご
    ng_send_g();
    ng_send_o();
}
void ng_send_za(void) { // ざ
    ng_send_z();
    ng_send_a();
}
void ng_send_zi(void) { // じ
    ng_send_z();
    ng_send_i();
}
void ng_send_zu(void) { // ず
    ng_send_z();
    ng_send_u();
}
void ng_send_ze(void) { // ぜ
    ng_send_z();
    ng_send_e();
}
void ng_send_zo(void) { // ぞ
    ng_send_z();
    ng_send_o();
}
void ng_send_da(void) { // だ
    ng_send_d();
    ng_send_a();
}
void ng_send_di(void) { // ぢ
    ng_send_d();
    ng_send_i();
}
void ng_send_du(void) { // づ
    ng_send_d();
    ng_send_u();
}
void ng_send_de(void) { // で
    ng_send_d();
    ng_send_e();
}
void ng_send_do(void) { // ど
    ng_send_d();
    ng_send_o();
}
void ng_send_ba(void) { // ば
    ng_send_b();
    ng_send_a();
}
void ng_send_bi(void) { // び
    ng_send_b();
    ng_send_i();
}
void ng_send_bu(void) { // ぶ
    ng_send_b();
    ng_send_u();
}
void ng_send_be(void) { // べ
    ng_send_b();
    ng_send_e();
}
void ng_send_bo(void) { // ぼ
    ng_send_b();
    ng_send_o();
}
void ng_send_vu(void) { // ヴ
    ng_send_v();
    ng_send_u();
}

// 半濁音
void ng_send_pa(void) { // ぱ
    ng_send_p();
    ng_send_a();
}
void ng_send_pi(void) { // ぴ
    ng_send_p();
    ng_send_i();
}
void ng_send_pu(void) { // ぷ
    ng_send_p();
    ng_send_u();
}
void ng_send_pe(void) { // ぺ
    ng_send_p();
    ng_send_e();
}
void ng_send_po(void) { // ぽ
    ng_send_p();
    ng_send_o();
}

// 小書き
void ng_send_xya(void) {    // ゃ
    ng_send_x();
    ng_send_ya();
}
void ng_send_xyu(void) {    // ゅ
    ng_send_x();
    ng_send_yu();
}
void ng_send_xyo(void) {    // ょ
    ng_send_x();
    ng_send_yo();
}
void ng_send_xa(void) {     // ぁ
    ng_send_x();
    ng_send_a();
}
void ng_send_xi(void) {     // ぃ
    ng_send_x();
    ng_send_i();
}
void ng_send_xu(void) {     // ぅ
    ng_send_x();
    ng_send_u();
}
void ng_send_xe(void) {     // ぇ
    ng_send_x();
    ng_send_e();
}
void ng_send_xo(void) {     // ぉ
    ng_send_x();
    ng_send_o();
}
void ng_send_xwa(void) {    // ゎ
    ng_send_x();
    ng_send_wa();
}
void ng_send_xtu(void) {    // っ
    ng_send_x();
    ng_send_tu();
}
void ng_send_xke(void) {    // ヶ
    ng_send_x();
    ng_send_ke();
}
void ng_send_xka(void) {    // ヵ
    ng_send_x();
    ng_send_ka();
}

// 清音拗音 濁音拗音 半濁拗音
void ng_send_sya(void) {    // しゃ
    ng_send_s();
    ng_send_ya();
}
void ng_send_syu(void) {    // しゅ
    ng_send_s();
    ng_send_yu();
}
void ng_send_syo(void) {    // しょ
    ng_send_s();
    ng_send_yo();
}
void ng_send_ja(void) {     // じゃ
    ng_send_j();
    ng_send_a();
}
void ng_send_ju(void) {     // じゅ
    ng_send_j();
    ng_send_u();
}
void ng_send_jo(void) {     // じょ
    ng_send_j();
    ng_send_o();
}
void ng_send_kya(void) {    // きゃ
    ng_send_k();
    ng_send_ya();
}
void ng_send_kyu(void) {    // きゅ
    ng_send_k();
    ng_send_yu();
}
void ng_send_kyo(void) {    // きょ
    ng_send_k();
    ng_send_yo();
}
void ng_send_gya(void) {    // ぎゃ
    ng_send_g();
    ng_send_ya();
}
void ng_send_gyu(void) {    // ぎゅ
    ng_send_g();
    ng_send_yu();
}
void ng_send_gyo(void) {    // ぎょ
    ng_send_g();
    ng_send_yo();
}
void ng_send_tya(void) {    // ちゃ
    ng_send_t();
    ng_send_ya();
}
void ng_send_tyu(void) {    // ちゅ
    ng_send_t();
    ng_send_yu();
}
void ng_send_tyo(void) {    // ちょ
    ng_send_t();
    ng_send_yo();
}
void ng_send_dya(void) {    // ぢゃ
    ng_send_d();
    ng_send_ya();
}
void ng_send_dyu(void) {    // ぢゅ
    ng_send_d();
    ng_send_yu();
}
void ng_send_dyo(void) {    // ぢょ
    ng_send_d();
    ng_send_yo();
}
void ng_send_nya(void) {    // にゃ
    ng_send_n();
    ng_send_ya();
}
void ng_send_nyu(void) {    // にゅ
    ng_send_n();
    ng_send_yu();
}
void ng_send_nyo(void) {    // にょ
    ng_send_n();
    ng_send_yo();
}
void ng_send_hya(void) {    // ひゃ
    ng_send_h();
    ng_send_ya();
}
void ng_send_hyu(void) {    // ひゅ
    ng_send_h();
    ng_send_yu();
}
void ng_send_hyo(void) {    // ひょ
    ng_send_h();
    ng_send_yo();
}
void ng_send_bya(void) {    // びゃ
    ng_send_b();
    ng_send_ya();
}
void ng_send_byu(void) {    // びゅ
    ng_send_b();
    ng_send_yu();
}
void ng_send_byo(void) {    // びょ
    ng_send_b();
    ng_send_yo();
}
void ng_send_pya(void) {    // ぴゃ
    ng_send_p();
    ng_send_ya();
}
void ng_send_pyu(void) {    // ぴゅ
    ng_send_p();
    ng_send_yu();
}
void ng_send_pyo(void) {    // ぴょ
    ng_send_p();
    ng_send_yo();
}
void ng_send_mya(void) {    // みゃ
    ng_send_m();
    ng_send_ya();
}
void ng_send_myu(void) {    // みゅ
    ng_send_m();
    ng_send_yu();
}
void ng_send_myo(void) {    // みょ
    ng_send_m();
    ng_send_yo();
}
void ng_send_rya(void) {    // りゃ
    ng_send_r();
    ng_send_ya();
}
void ng_send_ryu(void) {    // りゅ
    ng_send_r();
    ng_send_yu();
}
void ng_send_ryo(void) {    // りょ
    ng_send_r();
    ng_send_yo();
}

// 清音外来音 濁音外来音
void ng_send_thi(void) {    // てぃ
    ng_send_t();
    ng_send_hi();
}
void ng_send_thu(void) {    // てゅ
    ng_send_t();
    ng_send_hu();
}
void ng_send_dhi(void) {    // でぃ
    ng_send_d();
    ng_send_hi();
}
void ng_send_dhu(void) {    // でゅ
    ng_send_d();
    ng_send_hu();
}
void ng_send_twu(void) {    // とぅ
    ng_send_t();
    ng_send_w();
    ng_send_u();
}
void ng_send_dwu(void) {    // どぅ
    ng_send_d();
    ng_send_w();
    ng_send_u();
}
void ng_send_sye(void) {    // しぇ
    ng_send_s();
    ng_send_ye();
}
void ng_send_tye(void) {    // ちぇ
    ng_send_t();
    ng_send_ye();
}
void ng_send_je(void) {     // じぇ
    ng_send_j();
    ng_send_e();
}
void ng_send_dye(void) {    // ぢぇ
    ng_send_d();
    ng_send_ye();
}
void ng_send_fa(void) {     // ふぁ
    ng_send_f();
    ng_send_a();
}
void ng_send_fi(void) {     // ふぃ
    ng_send_f();
    ng_send_i();
}
void ng_send_fe(void) {     // ふぇ
    ng_send_f();
    ng_send_e();
}
void ng_send_fo(void) {     // ふぉ
    ng_send_f();
    ng_send_o();
}
void ng_send_fyu(void) {    // ふゅ
    ng_send_f();
    ng_send_yu();
}
void ng_send_ye(void) {     // いぇ
    ng_send_y();
    ng_send_e();
}
void ng_send_wi(void) {     // うぃ
    ng_send_w();
    ng_send_i();
}
void ng_send_we(void) {     // うぇ
    ng_send_w();
    ng_send_e();
}
void ng_send_who(void) {    // うぉ
    ng_send_w();
    ng_send_ho();
}
void ng_send_va(void) {     // ヴぁ
    ng_send_v();
    ng_send_a();
}
void ng_send_vi(void) {     // ヴぃ
    ng_send_v();
    ng_send_i();
}
void ng_send_ve(void) {     // ヴぇ
    ng_send_v();
    ng_send_e();
}
void ng_send_vo(void) {     // ヴぉ
    ng_send_v();
    ng_send_o();
}
void ng_send_vyu(void) {    // ヴゅ
    ng_send_v();
    ng_send_yu();
}
void ng_send_qa(void) {     // くぁ
    ng_send_q();
    ng_send_a();
}
void ng_send_qi(void) {     // くぃ
    ng_send_q();
    ng_send_i();
}
void ng_send_qe(void) {     // くぇ
    ng_send_q();
    ng_send_e();
}
void ng_send_qo(void) {     // くぉ
    ng_send_q();
    ng_send_o();
}
void ng_send_kuxwa(void) {  // くゎ
    ng_send_ku();
    ng_send_xwa();
}
void ng_send_gwa(void) {    // ぐぁ
    ng_send_g();
    ng_send_wa();
}
void ng_send_gwi(void) {    // ぐぃ
    ng_send_g();
    ng_send_wi();
}
void ng_send_gwe(void) {    // ぐぇ
    ng_send_g();
    ng_send_we();
}
void ng_send_gwo(void) {    // ぐぉ
    ng_send_g();
    ng_send_wo();
}
void ng_send_guxwa(void) {  // ぐゎ
    ng_send_gu();
    ng_send_xwa();
}
void ng_send_tsa(void) {    // つぁ
    ng_send_t();
    ng_send_sa();
}
#else
#   if defined(NG_BMP)
#       define NG_SEND_KANA(string) bmp_send_string(string)
#   else
// 文字列を少し速く出力
static void ng_send_kana(const char *str) {
    // Macでは押していないカーソルキーがなぜか入力されることがあるので、普通の方法で出力
    if (naginata_config.os == NG_MAC) {
        send_string_P(str);
        return;
    }

    // 取り出し
    char ascii_code = pgm_read_byte(str++);
    while (ascii_code != '\0') {
        // 出力バッファを空にする
        clear_keys();
        // 次のを取り出し
        char next = pgm_read_byte(str++);
        // 出力
        {
            // アスキーコードからキーコードに変換
            uint8_t keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
            // 同じキーの連続
            if (ascii_code == next) {
                tap_code(keycode);
            } else {
                register_code(keycode);
            }
        }
        // 更新
        ascii_code = next;
    }
    // 最後にすべてのキーを離す
    clear_keyboard_but_mods();  // 押されている修飾キー以外の全てのキーをクリア
}
#       define NG_SEND_KANA(string) ng_send_kana(PSTR(string))
#   endif
// テスト用
void ng_send_iroha(void) {  // いろは歌
    NG_SEND_KANA("irohanihohetotirinuruwowakayotaresotunenaramuuwyinookuyamakehukoeteasakiyumemisiwyehimosesu");
}

// アルファベット
void ng_send_a(void) {  // あ
    NG_SEND_KANA("a");
}
void ng_send_b(void) {
    NG_SEND_KANA("b");
}
void ng_send_c(void) {
    NG_SEND_KANA("c");
}
void ng_send_d(void) {
    NG_SEND_KANA("d");
}
void ng_send_e(void) {  // え
    NG_SEND_KANA("e");
}
void ng_send_f(void) {
    NG_SEND_KANA("f");
}
void ng_send_g(void) {
    NG_SEND_KANA("g");
}
void ng_send_h(void) {
    NG_SEND_KANA("h");
}
void ng_send_i(void) {  // い
    NG_SEND_KANA("i");
}
void ng_send_j(void) {
    NG_SEND_KANA("j");
}
void ng_send_k(void) {
    NG_SEND_KANA("j");
}
void ng_send_l(void) {
    NG_SEND_KANA("l");
}
void ng_send_m(void) {
    NG_SEND_KANA("m");
}
void ng_send_n(void) {
    NG_SEND_KANA("n");
}
void ng_send_o(void) {  // お
    NG_SEND_KANA("o");
}
void ng_send_p(void) {
    NG_SEND_KANA("p");
}
void ng_send_q(void) {
    NG_SEND_KANA("q");
}
void ng_send_r(void) {
    NG_SEND_KANA("r");
}
void ng_send_s(void) {
    NG_SEND_KANA("s");
}
void ng_send_t(void) {
    NG_SEND_KANA("t");
}
void ng_send_u(void) {  // う
    NG_SEND_KANA("u");
}
void ng_send_v(void) {
    NG_SEND_KANA("v");
}
void ng_send_w(void) {
    NG_SEND_KANA("w");
}
void ng_send_x(void) {
    NG_SEND_KANA("x");
}
void ng_send_y(void) {
    NG_SEND_KANA("y");
}
void ng_send_z(void) {
    NG_SEND_KANA("z");
}

// 清音
void ng_send_ka(void) { // か
    NG_SEND_KANA("ka");
}
void ng_send_ki(void) { // き
    NG_SEND_KANA("ki");
}
void ng_send_ku(void) { // く
    NG_SEND_KANA("ku");
}
void ng_send_ke(void) { // け
    NG_SEND_KANA("ke");
}
void ng_send_ko(void) { // こ
    NG_SEND_KANA("ko");
}
void ng_send_sa(void) { // さ
    NG_SEND_KANA("sa");
}
void ng_send_si(void) { // し
    NG_SEND_KANA("si");
}
void ng_send_su(void) { // す
    NG_SEND_KANA("su");
}
void ng_send_se(void) { // せ
    NG_SEND_KANA("se");
}
void ng_send_so(void) { // そ
    NG_SEND_KANA("so");
}
void ng_send_ta(void) { // た
    NG_SEND_KANA("ta");
}
void ng_send_ti(void) { // ち
    NG_SEND_KANA("ti");
}
void ng_send_tu(void) { // つ
    NG_SEND_KANA("tu");
}
void ng_send_te(void) { // て
    NG_SEND_KANA("te");
}
void ng_send_to(void) { // と
    NG_SEND_KANA("to");
}
void ng_send_na(void) { // な
    NG_SEND_KANA("na");
}
void ng_send_ni(void) { // に
    NG_SEND_KANA("ni");
}
void ng_send_nu(void) { // ぬ
    NG_SEND_KANA("nu");
}
void ng_send_ne(void) { // ね
    NG_SEND_KANA("ne");
}
void ng_send_no(void) { // の
    NG_SEND_KANA("no");
}
void ng_send_ha(void) { // は
    NG_SEND_KANA("ha");
}
void ng_send_hi(void) { // ひ
    NG_SEND_KANA("hi");
}
void ng_send_hu(void) { // ふ
    NG_SEND_KANA("hu");
}
void ng_send_he(void) { // へ
    NG_SEND_KANA("he");
}
void ng_send_ho(void) { // ほ
    NG_SEND_KANA("ho");
}
void ng_send_ma(void) { // ま
    NG_SEND_KANA("ma");
}
void ng_send_mi(void) { // み
    NG_SEND_KANA("mi");
}
void ng_send_mu(void) { // む
    NG_SEND_KANA("mu");
}
void ng_send_me(void) { // め
    NG_SEND_KANA("me");
}
void ng_send_mo(void) { // も
    NG_SEND_KANA("mo");
}
void ng_send_ya(void) { // や
    NG_SEND_KANA("ya");
}
void ng_send_yu(void) { // ゆ
    NG_SEND_KANA("yu");
}
void ng_send_yo(void) { // よ
    NG_SEND_KANA("yo");
}
void ng_send_ra(void) { // ら
    NG_SEND_KANA("ra");
}
void ng_send_ri(void) { // り
    NG_SEND_KANA("ri");
}
void ng_send_ru(void) { // る
    NG_SEND_KANA("ru");
}
void ng_send_re(void) { // れ
    NG_SEND_KANA("re");
}
void ng_send_ro(void) { // ろ
    NG_SEND_KANA("ro");
}
void ng_send_wa(void) { // わ
    NG_SEND_KANA("wa");
}
void ng_send_wo(void) { // を
    NG_SEND_KANA("wo");
}
void ng_send_nn(void) { // ん
    NG_SEND_KANA("nn");
}
void ng_send_minus(void) {  // ー
    NG_SEND_KANA("-");
}

// 濁音
void ng_send_ga(void) { // が
    NG_SEND_KANA("ga");
}
void ng_send_gi(void) { // ぎ
    NG_SEND_KANA("gi");
}
void ng_send_gu(void) { // ぐ
    NG_SEND_KANA("gu");
}
void ng_send_ge(void) { // げ
    NG_SEND_KANA("ge");
}
void ng_send_go(void) { // ご
    NG_SEND_KANA("go");
}
void ng_send_za(void) { // ざ
    NG_SEND_KANA("za");
}
void ng_send_zi(void) { // じ
    NG_SEND_KANA("zi");
}
void ng_send_zu(void) { // ず
    NG_SEND_KANA("zu");
}
void ng_send_ze(void) { // ぜ
    NG_SEND_KANA("ze");
}
void ng_send_zo(void) { // ぞ
    NG_SEND_KANA("zo");
}
void ng_send_da(void) { // だ
    NG_SEND_KANA("da");
}
void ng_send_di(void) { // ぢ
    NG_SEND_KANA("di");
}
void ng_send_du(void) { // づ
    NG_SEND_KANA("du");
}
void ng_send_de(void) { // で
    NG_SEND_KANA("de");
}
void ng_send_do(void) { // ど
    NG_SEND_KANA("do");
}
void ng_send_ba(void) { // ば
    NG_SEND_KANA("ba");
}
void ng_send_bi(void) { // び
    NG_SEND_KANA("bi");
}
void ng_send_bu(void) { // ぶ
    NG_SEND_KANA("bu");
}
void ng_send_be(void) { // べ
    NG_SEND_KANA("be");
}
void ng_send_bo(void) { // ぼ
    NG_SEND_KANA("bo");
}
void ng_send_vu(void) { // ヴ
    NG_SEND_KANA("vu");
}

// 半濁音
void ng_send_pa(void) { // ぱ
    NG_SEND_KANA("pa");
}
void ng_send_pi(void) { // ぴ
    NG_SEND_KANA("pi");
}
void ng_send_pu(void) { // ぷ
    NG_SEND_KANA("pu");
}
void ng_send_pe(void) { // ぺ
    NG_SEND_KANA("pe");
}
void ng_send_po(void) { // ぽ
    NG_SEND_KANA("po");
}

// 小書き
void ng_send_xya(void) {    // ゃ
    NG_SEND_KANA("xya");
}
void ng_send_xyu(void) {    // ゅ
    NG_SEND_KANA("xyu");
}
void ng_send_xyo(void) {    // ょ
    NG_SEND_KANA("xyo");
}
void ng_send_xa(void) {     // ぁ
    NG_SEND_KANA("xa");
}
void ng_send_xi(void) {     // ぃ
    NG_SEND_KANA("xi");
}
void ng_send_xu(void) {     // ぅ
    NG_SEND_KANA("xu");
}
void ng_send_xe(void) {     // ぇ
    NG_SEND_KANA("xe");
}
void ng_send_xo(void) {     // ぉ
    NG_SEND_KANA("xo");
}
void ng_send_xwa(void) {    // ゎ
    NG_SEND_KANA("xwa");
}
void ng_send_xtu(void) {    // っ
    NG_SEND_KANA("xtu");
}
void ng_send_xke(void) {    // ヶ
    NG_SEND_KANA("xke");
}
void ng_send_xka(void) {    // ヵ
    NG_SEND_KANA("xka");
}

// 清音拗音 濁音拗音 半濁拗音
void ng_send_sya(void) {    // しゃ
    NG_SEND_KANA("sya");
}
void ng_send_syu(void) {    // しゅ
    NG_SEND_KANA("syu");
}
void ng_send_syo(void) {    // しょ
    NG_SEND_KANA("syo");
}
void ng_send_ja(void) {     // じゃ
    NG_SEND_KANA("ja");
}
void ng_send_ju(void) {     // じゅ
    NG_SEND_KANA("ju");
}
void ng_send_jo(void) {     // じょ
    NG_SEND_KANA("jo");
}
void ng_send_kya(void) {    // きゃ
    NG_SEND_KANA("kya");
}
void ng_send_kyu(void) {    // きゅ
    NG_SEND_KANA("kyu");
}
void ng_send_kyo(void) {    // きょ
    NG_SEND_KANA("kyo");
}
void ng_send_gya(void) {    // ぎゃ
    NG_SEND_KANA("gya");
}
void ng_send_gyu(void) {    // ぎゅ
    NG_SEND_KANA("gyu");
}
void ng_send_gyo(void) {    // ぎょ
    NG_SEND_KANA("gyo");
}
void ng_send_tya(void) {    // ちゃ
    NG_SEND_KANA("tya");
}
void ng_send_tyu(void) {    // ちゅ
    NG_SEND_KANA("tyu");
}
void ng_send_tyo(void) {    // ちょ
    NG_SEND_KANA("tyo");
}
void ng_send_dya(void) {    // ぢゃ
    NG_SEND_KANA("dya");
}
void ng_send_dyu(void) {    // ぢゅ
    NG_SEND_KANA("dyu");
}
void ng_send_dyo(void) {    // ぢょ
    NG_SEND_KANA("dyo");
}
void ng_send_nya(void) {    // にゃ
    NG_SEND_KANA("nya");
}
void ng_send_nyu(void) {    // にゅ
    NG_SEND_KANA("nyu");
}
void ng_send_nyo(void) {    // にょ
    NG_SEND_KANA("nyo");
}
void ng_send_hya(void) {    // ひゃ
    NG_SEND_KANA("hya");
}
void ng_send_hyu(void) {    // ひゅ
    NG_SEND_KANA("hyu");
}
void ng_send_hyo(void) {    // ひょ
    NG_SEND_KANA("hyo");
}
void ng_send_bya(void) {    // びゃ
    NG_SEND_KANA("bya");
}
void ng_send_byu(void) {    // びゅ
    NG_SEND_KANA("byu");
}
void ng_send_byo(void) {    // びょ
    NG_SEND_KANA("byo");
}
void ng_send_pya(void) {    // ぴゃ
    NG_SEND_KANA("pya");
}
void ng_send_pyu(void) {    // ぴゅ
    NG_SEND_KANA("pyu");
}
void ng_send_pyo(void) {    // ぴょ
    NG_SEND_KANA("pyo");
}
void ng_send_mya(void) {    // みゃ
    NG_SEND_KANA("mya");
}
void ng_send_myu(void) {    // みゅ
    NG_SEND_KANA("myu");
}
void ng_send_myo(void) {    // みょ
    NG_SEND_KANA("myo");
}
void ng_send_rya(void) {    // りゃ
    NG_SEND_KANA("rya");
}
void ng_send_ryu(void) {    // りゅ
    NG_SEND_KANA("ryu");
}
void ng_send_ryo(void) {    // りょ
    NG_SEND_KANA("ryo");
}

// 清音外来音 濁音外来音
void ng_send_thi(void) {    // てぃ
    NG_SEND_KANA("thi");
}
void ng_send_thu(void) {    // てゅ
    NG_SEND_KANA("thu");
}
void ng_send_dhi(void) {    // でぃ
    NG_SEND_KANA("dhi");
}
void ng_send_dhu(void) {    // でゅ
    NG_SEND_KANA("dhu");
}
void ng_send_twu(void) {    // とぅ
    NG_SEND_KANA("twu");
}
void ng_send_dwu(void) {    // どぅ
    NG_SEND_KANA("dwu");
}
void ng_send_sye(void) {    // しぇ
    NG_SEND_KANA("sye");
}
void ng_send_tye(void) {    // ちぇ
    NG_SEND_KANA("tye");
}
void ng_send_je(void) {     // じぇ
    NG_SEND_KANA("je");
}
void ng_send_dye(void) {    // ぢぇ
    NG_SEND_KANA("dye");
}
void ng_send_fa(void) {     // ふぁ
    NG_SEND_KANA("fa");
}
void ng_send_fi(void) {     // ふぃ
    NG_SEND_KANA("fi");
}
void ng_send_fe(void) {     // ふぇ
    NG_SEND_KANA("fe");
}
void ng_send_fo(void) {     // ふぉ
    NG_SEND_KANA("fo");
}
void ng_send_fyu(void) {    // ふゅ
    NG_SEND_KANA("fyu");
}
void ng_send_ye(void) {     // いぇ
    NG_SEND_KANA("ye");
}
void ng_send_wi(void) {     // うぃ
    NG_SEND_KANA("wi");
}
void ng_send_we(void) {     // うぇ
    NG_SEND_KANA("we");
}
void ng_send_who(void) {    // うぉ
    NG_SEND_KANA("who");
}
void ng_send_va(void) {     // ヴぁ
    NG_SEND_KANA("va");
}
void ng_send_vi(void) {     // ヴぃ
    NG_SEND_KANA("vi");
}
void ng_send_ve(void) {     // ヴぇ
    NG_SEND_KANA("ve");
}
void ng_send_vo(void) {     // ヴぉ
    NG_SEND_KANA("vo");
}
void ng_send_vyu(void) {    // ヴゅ
    NG_SEND_KANA("vyu");
}
void ng_send_qa(void) {     // くぁ
    NG_SEND_KANA("qa");
}
void ng_send_qi(void) {     // くぃ
    NG_SEND_KANA("qi");
}
void ng_send_qe(void) {     // くぇ
    NG_SEND_KANA("qe");
}
void ng_send_qo(void) {     // くぉ
    NG_SEND_KANA("qo");
}
void ng_send_kuxwa(void) {  // くゎ
    NG_SEND_KANA("kuxwa");
}
void ng_send_gwa(void) {    // ぐぁ
    NG_SEND_KANA("gwa");
}
void ng_send_gwi(void) {    // ぐぃ
    NG_SEND_KANA("gwi");
}
void ng_send_gwe(void) {    // ぐぇ
    NG_SEND_KANA("gwe");
}
void ng_send_gwo(void) {    // ぐぉ
    NG_SEND_KANA("gwo");
}
void ng_send_guxwa(void) {  // ぐゎ
    NG_SEND_KANA("guxwa");
}
void ng_send_tsa(void) {    // つぁ
    NG_SEND_KANA("tsa");
}
#endif

void copy_spc_to_clipboard(void) {
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
    send_string_P(str);
    tap_code(KC_SPACE);
    tap_code(KC_ENTER);
#   endif
}
#endif

// 追加
void ng_edit_touten(void) { // 、
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
void ng_edit_kuten(void) { // 。
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
void ng_enter(void) { // {Enter}
#if defined(NG_BMP)
    bmp_send_string("\n");
#else
    tap_code(KC_ENTER);
#endif
}

// 編集モード
void ng_middle_dot(void) { // ・
#if defined(NG_BMP)
    bmp_send_string("/");
#else
    tap_code(KC_SLASH);
#endif
}
void ng_question_mark(void) { // ？{改行}
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
void ng_exclamation_mark(void) { // ！{改行}
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
void ng_solidus(void) { // ／{改行}
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
void ng_white_circle(void) { // 〇{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagimaru")); // "〇"
#else
    ng_send_unicode_string_P(PSTR("〇"));
#endif
}
void ng_vertical_line(void) { // ｜{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagitase")); // "｜"
#else
    ng_send_unicode_string_P(PSTR("｜"));
#endif
}
void ng_ellipsis(void) { // ……{改行}
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
void ng_bar(void) { // ――{改行}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagiyose")); // "──"
#else
    ng_send_unicode_string_P(PSTR("──"));
#endif
}
void ng_edit_separate_line(void) { // 　　　×　　　×　　　×{改行 2}
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

void ng_1_back_cursor_r(void) { // {↑}
    ng_move_cursor_with_ty_repeat(false, KC_UP, 1);
}
void ng_s1_back_cursor_r(void) { // +{↑}
    ng_move_cursor_with_ty_repeat(true, KC_UP, 1);
}
void ng_s7_back_cursor_r(void) { // +{↑ 7}
    ng_move_cursor_with_ty_repeat(true, KC_UP, 7);
}
void ng_1_forward_cursor_r(void) { // {↓}
    ng_move_cursor_with_ty_repeat(false, KC_DOWN, 1);
}
void ng_s1_forward_cursor_r(void) { // +{↓}
    ng_move_cursor_with_ty_repeat(true, KC_DOWN, 1);
}
void ng_s7_forward_cursor_r(void) { // +{↓ 7}
    ng_move_cursor_with_ty_repeat(true, KC_DOWN, 7);
}
void ng_1_previous_line_r(void) { // {→}
    ng_move_cursor_with_ty_repeat(false, KC_RIGHT, 1);
}
void ng_s1_previous_line_r(void) { // +{→}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 1);
}
void ng_s5_previous_line_r(void) { // +{→ 5}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 5);
}
void ng_s20_previous_line_r(void) { // +{→ 20}
    ng_move_cursor_with_ty_repeat(true, KC_RIGHT, 20);
}
void ng_1_next_line_r(void) { // {←}
    ng_move_cursor_with_ty_repeat(false, KC_LEFT, 1);
}
void ng_s1_next_line_r(void) { // +{←}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 1);
}
void ng_s5_next_line_r(void) { // +{← 5}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 5);
}
void ng_s20_next_line_r(void) { // +{← 20}
    ng_move_cursor_with_ty_repeat(true, KC_LEFT, 20);
}

void ng_1_up(void) { // {↑}
    ng_move_cursor_with_repeat(false, KC_UP, 1);
}
void ng_s1_up(void) { // +{↑}
    ng_move_cursor_with_repeat(true, KC_UP, 1);
}
void ng_s7_up(void) { // +{↑ 7}
    ng_move_cursor_with_repeat(true, KC_UP, 7);
}
void ng_1_down(void) { // {↓}
    ng_move_cursor_with_repeat(false, KC_DOWN, 1);
}
void ng_s1_down(void) { // +{↓}
    ng_move_cursor_with_repeat(true, KC_DOWN, 1);
}
void ng_s7_down(void) { // +{↓ 7}
    ng_move_cursor_with_repeat(true, KC_DOWN, 7);
}
void ng_1_right(void) { // {→}
    ng_move_cursor_with_repeat(false, KC_RIGHT, 1);
}
void ng_s1_right(void) { // +{→}
    ng_move_cursor_with_repeat(true, KC_RIGHT, 1);
}
void ng_s7_right(void) { // +{→ 7}
    ng_move_cursor_with_repeat(true, KC_RIGHT, 7);
}
void ng_1_left(void) { // {←}
    ng_move_cursor_with_repeat(false, KC_LEFT, 1);
}
void ng_s1_left(void) { // +{←}
    ng_move_cursor_with_repeat(true, KC_LEFT, 1);
}
void ng_s7_left(void) { // +{← 7}
    ng_move_cursor_with_repeat(true, KC_LEFT, 7);
}

void ng_edit_kakutei_down(void) { // {改行}{↓}
    ng_ime_complete();
    ng_forward_cursor();
}
void ng_edit_kakutei_left(void) { // {改行}{←}
    ng_ime_complete();
    ng_next_line();
}
void ng_edit_kakutei_end(void) { // {Enter}{End}
    ng_ime_complete();
    ng_end();
}
void ng_edit_delete_to_end(void) { // +{End}{BS}
#if defined(NG_BMP)
    bmp_send_string(SS_DOWN(X_LSFT));
    ng_end();
    bmp_send_string(SS_UP(X_LSFT));
    bmp_send_string("\b");
#else
    add_mods(MOD_BIT(KC_LEFT_SHIFT));
    ng_end();
    del_mods(MOD_BIT(KC_LEFT_SHIFT));
    tap_code(KC_BACKSPACE);
#endif
}
void ng_edit_s_home(void) { // +{Home}
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
void ng_edit_s_end(void) { // +{End}
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

void ng_edit_3_space(void) { // {Space 3}
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
void ng_edit_togaki(void) { // {Home}{改行}{Space 3}{←}
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
void ng_edit_serifu(void) { // {Home}{改行}{Space 1}{←}
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
void ng_edit_togaki_zengyo(void) { // {Home}{→}{End}{Del 4}{←}
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
void ng_edit_serifu_zengyo(void) { // {Home}{→}{End}{Del 2}{←}
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

void ng_parenthesis(void) { // (){改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagimaka")); // "（）"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("（）"));
    ng_back_cursor();
#endif
}
void ng_corner_bracket(void) { // 「」{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagikaka")); // "「」"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("「」"));
    ng_back_cursor();
#endif
}
void ng_white_corner_bracket(void) { // 『』{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("naginika")); // "『』"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("『』"));
    ng_back_cursor();
#endif
}
void ng_black_lenticular_bracket(void) { // 【】{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("nagisuka")); // "【】"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("【】"));
    ng_back_cursor();
#endif
}
void ng_double_angle_bracket(void) { // 《》{改行}{↑}
#if defined(NG_BMP) || defined(NG_USE_DIC)
    ng_ime_complete();
    dic_send_string(PSTR("naginiya")); // "《》"
    ng_back_cursor();
#else
    ng_send_unicode_string_P(PSTR("《》"));
    ng_back_cursor();
#endif
}
void ng_edit_next_line_corner_bracket(void) { // {改行}{End}{改行}「」{改行}{↑}
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
void ng_edit_next_line_space(void) { // {改行}{End}{改行}{Space}
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

void ng_edit_surround_parenthesis(void) { // ^x(^v){改行}{Space}+{↑}^x
    ng_cut();
    ng_parenthesis(); // (){改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
void ng_edit_surround_corner_bracket(void) { // ^x「^v」{改行}{Space}+{↑}^x
    ng_cut();
    ng_corner_bracket(); // 「」{改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
void ng_edit_surround_white_corner_bracket(void) { // ^x『^v』{改行}{Space}+{↑}^x
    ng_cut();
    ng_white_corner_bracket(); // 『』{改行}{↑}
    ng_paste();
    ng_forward_cursor();
    copy_spc_to_clipboard();
}
void ng_edit_surround_black_lenticular_bracket(void) { // ^x【^v】{改行}{Space}+{↑}^x
    ng_cut();
    ng_black_lenticular_bracket(); // 【】{改行}{↑}
    ng_paste();
    ng_forward_cursor();    // 1文字進む
    copy_spc_to_clipboard();
}
void ng_edit_surround_ruby(void) { // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
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
