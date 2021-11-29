//
// Created by AutJ on 2021/10/31.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_

#include "graph/DirectedGraph.h"
#include "NodeSampler.h"

class DegreeBasedSampler : public NodeSampler {
   public:
    explicit DegreeBasedSampler(std::size_t number_of_nodes, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_
