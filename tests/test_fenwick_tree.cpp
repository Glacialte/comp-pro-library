#include <gtest/gtest.h>
#include "../data-structure/fenwick_tree.hpp"

TEST(FenwickTree, DefaultConstructor)
{
    const auto size = std::size_t{3};
    EXPECT_NO_THROW(gcl::FenwickTree<int> ft(size));
}

TEST(FenwickTree, ConstructByVector)
{
    std::vector<int> v = {1, 2, 4};
    EXPECT_NO_THROW(gcl::FenwickTree<int> ft(v));
}

TEST(FenwickTree, Get)
{
    std::vector<int> v = {1, 2, 4};
    gcl::FenwickTree<int> ft(v);
    EXPECT_EQ(ft.get(0), 1);
    EXPECT_EQ(ft.get(1), 2);
    EXPECT_EQ(ft.get(2), 4);
}

TEST(FenwickTree, Add)
{
    const auto size = std::size_t{3};
    std::vector<int> v(size);
    gcl::FenwickTree<int> ft(v);
    EXPECT_NO_THROW(ft.add(0, 1));
    EXPECT_NO_THROW(ft.add(1, 2));
    EXPECT_NO_THROW(ft.add(2, 4));
    EXPECT_EQ(ft.get(0), 1);
    EXPECT_EQ(ft.get(1), 2);
    EXPECT_EQ(ft.get(2), 4);
}

TEST(FenwickTree, AddTwiceSameIndex)
{
    gcl::FenwickTree<int> ft(3);
    ft.add(1, 2);
    ft.add(1, 5);
    EXPECT_EQ(ft.get(1), 7);
    EXPECT_EQ(ft.sum(0, 3), 7);
}

TEST(FenwickTree, ConstructAndAdd)
{
    std::vector<int> v = {1, 2, 4};
    gcl::FenwickTree<int> ft(v);
    ft.add(0, 10);
    EXPECT_EQ(ft.get(0), 11);
    EXPECT_EQ(ft.sum(0, 3), 17);
}

TEST(FenwickTree, Sum)
{
    std::vector<int> v = {1, 2, 4};
    gcl::FenwickTree<int> ft(v);
    EXPECT_EQ(ft.sum(0, 0), 0);
    EXPECT_EQ(ft.sum(0, 1), 1);
    EXPECT_EQ(ft.sum(0, 2), 3);
    EXPECT_EQ(ft.sum(0, 3), 7);
    EXPECT_EQ(ft.sum(1, 1), 0);
    EXPECT_EQ(ft.sum(1, 2), 2);
    EXPECT_EQ(ft.sum(1, 3), 6);
    EXPECT_EQ(ft.sum(2, 2), 0);
    EXPECT_EQ(ft.sum(2, 3), 4);
}

TEST(FenwickTree, Size)
{
    const auto size = std::size_t{100};
    std::vector<int> v(size);
    gcl::FenwickTree<int> ft(v);
    EXPECT_EQ(ft.size(), size);
}
