//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_BREADTHFIRSTSEARCHSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_BREADTHFIRSTSEARCHSAMPLER_H_

#include "engine/BaseSampler.h"

class BreadthFirstSearchSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t start_node;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit BreadthFirstSearchSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                                       int seed = 10);
};

#endif  // SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_BREADTHFIRSTSEARCHSAMPLER_H_
