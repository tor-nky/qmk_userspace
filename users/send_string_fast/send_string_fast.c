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

// 文字列をできるかぎりロールオーバーしながら高速出力
// 出力原理:
//    6KRO(Macを除く) なら、押したキーを最大6個ためて一度に送出し、離すのも一度に出力（最速）
//    6KRO で Mac なら、USBキーコード順にキーが続くかぎり最大6個ためる（やや高速）
//    NKRO なら、USBキーコード順にキーが続くかぎりためて一度に送出し、離すのも一度に出力（やや高速）
void send_string_fast(const char *str, bool is_not_apple) {
    bool is_output = false;
    bool is_nkro = false;
    #ifdef NKRO_ENABLE
        // is_nkro = keyboard_protocol && keymap_config.nkro;   // QMK Firmware 0.26.x 以前
        // is_nkro = usb_device_state_get_protocol() && keymap_config.nkro;   // QMK Firmware 0.27 〜 0.28.x
        is_nkro = host_can_send_nkro() && keymap_config.nkro;   // QMK Firmware 0.29 以降
    #endif
    #ifdef CONSOLE_ENABLE
        uint keys = 0;
        uint ms = 0;
    #endif
    bool is_dead = false;
    // SS_TAP(), SS_DOWN(), SS_UP(), SS_DELAY() 以降は関数 send_string() にまかせる
    while (is_dead || pgm_read_byte(str) > SS_QMK_PREFIX) {
        bool is_shifted = false;
        bool is_altgred = false;
        // すでにバッファに入っている文字を避ける、is_shifted と is_altgred が同じ、
        // NKROまたはMacではUSBキーコード順で、何文字続くか探索
        uint8_t sendable_length = 0;
        bool last_has_space = is_key_pressed(KC_SPACE); // QMK Firmware 0.23 以降
        {
            const char *str_copy = str;
            char ascii_code;
            bool is_dead_now = is_dead;
            uint8_t last_keycode = 0;
            for (uint8_t len = 0; is_dead_now || (ascii_code = pgm_read_byte(str_copy)) > SS_QMK_PREFIX; len++) {
                uint8_t keycode;
                bool is_shifted_now = false;
                bool is_altgred_now = false;
                if (is_dead_now) {
                    keycode = KC_SPACE;
                } else {
                    // 参考: qmk_firmware/quantum/send_string/send_string.c
                    keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
                    //      #define PGM_LOADBIT(mem, pos) ((pgm_read_byte(&((mem)[(pos) / 8])) >> ((pos) % 8)) & 0x01)
                    is_shifted_now = (pgm_read_byte(&((ascii_to_shift_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                    is_altgred_now = (pgm_read_byte(&((ascii_to_altgr_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                }
                // バッファにあるのと同じキー
                if ((len > 0 && (is_shifted_now != is_shifted || is_altgred_now != is_altgred))
                    || ((is_nkro || !is_not_apple) && keycode < last_keycode)
                    || (!is_nkro && len >= KEYBOARD_REPORT_KEYS)
                    || is_key_pressed(keycode)  // QMK Firmware 0.23 以降
                ) {
                    break;
                }
                last_keycode = keycode;
                if (is_dead_now) {
                    is_dead_now = false;
                } else {
                    str_copy++;
                    is_shifted = is_shifted_now;
                    is_altgred = is_altgred_now;
                    is_dead_now = (pgm_read_byte(&((ascii_to_dead_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                }
            }
            sendable_length = str_copy - str;
            // #ifdef CONSOLE_ENABLE
            //     uprintf("«%u»", sendable_length);   // 何文字見つかったか
            // #endif
        }
        // バッファをクリア
        clear_keys();

        if (is_shifted) add_mods(MOD_BIT(KC_LEFT_SHIFT));
        else            del_mods(MOD_BIT(KC_LEFT_SHIFT));
        if (is_altgred) add_mods(MOD_BIT(KC_RIGHT_ALT));
        else            del_mods(MOD_BIT(KC_RIGHT_ALT));
        // バッファにためる
        {
            uint8_t last_keycode = 0;
            for (uint8_t len = 0; is_dead || sendable_length > 0; len++) {
                char ascii_code;
                uint8_t keycode;
                bool is_shifted_now = false;
                bool is_altgred_now = false;
                if (is_dead) {
                    keycode = KC_SPACE;
                } else {
                    ascii_code = pgm_read_byte(str);
                    keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
                    is_shifted_now = (pgm_read_byte(&((ascii_to_shift_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                    is_altgred_now = (pgm_read_byte(&((ascii_to_altgr_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                }
                // バッファにあるのと同じキー
                if ((is_shifted_now != is_shifted || is_altgred_now != is_altgred)
                    || ((is_nkro || !is_not_apple) && keycode < last_keycode)
                    || (!is_nkro && len >= KEYBOARD_REPORT_KEYS)
                    || (is_dead && last_has_space)
                    || is_key_pressed(keycode)  // QMK Firmware 0.23 以降
                ) {
                    break;
                }
                add_key(keycode);
                #ifdef CONSOLE_ENABLE
                    keys++;
                    if (is_dead) {
                        print("†");
                    } else {
                        uprintf("%c", ascii_code);
                    }
                #endif
                last_keycode = keycode;
                if (is_dead) {
                    is_dead = false;
                } else {
                    is_output = true;
                    str++;
                    sendable_length--;
                    is_dead = (pgm_read_byte(&((ascii_to_dead_lut)[((uint8_t)ascii_code) / 8])) >> (((uint8_t)ascii_code) % 8)) & 0x01;
                }
            }
        }
        // バッファを吐き出す
        send_keyboard_report();
        #ifdef CONSOLE_ENABLE
            ms += (TAP_CODE_DELAY) > 0 ? (TAP_CODE_DELAY) : 1;
            print("⌨");
        #endif
    }
    if (is_output) {
        // 終了処理
        del_mods(MOD_BIT(KC_LEFT_SHIFT) | MOD_BIT(KC_RIGHT_ALT));
        clear_keys();
        send_keyboard_report();
        #ifdef CONSOLE_ENABLE
            ms += (TAP_CODE_DELAY) > 0 ? (TAP_CODE_DELAY) : 1;
            uprintf("⌨\nSend %u key(s) in %u ms.\n", keys, ms);
        #endif
    }

    // SS_TAP(), SS_DOWN(), SS_UP(), SS_DELAY() 以降は関数 send_string() にまかせる
    if (pgm_read_byte(str) == SS_QMK_PREFIX) {
        send_string(str);
    }
}
