/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define QUICK_TAP_TERM 0
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// fix iPhone and iPad power adapter issue
// iOS device need lessthan 100
// #define USB_MAX_POWER_CONSUMPTION 100

#ifdef RGBLIGHT_ENABLE
    #define RGB_MATRIX_SLEEP // turn off effects when suspended
    #undef RGBLIGHT_ANIMATIONS
    #undef RGBLIGHT_EFFECT_BREATHING
    #undef RGBLIGHT_EFFECT_RAINBOW_MOOD
    #undef RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #undef RGBLIGHT_EFFECT_SNAKE
    #undef RGBLIGHT_EFFECT_KNIGHT
    #undef RGBLIGHT_EFFECT_CHRISTMAS
    #undef RGBLIGHT_EFFECT_STATIC_GRADIENT
    #undef RGBLIGHT_EFFECT_RGB_TEST
    #undef RGBLIGHT_EFFECT_ALTERNATING
    #undef RGBLIGHT_EFFECT_TWINKLE
#endif

// Squeezing the most out of AVR
//  (from qmk_firmware/docs/squeezing_avr.md)
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_ACTION_ONESHOT   // 598 bytes (v0.25.17)
// #define NO_ACTION_TAPPING
#define NO_MUSIC_MODE
#define LAYER_STATE_8BIT    // 102 bytes (v0.25.17)


// #define NG_USE_KAWASEMI // Mac汎用 → かわせみ専用
// #define UNICODE_KEY_MAC KC_NO   // Keycodes used for starting Unicode input on different platforms

// Forces the keyboard to wait for a USB connection to be established before it starts up
#define USB_WAIT_FOR_ENUMERATION
