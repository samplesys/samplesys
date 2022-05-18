#include "engine/node_sampling/PageRankBasedSampler.h"

#include <algorithm>
#include <cmath>

using namespace std;

PageRankBasedSampler::PageRankBasedSampler(std::size_t number_of_nodes, double alpha, int seed)
    : NodeSampler(number_of_nodes, seed), alpha(alpha) {}

vector<pair<size_t, size_t>> PageRankBasedSampler::_sample(const DirectedGraph *g) {
    const auto &degrees = g->get_degrees();
    const auto &offsets = g->get_offsets();
    const auto &columns = g->get_columns();

    auto PR    = vector<double>(g->number_of_nodes(), 0);
    auto newPR = decltype(PR)(g->number_of_nodes(), 1);
    auto diff  = [](decltype(PR) &p, decltype(PR) &q) -> double {
        double d = 0;
        for (size_t i = 0; i < p.size(); i++) {
            d += fabs(p[i] - q[i]);
        }
        return d;
    };
    while (diff(PR, newPR) / static_cast<double>(g->number_of_nodes()) > 1e-6) {
        PR.assign(newPR.begin(), newPR.end());
        newPR.assign(g->number_of_nodes(), 0);
#pragma omp parallel for
        for (size_t i = 0; i < g->number_of_nodes(); ++i) {
            if (offsets[i] == offsets[i + 1]) {
#pragma omp parallel for
                for (int j = 0; j < g->number_of_nodes(); j++) {
                    newPR[j] += PR[i] / static_cast<double>(g->number_of_nodes() - 1);
                }
                newPR[i] -= PR[i] / static_cast<double>(g->number_of_nodes() - 1);
            } else {
#pragma omp parallel for
                for (size_t loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
                    newPR[columns[loc]] += PR[i] / static_cast<double>(degrees[i]);
                }
            }
        }
#pragma omp parallel for
        for (auto &item : newPR) {
            item = item * alpha + (1 - alpha);
        }
    }
    return get_sampled_nodes(*g, newPR);
}

vector<pair<size_t, size_t>> PageRankBasedSampler::_sample(const UndirectedGraph *g) {
    const auto &degrees = g->get_degrees();
    const auto &offsets = g->get_offsets();
    const auto &columns = g->get_columns();

    auto PR    = vector<double>(g->number_of_nodes(), 0);
    auto newPR = decltype(PR)(g->number_of_nodes(), 1);
    auto diff  = [](decltype(PR) &p, decltype(PR) &q) -> double {
        double d = 0;
        for (size_t i = 0; i < p.size(); i++) {
            d += fabs(p[i] - q[i]);
        }
        return d;
    };
    while (diff(PR, newPR) / static_cast<double>(g->number_of_nodes()) > 1e-6) {
        PR.assign(newPR.begin(), newPR.end());
        newPR.assign(g->number_of_nodes(), 0);
#pragma omp parallel for
        for (size_t i = 0; i < g->number_of_nodes(); ++i) {
#pragma omp parallel for
            for (size_t loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
                newPR[columns[loc]] += PR[i] / static_cast<double>(degrees[i]);
            }
        }
#pragma omp parallel for
        for (auto &item : newPR) {
            item = item * alpha + (1 - alpha);
        }
    }
    return get_sampled_nodes(*g, newPR);
}