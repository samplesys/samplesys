//
// Created by AutJ on 2021/10/27.
//

#include "engine/node_sampling/RandomNodeSampler.h"

#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

RandomNodeSampler::RandomNodeSampler(size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}

Graph RandomNodeSampler::sample(const Graph &g) {
  const auto &nodes = g.get_nodes();
  auto sampled_nodes = set<size_t>();
  std::sample(nodes.begin(), nodes.end(),
              inserter(sampled_nodes, sampled_nodes.begin()), number_of_nodes,
              mt19937(seed));
  return g.node_subgraph(sampled_nodes);
}
