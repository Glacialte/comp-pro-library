#include <vector>
#include <algorithm>
#include <ranges>
#include "../data-structure/graph.hpp"
#include "../data-structure/unionfind.hpp"
#include "../concepts/graph_concepts.hpp"

namespace gcl
{
    // `WeightedGraph<W>` を `std::vector<UWEdge<weight_t<G>>>`に変換するためのヘルパー
    template <WeightedGraph G>
    auto edges_from_undirected_adj(const G &g)
        -> std::vector<UWEdge<weight_t<G>>>
    {
        using W = weight_t<G>;
        std::vector<UWEdge<W>> edges;
        const std::size_t n = std::ranges::size(g);
        for (std::size_t u = 0; u < n; u++)
        {
            for (const auto &e : g[u])
            {
                const std::size_t v = static_cast<std::size_t>(e.to);
                if (u < v)
                    edges.emplace_back(u, v, e.weight);
            }
        }
        return edges;
    }

    template <class W>
    struct KruskalResult
    {
        W total_weight{};
        std::vector<UWEdge<W>> edges;
        bool is_connected = false;

        // 元グラフの頂点数を渡してMSTを返す。連結でない場合はMSFを返す。
        auto build_mst(std::size_t n_vertices) const -> WGraph<W>
        {
            WGraph<W> mst(n_vertices);
            for (const auto &e : edges)
            {
                mst[e.from].emplace_back(e.to, e.weight);
                mst[e.to].emplace_back(e.from, e.weight);
            }
            return mst;
        }
    };

    // Kruskal法によって最小全域木を返す
    template <class W>
    auto kruskal(std::size_t n_vertices, std::vector<UWEdge<W>> edges)
        -> KruskalResult<W>
    {
        KruskalResult<W> result;
        std::sort(edges.begin(), edges.end(),
                  [](const auto &a, const auto &b)
                  {
                      if (a.weight != b.weight)
                          return a.weight < b.weight;
                      if (a.from != b.from)
                          return a.from < b.from;
                      return a.to < b.to;
                  });

        UnionFind uf(n_vertices);
        for (auto &edge : edges)
        {
            if (uf.same(edge.from, edge.to))
                continue;
            uf.unite(edge.from, edge.to);

            result.total_weight += edge.weight;
            result.edges.push_back(std::move(edge));
            if (result.edges.size() + 1 == n_vertices)
                break;
        }

        result.is_connected = (n_vertices == 0) ? true : (result.edges.size() == n_vertices - 1);
        return result;
    }

    // Kruskal法によって最小全域木を返す。`graph`は無向グラフであり、 `u->v` と `v->u` の両方が含まれることを前提とする。
    template <class W>
    auto kruskal(const WGraph<W> &graph)
        -> KruskalResult<W>
    {
        return kruskal(graph.size(), edges_from_undirected_adj(graph));
    }
}
