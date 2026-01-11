#include <gtest/gtest.h>
#include "../data-structure/graph.hpp"
#include "../concepts/graph_concepts.hpp"

TEST(GraphConcept, WGraph)
{
    static_assert(gcl::WeightedGraph<gcl::WGraph<std::uint64_t>>);
    static_assert(gcl::WeightedGraph<gcl::WGraph<std::int64_t>>);
    static_assert(gcl::WeightedGraph<gcl::WGraph<std::uint32_t>>);
    static_assert(gcl::WeightedGraph<gcl::WGraph<std::int32_t>>);
    static_assert(gcl::Weight<std::uint64_t>);
    static_assert(gcl::Weight<std::int64_t>);
    static_assert(gcl::Weight<std::uint32_t>);
    static_assert(gcl::Weight<std::int32_t>);
    static_assert((gcl::WeightedEdge<gcl::WEdge<std::uint64_t>, std::uint64_t>));
    static_assert((gcl::WeightedEdge<gcl::WEdge<std::int64_t>, std::int64_t>));
    static_assert((gcl::WeightedEdge<gcl::WEdge<std::uint32_t>, std::uint32_t>));
    static_assert((gcl::WeightedEdge<gcl::WEdge<std::int32_t>, std::int32_t>));
}

TEST(GraphConcept, EdgeHasToMember)
{
    struct EdgeWithoutTo
    {
        std::uint64_t weight;
    };
    using G1 = std::vector<std::vector<EdgeWithoutTo>>;
    struct WEdge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    using G2 = std::vector<std::vector<WEdge>>;
    static_assert(!gcl::WeightedGraph<G1>);
    static_assert(gcl::WeightedGraph<G2>);
}

TEST(GraphConcept, WEdgeHasWeightMember)
{
    struct EdgeWithoutWeight
    {
        std::size_t to;
    };
    using G1 = std::vector<std::vector<EdgeWithoutWeight>>;
    struct WEdge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    using G2 = std::vector<std::vector<WEdge>>;
    static_assert(!gcl::WeightedGraph<G1>);
    static_assert(gcl::WeightedGraph<G2>);
}

TEST(GraphConcept, GraphIsRange)
{
    struct WEdge
    {
        std::size_t to;
        std::uint64_t weight;
    };
    struct WeirdGraph
    {
        std::vector<int> dummy;
        size_t size() const { return dummy.size(); };
        WEdge operator[](size_t) { return WEdge(); };
    };
    static_assert(!gcl::WeightedGraph<WeirdGraph>);
}