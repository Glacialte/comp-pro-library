#include <gtest/gtest.h>
#include "../algorithm/minimum_spanning_tree.hpp"

TEST(MinimumSpanningTree, KruskalEdgeListConnected)
{
    using W = long long;

    // 0--1(1), 1--2(2), 0--2(3) なので MST は (0-1, 1-2), 重み 3
    std::vector<gcl::UWEdge<W>> edges{
        {0, 1, 1},
        {1, 2, 2},
        {0, 2, 3},
    };

    const auto res = gcl::kruskal<W>(3, edges);

    EXPECT_TRUE(res.is_connected);
    EXPECT_EQ(res.edges.size(), 2u);
    EXPECT_EQ(res.total_weight, 3);

    const auto mst = res.build_mst(3);
    EXPECT_EQ(mst.size(), 3u);

    // MSTは無向隣接リスト（両方向に追加）なので次数の合計は 2*(N-1)=4
    std::size_t deg_sum = 0;
    for (const auto &adj : mst)
        deg_sum += adj.size();
    EXPECT_EQ(deg_sum, 4u);
}

TEST(MinimumSpanningTree, KruskalWGraphConnected)
{
    using W = long long;

    gcl::WGraph<W> g(4);
    // 無向グラフを構築するためのヘルパー
    auto add_undirected = [&](std::size_t u, std::size_t v, W w)
    {
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    };

    // 0-1(1), 1-2(2), 2-3(3), 0-3(10), 0-2(5)
    // MST: 0-1, 1-2, 2-3 => 1+2+3 = 6
    add_undirected(0, 1, 1);
    add_undirected(1, 2, 2);
    add_undirected(2, 3, 3);
    add_undirected(0, 3, 10);
    add_undirected(0, 2, 5);

    const auto res = gcl::kruskal(g);

    EXPECT_TRUE(res.is_connected);
    EXPECT_EQ(res.edges.size(), 3u);
    EXPECT_EQ(res.total_weight, 6);

    const auto mst = res.build_mst(4);
    std::size_t deg_sum = 0;
    for (const auto &adj : mst)
        deg_sum += adj.size();
    EXPECT_EQ(deg_sum, 6u); // 2*(4-1)
}

TEST(MinimumSpanningTree, KruskalDisconnectedGivesForest)
{
    using W = long long;

    gcl::WGraph<W> g(4);
    auto add_undirected = [&](std::size_t u, std::size_t v, W w)
    {
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    };

    // 成分 {0,1} と {2,3}
    add_undirected(0, 1, 7);
    add_undirected(2, 3, 4);

    const auto res = gcl::kruskal(g);

    EXPECT_FALSE(res.is_connected);
    EXPECT_EQ(res.edges.size(), 2u);
    EXPECT_EQ(res.total_weight, 11);

    const auto forest = res.build_mst(4);
    std::size_t deg_sum = 0;
    for (const auto &adj : forest)
        deg_sum += adj.size();
    EXPECT_EQ(deg_sum, 4u); // 2*2本
}
