# 最短経路問題
## Graph
`gcl::Graph<W>`は重み付き有向グラフを表す隣接リストである。頂点は 0-based index (`0, ..., N-1`) で管理される

`W`は全順序集合であり、加算および加算代入が可能なコンセプトである。
`W`は探索中に発生する`dist[u] + weight`がオーバーフローしない型を選ぶこと。

- `Graph<W>` は `std::vector<std::vector<Edge<W>>>`
- `Edge<W>` は `{to, weight}` を持つ

## BellmanFord
グラフと始点を渡すことで最短経路を求める。負辺を含んでも良い。
`gcl::bellman_ford`が存在する。
`gcl::bellman_ford`は`dist`と`negative_cycle_exist`をメンバ変数に持つ構造体を返す。

- `dist[v]`：始点から頂点`v`までの最短距離（到達不能な場合は`std::numeric_limits<W>::max()`）
- `negative_cycle_exist`：始点から到達可能な負閉路を含む場合`true`、そうでなければ`false`

以下のように使う。
```cpp
#include <bits/stdc++.h>
#include "../algorithm/shortest_path.hpp"

using namespace std;

int main(){
    int N = 3;
    gcl::Graph<long long> g(N);

    // 辺の追加
    g[0].push_back(gcl::Edge{1, 5});
    g[1].push_back(gcl::Edge{2, 6});
    g[2].push_back(gcl::Edge{0, 7});

    // グラフと始点を引数に取り、始点からの最短距離の配列と負閉路の有無を返す
    auto [dist, negative_cycle_exist] = gcl::bellman_ford(g, 0);
}
```

## Dijkstra
グラフと始点を渡すことで最短経路を求める。辺の重みは非負である必要がある。
最短距離だけを求めるメソッド`gcl::dijkstra_dist`と、最短距離およびその経路を求めるメソッド`gcl::dijkstra_path`が存在する。
`gcl::dijkstra_dist`は`dist`のみを返すが、`gcl::dijkstra_path`は`dist`と`parent`をメンバ変数に持つ構造体を返す。

- `dist[v]`：始点から頂点`v`までの最短距離（到達不能な場合は`std::numeric_limits<W>::max()`）
- `parent[v]`：`restore_path`による経路の復元に用いる、`v`の直前にある頂点（始点から`v`に到達不能な場合は`gcl::npos`）

以下のように使う。
```cpp
#include <bits/stdc++.h>
#include "../algorithm/shortest_path.hpp"

using namespace std;

int main(){
    int N = 3;
    gcl::Graph<long long> g(N);

    // 辺の追加
    g[0].push_back(gcl::Edge{1, 5});
    g[1].push_back(gcl::Edge{2, 6});
    g[2].push_back(gcl::Edge{0, 7});

    // グラフと始点を引数に取り、始点からの最短距離の配列を返す
    auto dist = gcl::dijkstra_dist(g, 0);
    // グラフと始点を引数に取り、始点からの最短距離の配列と各頂点の遷移元を返す
    auto result = gcl::dijkstra_path(g, 0); 

    // 以下のように経路を復元出来る
    // `restore_path(v)` は始点から `v` までの頂点列を返す
    auto path_to_1 = result.restore_path(1);
    auto path_to_2 = result.restore_path(2);
}

```
