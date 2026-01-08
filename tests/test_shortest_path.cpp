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

TEST(BellmanFord, SimpleGraphNoNegativeCycle)
{
    using ll = long long;
    constexpr ll INF = std::numeric_limits<ll>::max();

    gcl::Graph<ll> g(5);
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

    gcl::Graph<ll> g(4);
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

    gcl::Graph<ll> g(5);
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

TEST(ShortestPathConcept, DefaultGraph)
{
    static_assert(gcl::WeightedGraph<gcl::Graph<std::uint64_t>>);
    static_assert(gcl::WeightedGraph<gcl::Graph<std::int64_t>>);
    static_assert(gcl::WeightedGraph<gcl::Graph<std::uint32_t>>);
    static_assert(gcl::WeightedGraph<gcl::Graph<std::int32_t>>);
    static_assert(gcl::Weight<std::uint64_t>);
    static_assert(gcl::Weight<std::int64_t>);
    static_assert(gcl::Weight<std::uint32_t>);
    static_assert(gcl::Weight<std::int32_t>);
    static_assert((gcl::WeightedEdge<gcl::Edge<std::uint64_t>, std::uint64_t>));
    static_assert((gcl::WeightedEdge<gcl::Edge<std::int64_t>, std::int64_t>));
    static_assert((gcl::WeightedEdge<gcl::Edge<std::uint32_t>, std::uint32_t>));
    static_assert((gcl::WeightedEdge<gcl::Edge<std::int32_t>, std::int32_t>));
}

TEST(ShortestPathConcept, EdgeHasToMember)
{
    struct EdgeWithoutTo
    {
        std::uint64_t weight;
    };
    using G1 = std::vector<std::vector<EdgeWithoutTo>>;
    struct Edge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    using G2 = std::vector<std::vector<Edge>>;
    static_assert(!gcl::WeightedGraph<G1>);
    static_assert(gcl::WeightedGraph<G2>);
}

TEST(ShortestPathConcept, EdgeHasWeightMember)
{
    struct EdgeWithoutWeight
    {
        std::size_t to;
    };
    using G1 = std::vector<std::vector<EdgeWithoutWeight>>;
    struct Edge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    using G2 = std::vector<std::vector<Edge>>;
    static_assert(!gcl::WeightedGraph<G1>);
    static_assert(gcl::WeightedGraph<G2>);
}

TEST(ShortestPathConcept, GraphIsRange)
{
    struct Edge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    struct WeirdGraph
    {
        std::vector<int> dummy;
        size_t size() const { return dummy.size(); };
        Edge operator[](size_t) { return Edge(); };
    };
    static_assert(!gcl::WeightedGraph<WeirdGraph>);
}
