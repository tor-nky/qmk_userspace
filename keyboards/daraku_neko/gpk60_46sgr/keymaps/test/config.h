//#define QUICK_TAP_TERM 0
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// fix iPhone and iPad power adapter issue
// iOS device need lessthan 100
// #define USB_MAX_POWER_CONSUMPTION 100

#ifdef RGBLIGHT_ENABLE
    #define RGB_MATRIX_SLEEP // turn off effects when suspended
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #define RGBLIGHT_EFFECT_SNAKE
    #define RGBLIGHT_EFFECT_KNIGHT
    #define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_TWINKLE
#endif
