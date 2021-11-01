//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/PageRankBasedSampler.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

using namespace std;

PageRankBasedSampler::PageRankBasedSampler(std::size_t number_of_nodes,
                                           double alpha, int seed)
    : NodeSampler(number_of_nodes, seed), alpha(alpha) {}

Graph PageRankBasedSampler::sample(const Graph &g) {
  auto degrees = vector<double>();
  degrees.reserve(g.number_of_nodes());
  auto nodes = vector<size_t>(g.get_nodes().begin(), g.get_nodes().end());
  for (const auto &node : nodes) {
    degrees.push_back(static_cast<double>(g.get_degree(node)));
  }
  auto neighbours = map<size_t, set<size_t>>();
  for (const auto &node : g.get_nodes()) {
    neighbours[node] = g.get_neighbour(node);
  }

  auto PR = vector<double>();
  PR.assign(g.number_of_nodes(), 1);
  // TODO: O(n^2) -> O(n*log(n))
  while (true) {
    auto newPR = vector<double>();
    newPR.assign(g.number_of_nodes(), 1 - alpha);
    for (size_t i = 0; i < g.number_of_nodes(); i++) {
      const auto &neighbour = neighbours[nodes[i]];
      for (const auto &node : neighbour) {
        const auto &index =
            find(nodes.begin(), nodes.end(), node) - nodes.begin();
        newPR[i] += alpha * PR[index] / degrees[index];
      }
    }
    double diff = 0;
    for (size_t i = 0; i < g.number_of_nodes(); i++) {
      diff += fabs(PR[i] - newPR[i]);
    }
    if (diff < 1e-6) {
      break;
    }
    PR = move(newPR);
  }

  auto sampled_nodes = set<size_t>();
  mt19937 generator(seed);
  for (size_t i = 0; i < number_of_nodes; i++) {
    discrete_distribution<size_t> distribution(PR.begin(), PR.end());
    auto index = distribution(generator);
    sampled_nodes.insert(nodes[index]);
    PR[index] = 0;
  }
  return g.node_subgraph(sampled_nodes);
}
