//
// Created by AutJ on 2021/11/1.
//

#include "engine/edge_sampling/RandomEdgeSampler.h"

#include <algorithm>
#include <iterator>

using namespace std;

RandomEdgeSampler::RandomEdgeSampler(size_t number_of_edges, int seed)
    : BaseSampler(seed), number_of_edges(number_of_edges) {}

vector<pair<size_t, size_t>> RandomEdgeSampler::_sample(const DirectedGraph *g) {
    auto ret           = vector<pair<size_t, size_t>>();
    auto probability   = vector<double>(g->number_of_edges(), 1);
    auto sampled_edges = random.choice(probability, number_of_edges, false);

    const auto &offsets = g->get_offsets();
    const auto &columns = g->get_columns();

    size_t i = 0;
    for (auto &&loc : sampled_edges) {
        while (loc >= offsets[i + 1]) {
            ++i;
        }
        ret.emplace_back(i, columns[loc]);
    }

    return ret;
}

vector<pair<size_t, size_t>> RandomEdgeSampler::_sample(const UndirectedGraph *g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &columns = g->get_columns();

    auto   edge_is_sampled = vector<bool>(g->number_of_edges());
    size_t current_sampled = 0;
    while (current_sampled < number_of_edges) {
        auto loc = random.randint<size_t>(g->number_of_edges());
        if (edge_is_sampled[loc]) continue;
        auto i = g->index_of_offsets(loc), j = columns[loc];
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