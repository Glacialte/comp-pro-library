# 最小全域木（Minimum Spanning Tree）
この章では、重み付き無向グラフに対する **最小全域木（MST）** および **最小全域森（MSF）** を求めるアルゴリズムを提供する。

グラフの表現（`WGraph` / `WEdge` / `UWEdge`）については、`data-structure/graph.hpp` のドキュメントを参照すること。

## 前提となるグラフ
本章で提供するアルゴリズムは、**重み付き無向グラフ**を対象とする。

- 頂点は **0-based index**（`0, ..., N-1`）で管理される
- 無向グラフは **両方向に同じ重みの辺が登録されている**ことを前提とする  
  （すなわち `u -> v` と `v -> u` の両方が存在する）
- 重み型 `W` は比較および加算が可能である必要がある  
  （具体的な制約はアルゴリズム側で仮定する）

Kruskal 法は、グラフが連結でない場合にも適用でき、その場合は  
**最小全域森（各連結成分ごとの最小全域木の集合）**を返す。

---

## Kruskal 法
`gcl::kruskal` は、与えられた重み付き無向グラフに対して  
`Kruskal` 法により最小全域木（または最小全域森）を構築する。

### 返り値：`KruskalResult<W>`
`gcl::kruskal` は以下の構造体を返す。

- `total_weight`：採用された全ての辺の重みの総和（グラフが非連結の場合でも、最小全域森の重みの総和）

- `edges`：最小全域木（または森）を構成する **無向辺の集合**。型は `std::vector<UWEdge<W>>`

- `is_connected`：入力グラフが連結であり、最小全域木が構成できた場合 `true`、非連結で最小全域森となった場合 `false`

### 補助メソッド
`KruskalResult<W>` には以下のメソッドが用意されている。

- `build_mst(n_vertices)`  
  辺集合 `edges` から、無向の隣接リスト表現（`WGraph<W>`）を構築する。  
  非連結の場合は「森」を表す隣接リストが返る。

---

## edge list を用いた利用方法
辺集合（無向辺の配列）を直接与えて Kruskal 法を実行する。

```cpp
#include <bits/stdc++.h>
#include "../algorithm/minimum_spanning_tree.hpp"
#include "../data-structure/graph.hpp"

using namespace std;

int main(){
    using W = long long;

    int N = 3;
    vector<gcl::UWEdge<W>> edges = {
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3},
    };

    auto result = gcl::kruskal(N, edges);

    // 最小全域木の重み
    auto total = result.total_weight; // 3

    // 連結かどうか
    bool connected = result.is_connected; // true

    // 隣接リストとして構築
    auto mst = result.build_mst(N);
}
```

## 隣接リスト（`WGraph`）を用いた利用方法
重み付き無向グラフを隣接リストで与えて Kruskal 法を実行する。

```cpp
#include <bits/stdc++.h>
#include "../algorithm/minimum_spanning_tree.hpp"
#include "../data-structure/graph.hpp"

using namespace std;

int main(){
    using W = long long;

    int N = 4;
    gcl::WGraph<W> g(N);

    auto add_undirected = [&](int u, int v, W w){
        g[u].push_back(gcl::WEdge{v, w});
        g[v].push_back(gcl::WEdge{u, w});
    };

    add_undirected(0, 1, 1);
    add_undirected(1, 2, 2);
    add_undirected(2, 3, 3);
    add_undirected(0, 3, 10);

    auto result = gcl::kruskal(g);

    // 最小全域木の重み
    auto total = result.total_weight; // 6

    // 最小全域木を隣接リストとして取得
    auto mst = result.build_mst(N);
}
```

### 備考
- `Kruskal` 法は辺数を `M`、頂点数を `N` とすると計算量は `O(M log M)` である。
- グラフが非連結の場合でも処理は失敗せず、最小全域森が返される点に注意すること。
- 無向グラフとして扱うため、隣接リストは必ず両方向に辺を追加する必要がある。

### 関連
- グラフ構造の定義：`data-structure/graph.hpp`
- Union-Find：`data-structure/unionfind.hpp`