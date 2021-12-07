//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_
#define SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_

#include "engine/BaseSampler.h"

class RandomEdgeSamplerWithInduction : public BaseSampler {
   private:
    std::size_t number_of_edges;

    std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g);
    std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g);

   public:
    explicit RandomEdgeSamplerWithInduction(std::size_t number_of_edges, int seed = 10);
    std::vector<std::pair<std::size_t, std::size_t>> sample(const Graph &g) override;
};

#endif  // SAMPLER_SRC_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLERWITHINDUCTION_H_
