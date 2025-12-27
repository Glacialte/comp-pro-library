#include <bits/stdc++.h>

namespace glc
{
    class UnionFind
    {
    public:
        UnionFind(const std::size_t size) : _size(size, std::size_t{1}), _parent(size)
        {

            std::iota(_parent.begin(), _parent.end(), 0);
        }

        // `id`の所属する集合の代表元を返す
        [[nodiscard]] std::size_t get_root(const std::size_t id)
        {
            if (_parent[id] == id)
                return id;
            return _parent[id] = get_root(_parent[id]);
        }

        // `id`の所属する集合のサイズを返す
        [[nodiscard]] std::size_t get_size(const std::size_t id)
        {
            return _size[get_root(id)];
        }

        // `left`と`right`の所属する集合が等しいか返す
        [[nodiscard]] bool is_same(const std::size_t left, const std::size_t right)
        {
            return get_root(left) == get_root(right);
        }

        // `left`と`right`の所属する集合を結合する
        // サイズが同じ集合同士は左に結合
        bool unite(std::size_t left, std::size_t right)
        {
            auto root_left = get_root(left);
            auto root_right = get_root(right);
            if (root_left == root_right)
                return false;
            auto size_left = get_size(root_left);
            auto size_right = get_size(root_right);
            if (size_left < size_right)
                std::swap(root_left, root_right);
            _parent[root_right] = root_left;
            _size[root_left] += _size[root_right];
            return true;
        }

    private:
        std::vector<size_t> _size, _parent;
    };
}
