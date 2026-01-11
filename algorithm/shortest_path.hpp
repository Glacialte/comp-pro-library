#include <vector>
#include <ranges>
#include <queue>
#include <utility>
#include <algorithm>
#include <functional>
#include "../concepts/graph_concepts.hpp"

namespace gcl
{
    template <typename T, typename U>
        requires std::totally_ordered_with<T, U> && std::assignable_from<T &, U>
    bool chmin(T &a, U &&b)
    {
        if (a > b)
        {
            a = std::forward<U>(b);
            return true;
        }
        return false;
    }

    inline constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

    template <typename W>
    struct DijkstraResult
    {
        std::size_t start;
        std::vector<W> dist;
        std::vector<std::size_t> parent;

        DijkstraResult(std::size_t start_,
                       std::vector<W> &&dist_,
                       std::vector<std::size_t> &&parent_) : start(start_), dist(std::move(dist_)), parent(std::move(parent_)) {}

        std::vector<std::size_t> restore_path(std::size_t goal) const
        {
            if (goal >= parent.size())
                return {};
            std::vector<std::size_t>
                path;
            for (auto v = goal; v != npos; v = parent[v])
            {
                path.push_back(v);
                if (v == start)
                    break;
            }
            if (path.empty() || path.back() != start)
                return {};
            std::reverse(path.begin(), path.end());
            return path;
        }
    };

    struct NoParent
    {
        void relax(std::size_t /*to*/, std::size_t /*from*/) noexcept {}
    };

    struct WithParent
    {
        std::vector<std::size_t> parent;
        explicit WithParent(std::size_t n) : parent(n, npos) {}
        void relax(std::size_t to, std::size_t from) noexcept { parent[to] = from; }
    };

    template <typename C, typename ParentPolicy>
        requires WeightedGraph<C>
    std::vector<weight_t<C>> dijkstra_impl(const C &graph, std::size_t start, ParentPolicy &pp)
    {
        using W = weight_t<C>;
        using Pair = std::pair<W, std::size_t>;
        constexpr W INF = std::numeric_limits<W>::max();

        std::vector<W> dist(std::ranges::size(graph), INF);
        std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;
        dist[start] = W{};
        pq.push({W{}, start});
        while (!pq.empty())
        {
            auto [d, v] = pq.top();
            pq.pop();

            if (dist[v] < d)
                continue;
            for (auto &&e : graph[v])
            {
                W nd = d + static_cast<W>(e.weight);
                if (chmin(dist[e.to], nd))
                {
                    pp.relax(e.to, v);
                    pq.push({nd, e.to});
                }
            }
        }
        return dist;
    }

    // Dijkstra: edge weights must be non-negative, return only distance
    template <typename C>
        requires WeightedGraph<C>
    std::vector<weight_t<C>> dijkstra_dist(const C &graph, std::size_t start)
    {
        NoParent pp;
        return dijkstra_impl(graph, start, pp);
    }

    // Dijkstra: edge weights must be non-negative, return distance and parent
    template <typename C>
        requires WeightedGraph<C>
    DijkstraResult<weight_t<C>> dijkstra_path(const C &graph, std::size_t start)
    {
        WithParent pp(std::ranges::size(graph));
        auto dist = dijkstra_impl(graph, start, pp);
        return {start, std::move(dist), std::move(pp.parent)};
    }

    template <typename W>
    struct BellmanFordResult
    {
        std::vector<W> dist;
        bool negative_cycle_exist;
    };

    template <typename C>
        requires WeightedGraph<C>
    BellmanFordResult<weight_t<C>> bellman_ford(const C &graph, std::size_t start)
    {
        using W = weight_t<C>;
        constexpr W INF = std::numeric_limits<W>::max();
        const auto N = std::ranges::size(graph);
        std::vector<W> dist(N, INF);

        bool negative_cycle_exist = false;
        dist[start] = W{};
        for (auto iter = std::size_t{0}; iter < N; iter++)
        {
            bool updated = false;
            for (auto i = std::size_t{0}; i < N; i++)
            {
                if (dist[i] == INF)
                    continue;
                for (const auto &edge : graph[i])
                {
                    updated |= chmin(dist[edge.to], dist[i] + edge.weight);
                }
            }
            if (!updated)
                break;
            if (iter == N - 1 && updated)
                negative_cycle_exist = true;
        }
        return BellmanFordResult{std::move(dist), negative_cycle_exist};
    }
}