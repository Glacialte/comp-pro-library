#include <gtest/gtest.h>
#include "../data-structure/unionfind.hpp"

TEST(UnionFind, Unite)
{
    const auto size = std::size_t{5};
    gcl::UnionFind uf(size);

    EXPECT_EQ(uf.unite(0, 1), true);  // {0, 1}, {2}, {3}, {4}
    EXPECT_EQ(uf.unite(2, 3), true);  // {0, 1}, {2, 3}, {4}
    EXPECT_EQ(uf.unite(0, 3), true);  // {0, 1, 2, 3}, {4}
    EXPECT_EQ(uf.unite(0, 2), false); // {0, 1, 2, 3}, {4}
    EXPECT_EQ(uf.unite(4, 4), false); // {0, 1, 2, 3}, {4}
}

TEST(UnionFind, Size)
{
    const auto size = std::size_t{3};
    gcl::UnionFind uf(size);

    EXPECT_EQ(uf.size(0), 1);
    uf.unite(0, 1);
    uf.unite(0, 2);
    EXPECT_EQ(uf.size(0), 3);
}

TEST(UnionFind, Find)
{
    const auto size = std::size_t{3};
    gcl::UnionFind uf(size);

    EXPECT_EQ(uf.find(0), 0);
    uf.unite(0, 1); // {0, 1} root -> 0
    uf.unite(0, 2);
    EXPECT_EQ(uf.find(0), 0);
    EXPECT_EQ(uf.find(1), 0);
    EXPECT_EQ(uf.find(2), 0);
}

TEST(UnionFind, Same)
{
    const auto size = std::size_t{5};
    gcl::UnionFind uf(size);

    // 初期状態
    EXPECT_EQ(uf.same(0, 0), true);
    EXPECT_EQ(uf.same(0, 1), false);
    EXPECT_EQ(uf.same(3, 4), false);

    // unite 後
    uf.unite(0, 1); // {0,1}
    EXPECT_EQ(uf.same(0, 1), true);
    EXPECT_EQ(uf.same(1, 0), true);
    EXPECT_EQ(uf.same(0, 2), false);

    uf.unite(2, 3); // {2,3}
    EXPECT_EQ(uf.same(2, 3), true);
    EXPECT_EQ(uf.same(1, 2), false);

    // 推移的結合
    uf.unite(1, 2); // {0,1,2,3}
    EXPECT_EQ(uf.same(0, 3), true);
    EXPECT_EQ(uf.same(3, 0), true);

    // 孤立点
    EXPECT_EQ(uf.same(0, 4), false);
    EXPECT_EQ(uf.same(4, 4), true);
}