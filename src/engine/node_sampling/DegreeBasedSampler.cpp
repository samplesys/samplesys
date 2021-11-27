//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/DegreeBasedSampler.h"

using namespace std;

DegreeBasedSampler::DegreeBasedSampler(std::size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}

vector<pair<size_t, size_t>> DegreeBasedSampler::sample(const DirectedGraph &g) {
    return get_sampled_nodes(g, vector<double>(g.get_degrees().begin(), g.get_degrees().end()));
}

vector<pair<size_t, size_t>> DegreeBasedSampler::sample(const UndirectedGraph &g) {
    return get_sampled_nodes(g, vector<double>(g.get_degrees().begin(), g.get_degrees().end()));
}
