# comp-pro-library
## 概要
競技プログラミング用のライブラリを管理するリポジトリ。
ドキュメントはdoc/を参照してください。

今の所は以下を追加予定
- データ構造
    - [x] UnionFind
    - [ ] Splay tree
    - [x] Fenwick tree
    - [ ] segment tree
    - [ ] lazy segment tree
    - 面白そうな木
- アルゴリズム
    - [x] Dijkstra
        - [x] Library Checker
    - [x] Bellman-Ford
    - [ ] Floyd-Warshall
    - [ ] 0-1 BFS

## requirements
- C++20対応コンパイラ
- Python3

## 使い方
- ルートディレクトリのmain.cppにコードを追加
- 以下のコマンドでライブラリを展開し、それ単体でコンパイル可能なファイル（expanded.cpp）を作成する。
```sh
python3 scripts/expand.py main.cpp -o expanded.cpp
```
- 以下のコマンドでコンパイルする。-std=c++20を付け忘れるとエラーが発生するので注意してください。
```sh
g++ -o expanded ./expanded.cpp -std=c++20
```
