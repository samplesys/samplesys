//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_
#define SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_

#include "engine/BaseSampler.h"

class SnowBallSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t k;
    std::size_t start_node;

   public:
    explicit SnowBallSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                             std::size_t k = 50, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_SRC_ENGINE_EXPLORATION_SAMPLING_SNOWBALLSAMPLER_H_
