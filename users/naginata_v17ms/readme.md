このソースは eswaiさんの [naginata_v15](https://github.com/eswai/qmk_firmware/tree/master/users/naginata_v15) を元に作成しています。
**AVRマイコンでは動きません。** RP2040マイコンなど他のマイコンを使います。
# 薙刀式カナ配列キーマップ
薙刀式カナ配列による入力をQMKで実現します。薙刀式v17（ベスト版）に準拠しています。
編集モードも実装していますが、
編集モードでの記号入力方式がOSによって異なるので、
使用するOS(Windows、MacOS、Linux)によって切り替える必要があります。
切り替えは再コンパイル不要で、動的に切り替えられます。
## 薙刀式とは
【薙刀式】v17ベスト版、発表。
https://oookaworks.seesaa.net/article/519707171.html#gsc.tab=0
## QMK Firmware　への組み込み方
1. [qmk_userspace/users/naginata_v17ms/](https://github.com/tor-nky/qmk_userspace/tree/main/users/naginata_v17ms) フォルダの内容を、各自の同様のフォルダを作りコピーする。
1. 各自の qmk_****/keyboards/%キーボード名%/keymaps/naginata_v17ms/ フォルダに keymap.c を作成する。
1. こちらの [keymap.c](https://github.com/tor-nky/qmk_userspace/blob/main/keyboards/tor_nky/coconut42/keymaps/naginata_v17m/keymap.c) などを参考に、2つの `// 薙刀式` で囲まれた部分を付け加える。  
OLED を使わなければ7番目は不要です。
1. コンパイルする `qmk compile -kb %キーボード名% -km naginata_v17ms`
1. キーボードに書き込む
## OSの設定
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

「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加えます。
「英数」キーでIMをオフにしたとき「U.S.」になるようにしてください。

さらに、[Karabiner-Elements](https://karabiner-elements.pqrs.org/)をインストールします。
ファイル unicode_hex_input_switcher.json をフォルダ ~/.config/karabiner/assets/complex_modification/ にコピーし、
Karabiner-Elements に Unicode Hex Input Switcher を登録してください。

念のため、Karabiner-Elements の設定 Device で、本ファームウェアが入ったキーボードが有効になっているか確認してください。
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

動作確認：Debian 13 → 2個以上の記号は不可
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
| 縦書き、横書き        | ON/OFFトグル   | NG_TAYO    | tategaki_toggle()  |
| 後置シフト            | ON/OFFトグル   | NG_KOTI  | kouchi_shift_toggle()  |
| 現在設定の出力        |   | NG_SHOS   | ng_show_os()  |

OLEDをオンにしているときは、設定の状態がOLEDに表示されます。
 * 1行目 OS設定。Windows、Mac、Linuxの頭文字です。
## 不具合
* 定義が設定されていないキーを押しても、何の代わりも出力しない  
この場合、キーマップから該当するキーを NG_** でないものに変えてください。
* Windows 11以降のメモ帳では、表示が引っかかったり入力を正しく行えないことがあるため使えません。
* Windows の秀丸エディタでは、入力が速すぎると表示が乱れたりクラッシュすることがあります。
* 薙刀式オン(H+JまたはNG_ON)を押した後、シフトなどの装飾キーは一度離してください。
