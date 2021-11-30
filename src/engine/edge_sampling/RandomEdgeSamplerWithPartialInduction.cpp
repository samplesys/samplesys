//
// Created by AutJ on 2021/12/1.
//

#include "engine/edge_sampling/RandomEdgeSamplerWithPartialInduction.h"

using namespace std;

RandomEdgeSamplerWithPartialInduction::RandomEdgeSamplerWithPartialInduction(double p, int seed)
    : BaseSampler(seed), p(p) {}

vector<pair<size_t, size_t>> RandomEdgeSamplerWithPartialInduction::sample(const DirectedGraph &g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto i_from_loc = vector<size_t>(g.number_of_edges());
    {
        size_t i = 0;
        for (size_t loc = 0; loc < g.number_of_edges(); ++loc) {
            while (offsets[i + 1] <= loc) {
                ++i;
            }
            i_from_loc[loc] = i;
        }
    }
    auto _edge_stream = std::vector<size_t>(g.number_of_edges());
    iota(_edge_stream.begin(), _edge_stream.end(), 0);
    random.shuffle(_edge_stream);
    auto edge_is_sampled = vector<bool>(g.number_of_edges());
    auto node_is_sampled = vector<bool>(g.number_of_nodes());
    for (auto &&loc : _edge_stream) {
        if (edge_is_sampled[loc]) continue;
        auto i = i_from_loc[loc], j = columns[loc];
        if (node_is_sampled[i] && node_is_sampled[j]) {
            edge_is_sampled[loc] = true;
            ret.emplace_back(i, j);
        } else if (random.uniform<>() < p) {
            node_is_sampled[i]   = true;
            node_is_sampled[j]   = true;
            edge_is_sampled[loc] = true;
            ret.emplace_back(i, j);
        }
    }
    return ret;
}

vector<pair<size_t, size_t>> RandomEdgeSamplerWithPartialInduction::sample(
    const UndirectedGraph &g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto i_from_loc = vector<size_t>(g.number_of_edges());
    {
        size_t i = 0;
        for (size_t loc = 0; loc < g.number_of_edges(); ++loc) {
            while (offsets[i + 1] <= loc) {
                ++i;
            }
            i_from_loc[loc] = i;
        }
    }
    auto _edge_stream = std::vector<size_t>(g.number_of_edges());
    iota(_edge_stream.begin(), _edge_stream.end(), 0);
    random.shuffle(_edge_stream);
    auto edge_is_sampled = vector<bool>(g.number_of_edges());
    auto node_is_sampled = vector<bool>(g.number_of_nodes());
    for (auto &&loc : _edge_stream) {
        if (edge_is_sampled[loc]) continue;
        auto i = i_from_loc[loc], j = columns[loc];
        if (node_is_sampled[i] && node_is_sampled[j]) {
            edge_is_sampled[loc]                         = true;
            edge_is_sampled[g.loc_of_edge_between(j, i)] = true;
            if (i < j) {
                ret.emplace_back(i, j);
            } else {
                ret.emplace_back(j, i);
            }
        } else if (random.uniform<>() < p) {
            node_is_sampled[i]                           = true;
            node_is_sampled[j]                           = true;
            edge_is_sampled[loc]                         = true;
            edge_is_sampled[g.loc_of_edge_between(j, i)] = true;
            if (i < j) {
                ret.emplace_back(i, j);
            } else {
                ret.emplace_back(j, i);
            }
        }
    }
    return ret;
}
