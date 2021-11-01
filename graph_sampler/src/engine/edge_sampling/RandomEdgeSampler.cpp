//
// Created by AutJ on 2021/11/1.
//

#include "engine/edge_sampling/RandomEdgeSampler.h"

#include <algorithm>
#include <iterator>
#include <random>

using namespace std;

RandomEdgeSampler::RandomEdgeSampler(size_t number_of_edges, int seed)
    : EdgeSampler(number_of_edges, seed) {}

Graph RandomEdgeSampler::sample(const Graph &g) {
  const auto &edges = g.get_edges();
  auto sampled_edges = set<pair<size_t, size_t>>();
  std::sample(edges.begin(), edges.end(),
              inserter(sampled_edges, sampled_edges.begin()), number_of_edges,
              mt19937(seed));
  return g.edge_subgraph(sampled_edges);
}