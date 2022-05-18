#ifndef SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_DEPTHFIRSTSEARCHSAMPLER_H_
#define SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_DEPTHFIRSTSEARCHSAMPLER_H_

#include "engine/BaseSampler.h"

class DepthFirstSearchSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t start_node;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit DepthFirstSearchSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                                     int seed = 10);
};

#endif  // SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_DEPTHFIRSTSEARCHSAMPLER_H_
