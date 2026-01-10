# Union-Find（Disjoint Set Union）

## class
`gcl::UnionFind` は、`0..N-1` の要素からなる集合族を管理する **Union-Find（DSU）** である。  
要素の属する集合の代表元（root）を求める `find` と、集合の結合 `unite` を提供する。

本実装は **経路圧縮**（`find`）と **union by size**（`unite`）により、各操作はならしで高速に動作する。

## API
- `UnionFind(std::size_t N)`  
  `N` 要素の Union-Find を構築する（初期状態では各要素が別集合）

- `find(std::size_t id)`  
  `id` が属する集合の代表元を返す（経路圧縮あり）

- `size(std::size_t id)`  
  `id` が属する集合の要素数を返す

- `same(std::size_t left, std::size_t right)`  
  `left` と `right` が同じ集合に属するか返す

- `unite(std::size_t left, std::size_t right)`  
  2つの集合を結合する  
  - すでに同一集合なら `false`  
  - 結合した場合は `true` を返す  
  - サイズが同じ集合同士の場合は **左側（`left` 側の root）に結合**する

## 使用例
```cpp
#include <cassert>
#include "data-structure/unionfind.hpp"

int main() {
    gcl::UnionFind uf(5);

    uf.unite(0, 1);
    uf.unite(3, 4);

    assert(uf.same(0, 1));
    assert(!uf.same(1, 2));

    uf.unite(1, 2);
    assert(uf.size(0) == 3);   // {0,1,2}

    assert(uf.find(0) == uf.find(2));
}
