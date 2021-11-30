//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_
#define SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_

#include "EdgeSampler.h"

class RandomEdgeSamplerWithInduction : public EdgeSampler {
   public:
    explicit RandomEdgeSamplerWithInduction(std::size_t number_of_edges, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_
