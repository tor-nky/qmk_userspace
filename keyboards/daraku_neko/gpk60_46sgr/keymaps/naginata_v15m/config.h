//#define QUICK_TAP_TERM 0
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// fix iPhone and iPad power adapter issue
// iOS device need lessthan 100
//#define USB_MAX_POWER_CONSUMPTION 100


// 薙刀式の編集モード、固有名詞が不要であればコメントアウト
// #define NG_NO_HENSHU1
// #define NG_NO_HENSHU2
#define NG_NO_KOYUMEISHI


// Linuxで使うときは下1行を有効にする
//#define USB_POLLING_INTERVAL_MS 8   // sets the USB polling rate in milliseconds
//#define TAP_CODE_DELAY 24   // Sets the delay between `register_code` and `unregister_code`

#define NG_USE_KAWASEMI // Mac汎用 → かわせみ専用

#ifdef NG_USE_KAWASEMI
// Keycodes used for starting Unicode input on different platforms
#   define UNICODE_KEY_MAC KC_NO   // かわせみ3用
#endif

// Forces the keyboard to wait for a USB connection to be established before it starts up
#define WAIT_FOR_USB
