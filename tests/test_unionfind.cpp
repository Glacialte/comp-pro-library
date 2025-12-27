#include <gtest/gtest.h>
#include "../data-structure/unionfind.hpp"

TEST(UnionFind, Unite)
{
    const auto size = std::size_t{5};
    glc::UnionFind uf(size);

    EXPECT_EQ(uf.unite(0, 1), true);  // {0, 1}, {2}, {3}, {4}
    EXPECT_EQ(uf.unite(2, 3), true);  // {0, 1}, {2, 3}, {4}
    EXPECT_EQ(uf.unite(0, 3), true);  // {0, 1, 2, 3}, {4}
    EXPECT_EQ(uf.unite(0, 2), false); // {0, 1, 2, 3}, {4}
    EXPECT_EQ(uf.unite(4, 4), false); // {0, 1, 2, 3}, {4}
}

TEST(UnionFind, GetSize)
{
    const auto size = std::size_t{3};
    glc::UnionFind uf(size);

    EXPECT_EQ(uf.get_size(0), 1);
    uf.unite(0, 1);
    uf.unite(0, 2);
    EXPECT_EQ(uf.get_size(0), 3);
}

TEST(UnionFind, GetRoot)
{
    const auto size = std::size_t{3};
    glc::UnionFind uf(size);

    EXPECT_EQ(uf.get_root(0), 0);
    uf.unite(0, 1); // {0, 1} root -> 0
    uf.unite(0, 2);
    EXPECT_EQ(uf.get_root(0), 0);
    EXPECT_EQ(uf.get_root(1), 0);
    EXPECT_EQ(uf.get_root(2), 0);
}
