# mywc

C++23を利用したシンプルな`wc`コマンド風ツール
A simple `wc`-like utility written in C++23.

---

## 概要

mywc は UNIX の `wc` コマンドを参考に作成した、行数・単語数・バイト数を集計するコマンドラインツールです。

このプログラムは作者が進めているC++学習プロジェクト群の一環として作成されました。

---

## 特徴

* 行数の集計 (`-l`)
* 単語数の集計 (`-w`)
* バイト数の集計 (`-b`)
* 複数ファイルの同時処理
* 標準入力の処理
* `--help`
* `--version`

---

## 動作環境

* C++23 対応コンパイラ
* CMake 4.1 以降

開発・動作確認環境:

* GCC
* WSL (Windows Subsystem for Linux)

---

## ビルド

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## 使用方法

```bash
mywc [OPTION]... [FILE]...
```

例:

```bash
mywc sample.txt

mywc -l sample.txt

mywc -lw sample.txt

mywc file1.txt file2.txt
```

標準入力:

```bash
cat sample.txt | mywc
```

---

## オプション

| Option      | Description         |
| ----------- | ------------------- |
| `-l`        | 行数を表示           |
| `-w`        | 単語数を表示         |
| `-b`        | バイト数を表示       |
| `--help`    | ヘルプを表示         |
| `--version` | バージョン情報を表示  |

---

## 実装について

このプロジェクトでは以下の学習を目的としています。

* コマンドライン引数解析
* 列挙型 (`enum class`)
* 演算子オーバーロード
* `std::expected`
* ファイル入出力
* STL コンテナ
* エラーハンドリング
* CMake によるビルド管理

---

## 制限事項

* GNU wc の完全互換実装ではありません。
* 文字数ではなくバイト数を集計します。
* UTF-8 文字数のカウントには対応していません。
* GNU wc と出力形式やエラー処理が異なる場合があります。

---

## 開発状況

現在のバージョンは一通りの機能実装を完了した状態です。

今後は不具合修正やリファクタリングを除き、大きな機能追加の予定はありません。

---

## バージョン

```text
mywc 6.0
```

---

## ライセンス

このプロジェクトはMITライセンスに基づきライセンスされています．
詳細はLICENSEファイルを確認してください．

---

## 関連プロジェクト

本プロジェクトは C++ 学習プロジェクト群の一環として開発されています。

他のトレーニングプロジェクトについては以下を参照してください。

| プロジェクト | 状況 | リンク | 概要 |
|------------|------|--------|------|
| mywc | 完了 | https://github.com/Dize-Azki6674/mywc | wc 風行数カウントツール |
| mygrep | 準備中 | https://github.com/Dize-Azki6674/mygrep | grep 風文字列検索ツール |

---

## 作者

Azkey

---

## Overview

mywc is a simple command-line utility inspired by the UNIX `wc` command.

It counts lines, words, and bytes in text files and standard input.

This project is part of a personal C++ training series.

---

## Features

* Line counting (`-l`)
* Word counting (`-w`)
* Byte counting (`-b`)
* Multiple file support
* Standard input support
* `--help`
* `--version`

---

## Requirements

* C++23 compatible compiler
* CMake 4.1 or newer

Tested with:

* GCC
* WSL (Windows Subsystem for Linux)

---

## Build

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

---

## Usage

```bash
mywc [OPTION]... [FILE]...
```

Examples:

```bash
mywc sample.txt

mywc -l sample.txt

mywc -lw sample.txt

mywc file1.txt file2.txt
```

Standard input:

```bash
cat sample.txt | mywc
```

---

## Options

| Option      | Description                 |
| ----------- | --------------------------- |
| `-l`        | Print line counts           |
| `-w`        | Print word counts           |
| `-b`        | Print byte counts           |
| `--help`    | Display help                |
| `--version` | Display version information |

---

## Implementation Notes

This project was created for learning modern C++.

Topics practiced in this project include:

* Command-line argument parsing
* Enumerations (`enum class`)
* Operator overloading
* `std::expected`
* File I/O
* STL containers
* Error handling
* Build management with CMake

---

## Limitations

* This is not a fully compatible GNU wc implementation.
* Counts bytes rather than Unicode characters.
* UTF-8 character counting is not supported.
* Output formatting and error handling may differ from GNU wc.

---

## Development Status

The current version has completed the implementation of all essential features.

Going forward, there are no plans for major feature additions other than bug fixes and refactoring.

---

## Version

```text
mywc 6.0
```

---

## License

This project is licensed under the MIT License.
See the LICENSE file for details.

---

## See Also

This project is part of a series of C++ training projects.

The current status of other projects in the training series can be found below.

| Project | Status | Link | Description |
|----------|----------|--------|-------------|
| mywc | Completed | https://github.com/Dize-Azki6674/mywc | A wc-like text statistics utility |
| mygrep | Under Construction | https://github.com/Dize-Azki6674/mygrep | A grep-like string search utility |

---

## Author

Azkey
