#include <vector>
#include <numeric>
#include <cstddef>
#include <utility>
#include <ranges>

namespace gcl
{
    class UnionFind
    {
    public:
        /*
            @brief n 要素のUnionFindを構築する
        */
        explicit UnionFind(std::size_t size) : _size(size, std::size_t{1}), _parent(size)
        {

            std::iota(_parent.begin(), _parent.end(), 0);
        }

        // `id`の所属する集合の代表元を返す
        [[nodiscard]] std::size_t find(std::size_t id)
        {
            if (_parent[id] == id)
                return id;
            return _parent[id] = find(_parent[id]);
        }

        // `id`の所属する集合のサイズを返す
        [[nodiscard]] std::size_t size(std::size_t id)
        {
            return _size[find(id)];
        }

        // `left`と`right`の所属する集合が等しいか返す
        [[nodiscard]] bool same(std::size_t left, std::size_t right)
        {
            return find(left) == find(right);
        }

        // `left`と`right`の所属する集合を結合する
        // サイズが同じ集合同士は左に結合
        bool unite(std::size_t left, std::size_t right)
        {
            auto root_left = find(left);
            auto root_right = find(right);
            if (root_left == root_right)
                return false;
            auto size_left = _size[root_left];
            auto size_right = _size[root_right];
            if (size_left < size_right)
                std::swap(root_left, root_right);
            _parent[root_right] = root_left;
            _size[root_left] += _size[root_right];
            return true;
        }

    private:
        std::vector<std::size_t> _size, _parent;
    };
}
