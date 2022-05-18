#ifndef SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_
#define SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_

#include "engine/BaseSampler.h"

class HybridNodeEdgeSampler : public BaseSampler {
   private:
    std::size_t number_of_edges;
    double      p;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit HybridNodeEdgeSampler(std::size_t number_of_edges, double p = 0.8, int seed = 10);
};

#endif  // SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_
