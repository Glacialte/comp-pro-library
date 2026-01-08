# Fenwick Tree
## concept
`ModuleLike`はFenwick Treeが内部で管理する値型に課される制約を表すコンセプトである。
このコンセプトは以下を要求する。

- 加算・減算が定義されていること（`+`, `-`, `+=`, `-=`）
- デフォルト構築が可能であること
- コピー可能であること

Fenwick Tree の実装では、`T{}` が **加法に関する単位元（零元）であることを仮定** している。
この性質はコンセプトでは静的に検証されないため、利用者が保証する必要がある。

## class
`gcl::FenwickTree<T>` は、`ModuleLike<T>` を満たす型 `T` に対して  
**一点加算および区間和（prefix sum）を対数時間で処理する** Fenwick Tree（Binary Indexed Tree）である。

内部的には 1-based index を用いるが、公開 API では **0-based index** を採用する。

## API
- `FenwickTree(std::size_t N)`：単位元`T{}`で初期化されたサイズ`N`の木を作る
- `FenwickTree(const std::vector<T>& v)`：`v`で初期化された木を作る
- `get(std::size_t idx)`：`idx`番目の値を取得する
- `add(std::size_t idx, T val)`：`idx`番目の値に`val`を加算する
- `sum(std::size_t left, std::size_t right)`：区間`[left, right)`の和を計算する
- `size()`：管理している要素数を返す

## 仕様例
```cpp
#include <vector>
#include <cassert>
#include "data-structure/fenwick_tree.hpp"
using namespace std;

int main(){
    vector<int> v = {1, 2, 4};
    gcl::FenwickTree<int> ft(v);

    int x = ft.sum(0, 3);
    assert(x == 7);
}
```