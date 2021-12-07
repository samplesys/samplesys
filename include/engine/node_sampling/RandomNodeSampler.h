//
// Created by AutJ on 2021/10/27.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_RANDOMNODESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_RANDOMNODESAMPLER_H_

#include "NodeSampler.h"

class RandomNodeSampler : public NodeSampler {
   private:
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomNodeSampler(std::size_t number_of_nodes, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const Graph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_RANDOMNODESAMPLER_H_
