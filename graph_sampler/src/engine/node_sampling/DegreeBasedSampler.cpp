//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/DegreeBasedSampler.h"

#include <random>

using namespace std;

DegreeBasedSampler::DegreeBasedSampler(std::size_t number_of_nodes, int seed)
    : NodeSampler(number_of_nodes, seed) {}

Graph DegreeBasedSampler::sample(const Graph &g) {
  auto nodes = vector<size_t>(g.get_nodes().begin(), g.get_nodes().end());
  auto sampled_nodes = set<size_t>();
  auto degrees = vector<double>();
  degrees.reserve(g.number_of_nodes());
  for (const auto &node : nodes) {
    degrees.push_back(static_cast<double>(g.get_degree(node)));
  }
  mt19937 generator(seed);
  for (size_t i = 0; i < number_of_nodes; i++) {
    discrete_distribution<size_t> distribution(degrees.begin(), degrees.end());
    auto index = distribution(generator);
    sampled_nodes.insert(nodes[index]);
    degrees[index] = 0;
  }
  return g.node_subgraph(sampled_nodes);
}
