//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_
#define SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_

#include "engine/BaseSampler.h"

class SnowBallSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t start_node;
    std::size_t k;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit SnowBallSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                             std::size_t k = 50, int seed = 10);
};

#endif  // SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_
