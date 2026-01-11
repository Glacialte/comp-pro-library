#include <concepts>
#include <limits>
#include <type_traits>
#include <ranges>

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
            cont[i];
            requires std::ranges::range<decltype(cont[i])>;
            requires WeightedEdge<edge_t<C>, weight_t<C>>;
        };
}