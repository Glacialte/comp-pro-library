#include <gtest/gtest.h>
#include "../algorithm/shortest_path.hpp"

using ull = std::uint64_t;

TEST(Dijkstra, SimpleLine)
{
    gcl::Graph<ull> g(4);
    g[0].push_back({1, 5});
    g[1].push_back({2, 6});
    g[2].push_back({3, 9});

    auto dist = gcl::dijkstra_dist(g, 0);
    ASSERT_EQ(dist.size(), std::size_t{4});
    EXPECT_EQ(dist[0], 0LL);
    EXPECT_EQ(dist[1], 5LL);
    EXPECT_EQ(dist[2], 11LL);
    EXPECT_EQ(dist[3], 20LL);
}

TEST(Dijkstra, TwoPathExists)
{
    gcl::Graph<ull> g(3);
    g[0].push_back({1, 5});
    g[0].push_back({2, 15});
    g[1].push_back({2, 7});

    auto dist = gcl::dijkstra_dist(g, 0);
    EXPECT_EQ(dist[0], 0LL);
    EXPECT_EQ(dist[1], 5LL);
    EXPECT_EQ(dist[2], 12LL);
}

TEST(Dijkstra, CycleExists)
{
    gcl::Graph<ull> g(4);
    g[0].push_back({1, 2});
    g[1].push_back({2, 3});
    g[1].push_back({3, 1});
    g[2].push_back({0, 1});
    g[2].push_back({3, 4});
    g[3].push_back({2, 1});

    auto dist = gcl::dijkstra_dist(g, 0);
    EXPECT_EQ(dist[0], 0LL);
    EXPECT_EQ(dist[1], 2LL);
    EXPECT_EQ(dist[2], 4LL);
    EXPECT_EQ(dist[3], 3LL);
}

TEST(Dijkstra, Unreachable)
{
    constexpr ull INF = std::numeric_limits<ull>::max();
    gcl::Graph<ull> g(4);
    g[0].push_back({1, 3});

    auto dist = gcl::dijkstra_dist(g, 0);
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 3);
    EXPECT_EQ(dist[2], INF);
    EXPECT_EQ(dist[3], INF);
}