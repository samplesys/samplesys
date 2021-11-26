//
// Created by AutJ on 2021/10/27.
//

#include "engine/node_sampling/RandomNodeSampler.h"

using namespace std;

RandomNodeSampler::RandomNodeSampler(size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}

vector<pair<size_t, size_t>> RandomNodeSampler::sample(const Graph &g) {
  vector<double> p(g.number_of_nodes(), 1);
  return get_sampled_nodes(g, p);
}
