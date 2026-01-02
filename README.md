# comp-pro-library
## 概要
競技プログラミング用のライブラリを管理するリポジトリ。

今の所は以下を追加予定
- データ構造
    - [x] UnionFind
    - [ ] Splay tree
    - [ ] Fenwick tree
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
- Python

## 使い方
- main.cppにコードを追加
- 以下のコマンドでライブラリを展開
```sh
python3 scripts/expand.py main.cpp -o expanded.cpp
```
- 以下のコマンドでコンパイル
```sh
g++ -o expanded ./expanded.cpp -std=c++20
```
