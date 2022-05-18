#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_

#include "engine/BaseSampler.h"

class RandomEdgeSampler : public BaseSampler {
   private:
    std::size_t number_of_edges;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomEdgeSampler(std::size_t number_of_edges, int seed = 10);
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
