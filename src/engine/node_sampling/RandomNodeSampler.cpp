//
// Created by AutJ on 2021/10/27.
//

#include "engine/node_sampling/RandomNodeSampler.h"

using namespace std;

RandomNodeSampler::RandomNodeSampler(size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}

vector<pair<size_t, size_t>> RandomNodeSampler::sample(const DirectedGraph &g) {
    return get_sampled_nodes(g, vector<double>(g.number_of_nodes(), 1));
}

vector<pair<size_t, size_t>> RandomNodeSampler::sample(const UndirectedGraph &g) {
    return get_sampled_nodes(g, vector<double>(g.number_of_nodes(), 1));
}
