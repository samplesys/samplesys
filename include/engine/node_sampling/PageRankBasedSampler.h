//
// Created by AutJ on 2021/10/31.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_

#include "NodeSampler.h"
#include "graph/DirectedGraph.h"

class PageRankBasedSampler : public NodeSampler {
   private:
    double alpha;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit PageRankBasedSampler(std::size_t number_of_nodes, double alpha = 0.85, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const Graph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_PAGERANKBASEDSAMPLER_H_
