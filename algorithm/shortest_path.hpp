#include <vector>
#include <ranges>
#include <concepts>
#include <queue>
#include <limits>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace gcl
{
    template <typename W>
    concept Weight =
        std::totally_ordered<W> &&
        std::default_initializable<W> &&
        std::copyable<W> &&
        requires(W a, W b) {
            { a + b } -> std::convertible_to<W>;
            { a += b } -> std::same_as<W &>;
            requires std::numeric_limits<W>::is_specialized;
            { std::numeric_limits<W>::max() } -> std::convertible_to<W>;
        };

    template <typename E, typename W>
    concept WeightedEdge =
        Weight<W> && requires(const E &e) {
            { e.to } -> std::convertible_to<std::size_t>;
            { e.weight } -> std::convertible_to<W>;
        };

    template <class C>
    using adj_range_t = decltype(std::declval<const C &>()[std::declval<std::size_t>()]);

    template <class C>
    using edge_t = std::ranges::range_value_t<adj_range_t<C>>;

    template <class C>
    using weight_t = std::remove_cvref_t<decltype(std::declval<const edge_t<C> &>().weight)>;

    template <typename C>
    concept WeightedGraph =
        Weight<weight_t<C>> &&
        std::ranges::sized_range<C> &&
        requires(const C &cont, std::size_t i) {
            requires WeightedEdge<edge_t<C>, weight_t<C>>;
        };

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

    template <class W>
    struct ShortestPathResult
    {
        std::size_t start;
        std::vector<W> dist;
        std::vector<std::size_t> parent;

        ShortestPathResult(std::size_t start_,
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
    ShortestPathResult<weight_t<C>> dijkstra_path(const C &graph, std::size_t start)
    {
        WithParent pp(std::ranges::size(graph));
        auto dist = dijkstra_impl(graph, start, pp);
        return {start, std::move(dist), std::move(pp.parent)};
    }

    template <typename W>
        requires Weight<W>
    struct Edge
    {
        std::size_t to;
        W weight;
    };

    template <typename W>
    using Graph = std::vector<std::vector<Edge<W>>>;
}