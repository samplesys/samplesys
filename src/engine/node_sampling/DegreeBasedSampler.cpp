//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/DegreeBasedSampler.h"

using namespace std;

DegreeBasedSampler::DegreeBasedSampler(std::size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}
vector<pair<size_t, size_t>> DegreeBasedSampler::sample(const Graph &g) {
  vector<double> p(g.get_degrees().begin(), g.get_degrees().end());
  return get_sampled_nodes(g, p);
}
