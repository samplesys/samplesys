//
// Created by AutJ on 2021/11/1.
//

#include "engine/edge_sampling/EdgeSampler.h"

#include <algorithm>

using namespace std;

EdgeSampler::EdgeSampler(size_t number_of_edges, int seed)
    : BaseSampler(seed), number_of_edges(number_of_edges) {}

std::vector<std::pair<std::size_t, std::size_t>> EdgeSampler::get_sampled_edges(
    const Graph &g, const vector<double> &probability) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto sampled_edges = random.choice(probability, number_of_edges, false);
    sort(sampled_edges.begin(), sampled_edges.end());

    size_t i = 0;
    for (auto &&loc : sampled_edges) {
        while (loc >= offsets[i + 1]) {
            ++i;
        }
        ret.emplace_back(i, columns[loc]);
    }

    return ret;
}