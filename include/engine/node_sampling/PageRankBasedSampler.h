//
// Created by AutJ on 2021/10/31.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_

#include "graph/DirectedGraph.h"
#include "NodeSampler.h"

class PageRankBasedSampler : public NodeSampler {
   private:
    double alpha;

   public:
    explicit PageRankBasedSampler(std::size_t number_of_nodes, double alpha = 0.85, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_
