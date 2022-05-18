#include "engine/edge_sampling/RandomEdgeSamplerWithInduction.h"

using namespace std;

RandomEdgeSamplerWithInduction::RandomEdgeSamplerWithInduction(size_t number_of_edges, int seed)
    : BaseSampler(seed), number_of_edges(number_of_edges) {}

vector<pair<size_t, size_t>> RandomEdgeSamplerWithInduction::_sample(const DirectedGraph *g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &offsets = g->get_offsets();
    const auto &columns = g->get_columns();

    auto probability     = vector<double>(g->number_of_edges(), 1);
    auto sampled_edges   = random.choice(probability, number_of_edges, false);
    auto node_is_sampled = vector<bool>(g->number_of_nodes());
    for (auto loc : sampled_edges) {
        node_is_sampled[g->index_of_offsets(loc)] = true;
        node_is_sampled[columns[loc]]             = true;
    }
    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }
    return ret;
}

vector<pair<size_t, size_t>> RandomEdgeSamplerWithInduction::_sample(const UndirectedGraph *g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &offsets = g->get_offsets();
    const auto &columns = g->get_columns();

    auto   edge_is_sampled = vector<bool>(g->number_of_edges());
    auto   node_is_sampled = vector<bool>(g->number_of_nodes());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        auto loc = random.randint<size_t>(g->number_of_edges());
        if (edge_is_sampled[loc]) continue;
        auto i = g->index_of_offsets(loc), j = columns[loc];
        edge_is_sampled[loc]                          = true;
        edge_is_sampled[g->loc_of_edge_between(j, i)] = true;
        node_is_sampled[i]                            = true;
        node_is_sampled[j]                            = true;
        ++current_sampled;
    }
    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (i < offsets[loc] && node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }
    return ret;
}