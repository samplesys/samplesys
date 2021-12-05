//
// Created by AutJ on 2021/11/1.
//

#include "engine/node_sampling/NodeSampler.h"

#include <algorithm>

using namespace std;

NodeSampler::NodeSampler(size_t number_of_nodes, int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes) {}

vector<pair<size_t, size_t>> NodeSampler::get_sampled_nodes(const DirectedGraph  &g,
                                                            const vector<double> &probability) {
    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto ret = vector<pair<size_t, size_t>>();

    auto sampled_nodes = random.choice(probability, number_of_nodes, false);

    auto node_is_sampled = vector<bool>(g.number_of_nodes());
    for (auto &&v : sampled_nodes) {
        node_is_sampled[v] = true;
    }
    for (size_t i = 0; i < g.number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }
    return ret;
}

vector<pair<size_t, size_t>> NodeSampler::get_sampled_nodes(const UndirectedGraph &g,
                                                            const vector<double>  &probability) {
    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto ret = vector<pair<size_t, size_t>>();

    auto sampled_nodes = random.choice(probability, number_of_nodes, false);

    auto node_is_sampled = vector<bool>(g.number_of_nodes());
    for (auto &&v : sampled_nodes) {
        node_is_sampled[v] = true;
    }
    for (size_t i = 0; i < g.number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (i < offsets[loc] && node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }
    return ret;
}