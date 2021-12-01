//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_FORESTFIRESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_FORESTFIRESAMPLER_H_

#include "engine/BaseSampler.h"

class ForestFireSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t start_node;
    double      p;
    std::size_t max_visited_nodes;
    std::size_t restart_hop_size;

   public:
    explicit ForestFireSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                               double p = 0.4, std::size_t max_visited_nodes_backlog = 100,
                               std::size_t restart_hop_size = 10, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_FORESTFIRESAMPLER_H_
