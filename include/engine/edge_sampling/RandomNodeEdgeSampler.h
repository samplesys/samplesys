//
// Created by AutJ on 2021/11/27.
//

#ifndef SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMNODEEDGESAMPLER_H_
#define SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMNODEEDGESAMPLER_H_

#include "engine/BaseSampler.h"

class RandomNodeEdgeSampler : public BaseSampler {
   private:
    std::size_t number_of_edges;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomNodeEdgeSampler(std::size_t number_of_edges, int seed = 10);
};

#endif  // SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMNODEEDGESAMPLER_H_
