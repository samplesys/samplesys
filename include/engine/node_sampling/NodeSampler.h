#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_

#include "engine/BaseSampler.h"

class NodeSampler : public BaseSampler {
   protected:
    std::size_t number_of_nodes;

   public:
    explicit NodeSampler(std::size_t number_of_nodes, int seed);
    virtual std::vector<std::pair<std::size_t, std::size_t>> get_sampled_nodes(
        const DirectedGraph &g, const std::vector<double> &probability);
    virtual std::vector<std::pair<std::size_t, std::size_t>> get_sampled_nodes(
        const UndirectedGraph &g, const std::vector<double> &probability);
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_
