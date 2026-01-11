# comp-pro-library
競技プログラミング用 C++ ライブラリ集。
提出用の **単一ファイル (`expanded.cpp`) を自動生成**することを前提に設計されています。

詳細な仕様・各データ構造やアルゴリズムの説明は `doc/` を参照してください。

## Quick Start
```sh
# main.cppにコードを追加する
python3 scripts/expand.py main.cpp -o expanded.cpp
g++ -O2 -o expanded ./expanded.cpp -std=c++20
```
- 提出時は `expanded.cpp` をそのまま使う
- `-std=c++20`を付け忘れるとコンパイルエラーになります

VSCode を利用している場合、`.vscode/tasks.json` を用いて  
`expand → compile` を`Ctrl+Shift+B`で実行できます。

## 特徴・設計方針
- 競技プログラミング向け（AtCoder / Codeforces / Library Checker を想定）
- 提出用に 1 ファイルへ展開 (expand) する運用を前提
- C++20 を前提とする
- 実行時エラーハンドリングは最小限（assert は使用する場合あり）
- ライブラリ部分は namespace 管理

## 収録内容
### データ構造
- [x] UnionFind（経路圧縮 + union by size）
- [x] Fenwick tree（点更新・区間和・点取得）
- [ ] Splay tree
- [ ] segment tree
- [ ] lazy segment tree
### アルゴリズム
- [x] Dijkstra（経路復元）
- [x] Bellman-Ford
- [ ] Floyd-Warshall
- [ ] 0-1 BFS

## Requirements
- C++20対応コンパイラ
    - 動作確認例：`g++ 13.3.0`
- Python3.x (expandスクリプト用)

## 使い方
1. ルートディレクトリの`main.cpp`に解きたい問題のコードを書く
2. ライブラリを展開する
```sh
python3 scripts/expand.py main.cpp -o expanded.cpp
```
3. 展開後の単一ファイルをコンパイル
```sh
g++ -O2 -o expanded ./expanded.cpp -std=c++20
```
4. `expanded.cpp`を提出する

## テスト
ライブラリには簡易テストを含みます。
```sh
./scripts/build_test.sh
```
- 各データ構造・アルゴリズムの基本動作を確認
- 一部Library Checkerでの動作を確認（詳細は`doc/`）

## ディレクトリ構成
```sh
.
├── algorithm/     # 特定データ構造に依存しないアルゴリズム
├── data-structure # データ構造
├── doc/           # ドキュメント
├── scripts/       # expand / test 用スクリプト
├── tests/         # テストコード
├── all.hpp        # algorithm/ data-structure/ のファイル全てをインクルードするためのファイル
├── main.cpp       # ユーザが編集する入口
├── expanded.cpp   # 自動生成（提出用）
├── .vscode        # 開発時のIntellisense等のコンフィグ, ビルドタスク
└── .devcontainer/ # devcontainer用ファイル群
```

## 注意点
- 提出前に必ずローカル環境でコンパイル・実行確認してください
