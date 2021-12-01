//
// Created by AutJ on 2021/11/30.
//

#ifndef SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_
#define SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_

#include <engine/BaseSampler.h>

class HybridNodeEdgeSampler : public BaseSampler {
   private:
    std::size_t number_of_edges;
    double      p;

   public:
    explicit HybridNodeEdgeSampler(std::size_t number_of_edges, double p = 0.8, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const DirectedGraph &g) override;
    std::vector<std::pair<std::size_t, std::size_t>> sample(const UndirectedGraph &g) override;
};

#endif  // SAMPLER_SRC_ENGINE_EDGE_SAMPLING_HYBRIDNODEEDGESAMPLER_H_
