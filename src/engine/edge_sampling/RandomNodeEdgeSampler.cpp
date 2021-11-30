//
// Created by AutJ on 2021/11/27.
//

#include "engine/edge_sampling/RandomNodeEdgeSampler.h"

using namespace std;

RandomNodeEdgeSampler::RandomNodeEdgeSampler(size_t number_of_edges, int seed)
    : EdgeSampler(number_of_edges, seed) {}

vector<pair<size_t, size_t>> RandomNodeEdgeSampler::sample(const DirectedGraph& g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g.get_columns();
    const auto& offsets = g.get_offsets();
    const auto& degrees = g.get_degrees();

    auto   edge_is_sampled = vector<bool>(g.number_of_edges());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        auto i = random.randint<size_t>(g.number_of_nodes());
        if (!degrees[i]) continue;
        auto loc = offsets[i] + random.randint<size_t>(degrees[i]);
        if (edge_is_sampled[loc]) continue;
        edge_is_sampled[loc] = true;
        ret.emplace_back(i, columns[loc]);
        ++current_sampled;
    }
    return ret;
}

vector<pair<size_t, size_t>> RandomNodeEdgeSampler::sample(const UndirectedGraph& g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g.get_columns();
    const auto& offsets = g.get_offsets();
    const auto& degrees = g.get_degrees();

    auto   edge_is_sampled = vector<bool>(g.number_of_edges());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        auto i = random.randint<size_t>(g.number_of_nodes());
        if (!degrees[i]) continue;
        auto loc = offsets[i] + random.randint<size_t>(degrees[i]);
        if (edge_is_sampled[loc]) continue;
        auto j                                       = columns[loc];
        edge_is_sampled[loc]                         = true;
        edge_is_sampled[g.loc_of_edge_between(j, i)] = true;
        if (i < j) {
            ret.emplace_back(i, j);
        } else {
            ret.emplace_back(j, i);
        }
        ++current_sampled;
    }
    return ret;
}