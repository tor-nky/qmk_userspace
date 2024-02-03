//#define QUICK_TAP_TERM 0
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// fix iPhone and iPad power adapter issue
// iOS device need lessthan 100
//#define USB_MAX_POWER_CONSUMPTION 100

#define UNICODE_TYPE_DELAY 0


#define NG_USE_KAWASEMI // Mac汎用 → かわせみ専用

#ifdef NG_USE_KAWASEMI
// Keycodes used for starting Unicode input on different platforms
#   define UNICODE_KEY_MAC KC_NO   // かわせみ3用
#endif

// Forces the keyboard to wait for a USB connection to be established before it starts up
#define WAIT_FOR_USB
