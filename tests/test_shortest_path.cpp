#include <gtest/gtest.h>
#include "../algorithm/shortest_path.hpp"
#include "../data-structure/graph.hpp"

using ull = std::uint64_t;

TEST(Dijkstra, SimpleLine)
{
    gcl::WGraph<ull> g(4);
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
    gcl::WGraph<ull> g(3);
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
    gcl::WGraph<ull> g(4);
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
    gcl::WGraph<ull> g(4);
    g[0].push_back({1, 3});

    auto dist = gcl::dijkstra_dist(g, 0);
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 3);
    EXPECT_EQ(dist[2], INF);
    EXPECT_EQ(dist[3], INF);
}

TEST(BellmanFord, SimpleGraphNoNegativeCycle)
{
    using ll = long long;
    constexpr ll INF = std::numeric_limits<ll>::max();

    gcl::WGraph<ll> g(5);
    g[0].push_back({1, 4});
    g[1].push_back({2, -2});
    g[2].push_back({3, 3});
    g[0].push_back({2, 10});

    auto res = gcl::bellman_ford(g, 0);
    ASSERT_EQ(res.dist.size(), std::size_t{5});
    EXPECT_FALSE(res.negative_cycle_exist);

    EXPECT_EQ(res.dist[0], 0);
    EXPECT_EQ(res.dist[1], 4);
    EXPECT_EQ(res.dist[2], 2);   // 0->1->2
    EXPECT_EQ(res.dist[3], 5);   // 0->1->2->3
    EXPECT_EQ(res.dist[4], INF); // 到達不能
}

TEST(BellmanFord, ReachableNegativeCycle)
{
    using ll = long long;

    gcl::WGraph<ll> g(4);
    // 1<->2 で負閉路
    g[0].push_back({1, 1});
    g[1].push_back({2, 1});
    g[2].push_back({1, -3});
    g[2].push_back({3, 0});

    auto res = gcl::bellman_ford(g, 0);
    EXPECT_TRUE(res.negative_cycle_exist);
}

TEST(BellmanFord, NegativeCycleUnreachable)
{
    using ll = long long;
    constexpr ll INF = std::numeric_limits<ll>::max();

    gcl::WGraph<ll> g(5);
    // start=0 から到達できる部分（負閉路なし）
    g[0].push_back({1, 2});
    g[1].push_back({2, 2});

    // 3<->4 に負閉路を作るが、0 からは到達不能
    g[3].push_back({4, 1});
    g[4].push_back({3, -3});

    auto res = gcl::bellman_ford(g, 0);
    EXPECT_FALSE(res.negative_cycle_exist);

    ASSERT_EQ(res.dist.size(), std::size_t{5});
    EXPECT_EQ(res.dist[0], 0);
    EXPECT_EQ(res.dist[1], 2);
    EXPECT_EQ(res.dist[2], 4);
    EXPECT_EQ(res.dist[3], INF);
    EXPECT_EQ(res.dist[4], INF);
}
