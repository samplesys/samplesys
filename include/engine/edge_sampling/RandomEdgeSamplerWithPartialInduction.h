//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_

#include "EdgeSampler.h"

class RandomEdgeSamplerWithPartialInduction : public BaseSampler {
   private:
    double p;

   public:
    explicit RandomEdgeSamplerWithPartialInduction(double p = 0.5, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_
