//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/PageRankBasedSampler.h"

#include <algorithm>
#include <cmath>

using namespace std;

PageRankBasedSampler::PageRankBasedSampler(std::size_t number_of_nodes, double alpha, int seed)
    : NodeSampler(number_of_nodes, seed), alpha(alpha) {}

vector<pair<size_t, size_t>> PageRankBasedSampler::sample(const DirectedGraph &g) {
    const auto &degrees = g.get_degrees();
    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto PR    = vector<double>(g.number_of_nodes(), 0);
    auto newPR = decltype(PR)(g.number_of_nodes(), 1);
    auto diff  = [](decltype(PR) &p, decltype(PR) &q) -> double {
        double d = 0;
        for (size_t i = 0; i < p.size(); i++) {
            d += fabs(p[i] - q[i]);
        }
        return d;
    };
    while (diff(PR, newPR) > 1e-6) {
        PR.assign(newPR.begin(), newPR.end());
        newPR.assign(g.number_of_nodes(), 1 - alpha);
#pragma omp parallel for
        for (size_t i = 0; i < g.number_of_nodes(); ++i) {
            auto left = offsets[i], right = offsets[i + 1];
#pragma omp parallel for
            for (size_t loc = left; loc < right; ++loc) {
                auto j = columns[loc];
                newPR[j] += PR[i] / static_cast<double>(degrees[i]) * alpha;
            }
        }
    }
    return get_sampled_nodes(g, newPR);
}

vector<pair<size_t, size_t>> PageRankBasedSampler::sample(const UndirectedGraph &g) {
    const auto &degrees = g.get_degrees();
    const auto &offsets = g.get_offsets();
    const auto &columns = g.get_columns();

    auto PR    = vector<double>(g.number_of_nodes(), 0);
    auto newPR = decltype(PR)(g.number_of_nodes(), 1);
    auto diff  = [](decltype(PR) &p, decltype(PR) &q) -> double {
        double d = 0;
        for (size_t i = 0; i < p.size(); i++) {
            d += fabs(p[i] - q[i]);
        }
        return d;
    };
    while (diff(PR, newPR) > 1e-6) {
        PR.assign(newPR.begin(), newPR.end());
        newPR.assign(g.number_of_nodes(), 1 - alpha);
#pragma omp parallel for
        for (size_t i = 0; i < g.number_of_nodes(); ++i) {
            auto left = offsets[i], right = offsets[i + 1];
#pragma omp parallel for
            for (size_t loc = left; loc < right; ++loc) {
                auto j = columns[loc];
                newPR[j] += PR[i] / static_cast<double>(degrees[i]) * alpha;
                newPR[i] += PR[j] / static_cast<double>(degrees[j]) * alpha;
            }
        }
    }
    return get_sampled_nodes(g, newPR);
}
