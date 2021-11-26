//
// Created by AutJ on 2021/11/1.
//

#include "engine/edge_sampling/RandomEdgeSampler.h"

#include <algorithm>
#include <iterator>

using namespace std;

RandomEdgeSampler::RandomEdgeSampler(size_t number_of_edges, int seed)
    : EdgeSampler(number_of_edges, seed) {}

vector<pair<size_t, size_t>> RandomEdgeSampler::sample(const Graph &g) {
  vector<double> p(g.number_of_edges(), 1);
  return get_sampled_edges(g, p);
}