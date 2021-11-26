//
// Created by AutJ on 2021/10/31.
//

#include "engine/node_sampling/PageRankBasedSampler.h"

#include <algorithm>
#include <cmath>

using namespace std;

PageRankBasedSampler::PageRankBasedSampler(std::size_t number_of_nodes,
                                           double alpha, int seed)
    : NodeSampler(number_of_nodes, seed), alpha(alpha) {}

vector<pair<size_t, size_t>> PageRankBasedSampler::sample(const Graph &g) {
  const auto &degrees = g.get_degrees();
  const auto &offsets = g.get_offsets();
  const auto &columns = g.get_columns();

  auto PR = vector<double>(g.number_of_nodes(), 1);
  while (true) {
    auto newPR = vector<double>(g.number_of_nodes(), 1 - alpha);

    for (size_t i = 0; i < g.number_of_nodes(); ++i) {
      auto left = offsets[i], right = offsets[i + 1];
      for (size_t loc = left; loc < right; ++loc) {
        auto j = columns[loc];
        newPR[j] += PR[i] / static_cast<double>(degrees[i]) * alpha;
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
  return get_sampled_nodes(g, PR);
}
