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

## Example
```cpp
#include <bits/stdc++.h>
#include "all.hpp"

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n_vertices, n_edges, start, goal;
    cin >> n_vertices >> n_edges >> start >> goal;

    using Weight = long long;
    gcl::WGraph<Weight> g(n_vertices);

    // 辺の入力（有向）
    while (n_edges--)
    {
        int from, to;
        Weight weight;
        cin >> from >> to >> weight;
        g[from].push_back(gcl::WEdge<Weight>{to, weight});
    }

    // Dijkstra（経路復元付き）
    auto result = gcl::dijkstra_path(g, start);

    // 最短距離（到達不能な場合は numeric_limits<Weight>::max()）
    cout << "distance to goal: " << result.dist[goal] << '\n';

    // 経路復元（start -> goal の頂点列）
    auto path = result.restore_path(goal);
    cout << "path to goal: ";
    for (auto v : path)
    {
        cout << v << ' ';
    }
    cout << '\n';
}
```

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
- [x] Kruskal

## Requirements
- C++20対応コンパイラ
    - 動作確認例：`g++ 13.3.0`
- Python3 (expandスクリプト用)
    - 動作確認済：`python3.8`以上

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
├── concepts/      # アルゴリズムやデータ構造が満たすべきコンセプト
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
