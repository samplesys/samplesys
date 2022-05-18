/*
 * Created by AutJ on 2022-05-17.
 */

#ifndef SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_RANDOMWALKWITHRESTARTSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_RANDOMWALKWITHRESTARTSAMPLER_H_

#include "engine/BaseSampler.h"

class RandomWalkWithRestartSampler : public BaseSampler {
   private:
    std::size_t number_of_nodes;
    std::size_t start_node;
    double      p;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomWalkWithRestartSampler(std::size_t number_of_nodes, std::size_t start_node = -1,
                                          double p = 0.1, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const Graph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_EXPLORATION_SAMPLING_RANDOMWALKWITHRESTARTSAMPLER_H_
