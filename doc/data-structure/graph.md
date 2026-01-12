# Graph / WGraph
このファイルでは、グラフアルゴリズムで用いる **隣接リスト形式のグラフ表現** を定義している。  
重みなしグラフと重み付きグラフの両方を提供し、アルゴリズム側（最短路、探索など）とは分離された  
**データ構造定義**のみを扱う。

## 概要
- 頂点は **0-based index**（`0, ..., N-1`）で管理される
- グラフは **有向グラフ**として表現される
- 辺の集合は隣接リストとして管理される
- 本ファイルは **アルゴリズムを一切含まない**

## 重みなしグラフ
### struct `Edge`
`Edge` は重みを持たない辺を表す構造体である。
#### メンバ
- `std::size_t to`：辺の行き先頂点
#### 備考
- 辺の始点は、隣接リストを保持している頂点のインデックスによって表現される


### type `Graph`
```cpp
using Graph = std::vector<std::vector<Edge>>;
```

`Graph`は、重みなし有向グラフを表す隣接リスト型である。

- `Graph[u]`は、頂点`u`から出るすべての辺の集合を表す
- BFS、DFS、トポロジカルソート、SCC など重みを必要としないグラフアルゴリズムを想定している

## 重み付きグラフ
### struct `WEdge<W>`
`WEdge<W>`は、重み付き辺を表す構造体である。
#### テンプレート引数
- `W`：辺の重みを表す型
#### メンバ
- `std::size_t to`：辺の行き先頂点
- `W weight`：辺の重み
#### 備考
- 重み型`W`には制約を課していない（加算や比較が可能であることなどの要件は、アルゴリズム側のコンセプトで課される）

### type `WGraph<W>`
```cpp
template <typename W>
using WGraph = std::vector<std::vector<WEdge<W>>>;
```
`WGraph<W>`は、重み付き有向グラフを表す隣接リスト型である。
- `WGraph<W>[u]`は、頂点`u`から出る全ての重み付き辺の集合を表す
- `W`の型は、最短経路アルゴリズムなどで使用される重み型に対応する

## 使用例
```cpp
#include "data-structure/graph.hpp"

int main(){
    int N = 3;
    gcl::Graph g(N);

    g[0].push_back(gcl::Edge{1});
    g[1].push_back(gcl::Edge{2});
}
```
```cpp
#include "data-structure/graph.hpp"

int main(){
    int N = 3;
    gcl::WGraph<long long> g(N);

    g[0].push_back(gcl::WEdge{1, 5});
    g[1].push_back(gcl::WEdge{2, 6});
}
```

## 重み付き無向辺
### struct `UWEdge<W>`
`UWEdge<W>`は、端点を2つ持つ重み付き無向辺を表す構造体である。
隣接リスト（`UGraph<W>`）の要素である`WEdge<W>`とは異なり、`UWEdge<W>`は始点・終点の両方を保持する。

#### テンプレート引数
- `W`：辺の重みを表す型
#### メンバ
- `std::size_t from`：辺の一方の端点
- `std::size_t to`：辺のもう一方の端点
- `W weight`：辺の重み
#### 備考
- `UWEdge<W>` は **無向辺の集合**として扱う用途を想定している（例：Kruskal法）。
- `W` の要件（比較可能など）はアルゴリズム側で課される。
- `from/to/weight` は`std::size_t` および `W` に変換可能な型から構築できる（入力の `int` や `long long` を受けやすくするため）。

## 関連
- 重み付きグラフに対する制約は`graph_concepts.hpp`に定義されている
- 最短経路問題アルゴリズムは`algorithm/shortest_path.hpp`を参照
- 最小全域木（Kruskal法）は`algorithm/minimum_spanning_tree.hpp`を参照