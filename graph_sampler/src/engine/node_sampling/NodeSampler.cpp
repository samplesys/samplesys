//
// Created by AutJ on 2021/11/1.
//

#include "engine/node_sampling/NodeSampler.h"

using namespace std;

NodeSampler::NodeSampler(size_t number_of_nodes, int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes) {}
