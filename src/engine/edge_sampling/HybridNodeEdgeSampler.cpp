//
// Created by AutJ on 2021/11/30.
//

#include "engine/edge_sampling/HybridNodeEdgeSampler.h"

using namespace std;

HybridNodeEdgeSampler::HybridNodeEdgeSampler(size_t number_of_edges, double p, int seed)
    : BaseSampler(seed), number_of_edges(number_of_edges), p(p) {}

vector<pair<size_t, size_t>> HybridNodeEdgeSampler::_sample(const DirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    auto   edge_is_sampled = vector<bool>(g->number_of_edges());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        if (random.uniform<double>() < p) {
            auto i = random.randint<size_t>(g->number_of_nodes());
            if (!degrees[i]) continue;
            auto loc = offsets[i] + random.randint<size_t>(degrees[i]);
            if (edge_is_sampled[loc]) continue;
            edge_is_sampled[loc] = true;
            ret.emplace_back(i, columns[loc]);
            ++current_sampled;
        } else {
            auto loc = random.randint<size_t>(g->number_of_edges());
            if (edge_is_sampled[loc]) continue;
            edge_is_sampled[loc] = true;
            ret.emplace_back(g->index_of_offsets(loc), columns[loc]);
        }
    }
    return ret;
}

vector<pair<size_t, size_t>> HybridNodeEdgeSampler::_sample(const UndirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    auto   edge_is_sampled = vector<bool>(g->number_of_edges());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        size_t i = 0, j = 0, loc = 0;
        if (random.uniform<double>() < p) {
            i = random.randint<size_t>(g->number_of_nodes());
            if (!degrees[i]) continue;
            loc = offsets[i] + random.randint<size_t>(degrees[i]);
            if (edge_is_sampled[loc]) continue;
            j = columns[loc];
        } else {
            loc = random.randint<size_t>(g->number_of_edges());
            if (edge_is_sampled[loc]) continue;
            i = g->index_of_offsets(loc);
            j = columns[loc];
        }
        edge_is_sampled[loc]                          = true;
        edge_is_sampled[g->loc_of_edge_between(j, i)] = true;
        if (i < j) {
            ret.emplace_back(i, j);
        } else {
            ret.emplace_back(j, i);
        }
        ++current_sampled;
    }
    return ret;
}