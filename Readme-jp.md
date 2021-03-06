# PDFontTool

PDFontTool は TrueType Font(.ttf), OpenType Font(.otf) など、Windows で読み込めるフォントを [Playdate](https://play.date/jp/) で使用できるフォント (.fnt + .png) に変換する Windows (10 以降) 用のコマンドラインツールです。

Windows の Direct2D を利用して、アンチエイリアス無しで白黒キャンバスに直接レンダリングしています。アウトラインフォントも細かな設定無しに、それなりに綺麗なフォントに変換できます。

[English](Readme.md)

## 変換サンプル

![PDFontTool](./images/PDFontTool.gif)

|NotoSansJP-Thin 16px|NotoSansJP-Thin 24px|NotoSansJP-Medium 24px|
|---|---|---|
|![NotoSansJP-Thin 16px](./images/NotoSansJP-Thin-16.png)|![NotoSansJP-Thin 24px](./images/NotoSansJP-Thin-24.png)|![NotoSansJP-Medium 24px](./images/NotoSansJP-Medium-24.png)|

|Lato-Thin 16px|Lato-Regular 24px|Lato-BoldItalic 24px|
|---|---|---|
|![Lato-Thin 16px](./images/Lato-Thin-16.png)|![Lato-Regular 24px](./images/Lato-Regular-24.png)|![Lato-BoldItalic 24px](./images/Lato-BoldItalic-24.png)|

## フォルダ構成

```text
+- pdft
|   +- bin  # PDFontTool 実行ファイル (pdft.exe)
|   |
|   +- src  # PDFontTool ソース
|
+- .vscode  # Visual Stucio Code 用 バッチファイル
|
+- Source   # Playdate 用 テストアプリケーション (PDFontTool.pdx) ソース
```

## 利用方法

```bat
pdft [OPTIONS]... [SOURCE] [OUTPUT FOLDER] [OUTPUT FILENAME] [FONT SIZE]
```

### SOURCE

ソースフォントのファイルパス。

### OUTPUT FOLDER

出力先のフォルダパス。

### OUTPUT FILENAME

出力ファイル名。（拡張子は必要ありません）

### FONT SIZE

フォントサイズ。 (ピクセル指定)

### OPTIONS

#### -g

グリッドサイズ（生成されるPNG画像のグリフ 1 つ分のサイズ）を width,height で指定する。

- 指定しない場合、フォントの描画サイズから自動で計算します。

#### -R

文字幅を再計算する。
**(等幅フォント以外には、このオプションは必要はありません。通常は指定しないでください。)**

- 過去に作成された漢字ビットマップフォントには、等幅フォントのものが多く、特に ASCII 範囲の見栄えが良くありません。PDFontTool では、実際のフォント幅を測定、反映するオプションを実装し、見栄えを改善するようにしました。
- 文字幅の測定に GDI+ の GetPixel を利用しているため、実行時間がかなり長くなります。

#### -ma

アスキー範囲のマージン。

- -R オプションと同時に指定する必要があります。

#### -mo

アスキー範囲外のマージン。

- -R オプションと同時に指定する必要があります。

## PDFontTool.pdx

PDFontTool.pdx は pdft.exe で生成したフォントを確認するための Playdate 用テストアプリケーションです。
Visual Studio Code を利用して、ビルド、実行できるようにしています。

## 翻訳

作者が作成するドキュメントの元は日本語です。英語ドキュメントは機械翻訳を元にしているか、作成されていません。よろしければ、英語ドキュメントの作成、および間違いの指摘にご協力いただけると幸いです。

## 付記

- pdft.exe は Windows OS の機能に大きく依存しているため、その他のプラットフォームで動作させるのは難しいです。
- Playdate は Panic Inc. の製品です。 PDFontTool は Panic Inc. 公式のツールではありません。
- PDFontTool を使用したことによって生じたいかなる損害にも、当方は一切の責任を負いません。
