#include <vector>
#include <cstddef>
#include <utility>
#include <concepts>
#include <type_traits>

namespace gcl
{
    // 重みなし辺
    struct Edge
    {
        std::size_t to{};
        Edge() = default;
        constexpr Edge(std::size_t to_) : to(to_) {}
    };

    // 重みなしグラフ
    using Graph = std::vector<std::vector<Edge>>;

    // 重み付き辺
    template <typename W>
    struct WEdge
    {
        std::size_t to{};
        W weight{};

        WEdge() = default;

        constexpr WEdge(std::size_t to_, W weight_) : to(to_), weight(std::move(weight_)) {}

        template <class T, class We>
            requires std::convertible_to<std::remove_reference_t<T>, std::size_t> && std::convertible_to<We, W>
        constexpr WEdge(T &&to_, We &&weight_) : to(static_cast<std::size_t>(to_)), weight(static_cast<W>(std::forward<We>(weight_)))
        {
        }
    };

    // 重み付きグラフ
    template <typename W>
    using WGraph = std::vector<std::vector<WEdge<W>>>;

    // 重み付き無向辺
    template <class W>
    struct UWEdge
    {
        std::size_t from{}, to{};
        W weight{};

        UWEdge() = default;

        constexpr UWEdge(std::size_t from_, std::size_t to_, W weight_) : from(from_), to(to_), weight(std::move(weight_)) {}

        template <class From, class To, class We>
            requires std::convertible_to<std::remove_reference_t<From>, std::size_t> &&
                         std::convertible_to<std::remove_reference_t<To>, std::size_t> &&
                         std::convertible_to<We, W>
        constexpr UWEdge(From &&from_, To &&to_, We &&weight_) : from(static_cast<std::size_t>(from_)), to(static_cast<std::size_t>(to_)),
                                                                 weight(static_cast<W>(std::forward<We>(weight_)))
        {
        }
    };
}