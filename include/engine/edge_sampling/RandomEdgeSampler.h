//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_

#include <engine/BaseSampler.h>

class RandomEdgeSampler : public BaseSampler {
   private:
    std::size_t number_of_edges;

   public:
    explicit RandomEdgeSampler(std::size_t number_of_edges, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
