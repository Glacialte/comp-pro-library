#include <vector>
#include <ranges>
#include <concepts>
#include <queue>
#include <limits>
#include <utility>
#include <type_traits>

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

    template <typename C>
        requires WeightedGraph<C>
    std::vector<weight_t<C>> dijkstra(const C &graph, std::size_t start)
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
                    pq.push({dist[e.to], e.to});
            }
        }
        return dist;
    }
}