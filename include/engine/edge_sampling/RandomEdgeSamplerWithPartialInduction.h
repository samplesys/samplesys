#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_

#include "engine/BaseSampler.h"

class RandomEdgeSamplerWithPartialInduction : public BaseSampler {
   private:
    double p;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomEdgeSamplerWithPartialInduction(double p = 0.5, int seed = 10);
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHPARTIALINDUCTION_H_
