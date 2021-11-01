//
// Created by AutJ on 2021/11/1.
//

#include "engine/edge_sampling/EdgeSampler.h"

using namespace std;

EdgeSampler::EdgeSampler(size_t number_of_edges, int seed)
    : BaseSampler(seed), number_of_edges(number_of_edges) {}
