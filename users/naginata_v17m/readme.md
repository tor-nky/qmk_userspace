このソースは eswaiさんの [naginata_v15](https://github.com/eswai/qmk_firmware/tree/master/users/naginata_v15) を元に作成しています。

次のようなバイナリを作成することができます。
* 現行のMS-IMEでも使用できる辞書登録式
WindowsとMacでは日本語キーボード、英語キーボードどちらでも使えます。
* IME設定を駆使して好みに近づけた方式
* BLE Micro Pro用(Bluetooth接続のiPhoneを含む)(bmp-vial 1.4.0対応)
# 薙刀式カナ配列キーマップ
薙刀式カナ配列による入力をQMKで実現します。薙刀式v17（ベスト版）に準拠しています。
編集モードも実装していますが、
編集モードでの記号入力方式がOSによって異なるので、
使用するOS(Windows、MacOS、Linux)によって切り替える必要があります。
切り替えは再コンパイル不要で、動的に切り替えられます。
## 薙刀式v17（ベスト版）とは
【薙刀式】v17ベスト版、発表。
http://oookaworks.seesaa.net/article/519707171.html#gsc.tab=0
## QMK Firmware　への組み込み方
1. [qmk_userspace/users/naginata_v17m/](https://github.com/tor-nky/qmk_userspace/tree/main/users/naginata_v17m) フォルダの内容を、各自の同様のフォルダを作りコピーする。
1. 各自の qmk_****/keyboards/%キーボード名%/keymaps/naginata_v17m/ フォルダに keymap.c を作成する。
1. こちらの [keymap.c](https://github.com/tor-nky/qmk_userspace/blob/main/keyboards/tor_nky/coconut42/keymaps/naginata_v15m/keymap.c) などを参考に、2つの `// 薙刀式` で囲まれた部分を付け加える。
OLED を使わなければ7番目は不要です。
1. コンパイルする `qmk compile -kb %キーボード名% -km naginata_v17m`
1. キーボードに書き込む
## OSなどの設定 (辞書使用の場合とBLE Micro Pro)
IMEへの辞書登録が必要ですが、他の設定はいりません。そのため、現行のMS-IMEでも使えます。
辞書使用の場合、config.h の中に ``#define NG_USE_DIC`` を書き加えてコンパイルします。

BLE Micro Pro の場合、vial-qmk/keyboards/ble_micro_pro/keymaps/ 下の [naginata_v15m/](https://github.com/tor-nky/vial-qmk/tree/kana/naginata/keyboards/ble_micro_pro/keymaps/naginata_v15m) と [vial/](https://github.com/tor-nky/vial-qmk/tree/kana/naginata/keyboards/ble_micro_pro/keymaps/vial) の内容の違いをお調べ下さい。
### 共通
下表のものを __辞書登録__ してください。
|単語|読み|参考|
|---|---|---|
|……|なぎてて|__なぎ__ なたしき __て__ ん __て__ ん|
|――|なぎよせ|__なぎ__ なたしき __よ__ こ __せ__ ん|
|×　　　×　　　×|なぎばつ|__なぎ__ なたしき __ばつ__|
|○|なぎまる|__なぎ__ なたしき __まる__|
|｜|なぎたせ|__なぎ__ なたしき __た__ て __せ__ ん|
|（|なぎひまか|__なぎ__ なたしき __ひ__ らき __ま__ る __か__ っこ|
|）|なぎとまか|__なぎ__ なたしき __と__ じ __ま__ る __か__ っこ|
|「|なぎひかか|__なぎ__ なたしき __ひ__ らき __か__ ぎ __か__ っこ|
|」|なぎとかか|__なぎ__ なたしき __と__ じ __か__ ぎ __か__ っこ|
|『|なぎひにか|__なぎ__ なたしき __ひ__ らき __に__ じゅう __か__ ぎかっこ|
|』|なぎとにか|__なぎ__ なたしき __と__ じ __に__ じゅう __か__ ぎかっこ|
|【|なぎひすか|__なぎ__ なたしき __ひ__ らき __す__ みつき __か__ っこ|
|】|なぎとすか|__なぎ__ なたしき __と__ じ __す__ みつき __か__ っこ|
|《|なぎひにや|__なぎ__ なたしき __ひ__ らき __に__ じゅう __や__ まかっこ|
|》|なぎとにや|__なぎ__ なたしき __と__ じ __に__ じゅう __や__ まかっこ|

### Windows辞書式の場合
古いIMEでは使えないかもしれません。

また、可能であれば次の設定をすると、かな入力中でも JK+Q (文末へ) が効くようになります。
IMEのキー設定
|* キー|入力/変換済み文字なし|他|
|---|:---:|:---:|
|Ctrl+Shift+変換| - |全確定|
### Mac辞書式の場合
日本語IMのライブ変換は使用できません。
M+Comma+Z を押して「　　　×　　　×　　　×」が入力できなければ、変換学習を一度リセットしてください。

また、「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加え、「英数」キーでIMをオフにしたとき「U.S.」になるようにすると、かな入力中でも JK+Q (文末へ) が効くようになります。
### Linux辞書式の場合
(Macの)英数キーを押すとIMEがオフになる Debian 13以降などに対応します。
config.h に `#define USB_POLLING_INTERVAL_MS 8` を書き加えます。

動作確認：Debian 13 → 良好
### iOS辞書式(BMP専用)の場合
iOSの日本語IMに備わっていないので「ひらがな変換」「カタカナ変換」「再変換」は使えません。
## OSの設定 (辞書を使わない場合)
### Windowsの場合
キーボード設定を日本語106キーボードにする。
[WinCompose](http://wincompose.info/)をインストールする。

IMEのキー設定
|* キー|入力/変換済み文字なし|他|
|---|:---:|:---:|
|Ctrl+Shift+無変換| - |全消去|
|Ctrl+Shift+変換| - |全確定|
### Macの場合
キーボードが日本語/英語どちらの設定でも動きます。
#### IM に「かわせみ」を使わない場合
「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加えます。
「英数」キーでIMをオフにしたとき「U.S.」になるようにしてください。

さらに、[Karabiner-Elements](https://karabiner-elements.pqrs.org/)をインストールします。
ファイル unicode_hex_input_switcher.json をフォルダ ~/.config/karabiner/assets/complex_modification/ にコピーし、
Karabiner-Elements に Unicode Hex Input Switcher を登録してください。

念のため、Karabiner-Elements の設定 Device で、本ファームウェアが入ったキーボードが有効になっているか確認してください。
#### IM に「かわせみ」を使用する場合
コード入力に Control+Option+Shift+かな を設定してください。
（config.h の中に ``#define NG_USE_KAWASEMI`` と ``#define UNICODE_KEY_MAC KC_NO`` を書き加えてコンパイルします）
### Linuxの場合
キーボードが日本語/英語どちらの設定でも動きます。

IMEのキー設定
|モード|入力キー|コマンド|備考|
|---|:---:|:---:|---|
|変換前入力中|Ctrl Shift Muhenkan|__キャンセル__|追加|
|変換中|〃|__〃__|追加|
|入力文字なし|Ctrl Space|__IME を 無効化__|追加|
|変換前入力中|〃|__〃__||
|変換中|〃|__〃__||

動作確認：Debian 12 → 良好、Debian 13 → 2個以上の記号は不可
## QMKファームウェアの設定
独自拡張として、標準のシフト&スペースに加えて、シフト&エンターキーを追加しました。
NG_SHFT2をキーマップに配置すると、単押しならエンター、
同時押しで薙刀式のシフトキーとして機能します。

OLEDが有効な場合には左側のOLEDには、
日本語入力モードに応じて「かな」または「ABC」と表紙されます。
右側のOLEDには薙刀式のロゴが表示されます。薙刀式のロゴは大岡俊彦氏に帰属します。

3キー同時押しのカーソル移動と、Delキーにだけキーリピートが効きます。

F+G を押さなくても 左右シフト＋英字 で 固定英数入力かIMEオフになるので、アルファベットの入力がすぐにできます。
再びかな入力にするときは H+J を押して IMEオン にします。
## キーボードの切り替え操作
以下の機能を動的に切り替えることができます。
設定内容をEEPROMに記録することができます。
| 設定項目 | 設定 | キーコード | 関数呼び出し |
|---|---|---|---|
| OS切り替え            | Windows  | NGSW_WIN  | switchOS(NG_WIN)  |
|                      | MacOS    | NGSW_MAC  | switchOS(NG_MAC)  |
|                      | Linux    | NGSW_LNX  | switchOS(NG_LNX)  |
|                      | iOS(BMP専用) | NGSW_IOS  | switchOS(NG_IOS)  |
| 後置シフト            | ON/OFFトグル   | NG_KOTI  | kouchi_shift_toggle()  |
| 現在設定の出力        |   | NG_SHOS   | ng_show_os()  |

本家のDvorakJ版薙刀式は前置シフトのみですが、
時間制限付き後置シフトも有効にできます。
naginata.h 内の `#define NG_KOUCHI_SHIFT_MS 60` にミリ秒単位で設定し、コンパイルします。

OLEDをオンにしているときは、設定の状態がOLEDに表示されます。
 * 1行目 OS設定。Windows、Mac、Linuxの頭文字です。
 * 2行目 縦書き(T)、横書き(Y)兼用
 * 3行目 後置シフト(Kでオン)
## 不具合
* 定義が設定されていないキーを押しても、何の代わりも出力しない
この場合、キーマップから該当するキーを NG_** でないものに変えてください。
* Windows 11以降のメモ帳では、表示が引っかかったり入力を正しく行えないことがあるため使えません。
* Windows の秀丸エディタでは、入力が速すぎると表示が乱れたりクラッシュすることがあります。
* 薙刀式オン(H+JまたはNG_ON)を押した後、シフトなどの装飾キーは一度離してください。
* (辞書式・新MS-IME) かな変換中に英数入力に切り替え、確定しないでキーを押すと最初の文字が入力されない。
* (辞書式・新MS-IME) かな変換中に Shift+英数 を押すと最初の文字が入力されない。
