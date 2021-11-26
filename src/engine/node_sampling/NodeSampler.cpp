//
// Created by AutJ on 2021/11/1.
//

#include "engine/node_sampling/NodeSampler.h"

#include <algorithm>

using namespace std;

NodeSampler::NodeSampler(size_t number_of_nodes, int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes) {}

vector<pair<size_t, size_t>> NodeSampler::get_sampled_nodes(
    const Graph &g, const vector<double> &probability) {
  auto ret = vector<pair<size_t, size_t>>();
  auto sampled_nodes = random.choice(probability, number_of_nodes, false);
  sort(sampled_nodes.begin(), sampled_nodes.end());

  const auto &offsets = g.get_offsets();
  const auto &columns = g.get_columns();

  auto nodes_is_sampled = vector<bool>(g.number_of_nodes());
  for (auto &&v: sampled_nodes) {
    nodes_is_sampled[v] = true;
  }
  for (size_t i = 0; i < g.number_of_nodes(); ++i) {
    if (!nodes_is_sampled[i]) continue;
    for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
      if (nodes_is_sampled[columns[loc]]) {
        ret.emplace_back(i, columns[loc]);
      }
    }
  }

  return ret;
}
