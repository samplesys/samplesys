//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_

#include "EdgeSampler.h"

class RandomEdgeSampler : public EdgeSampler {
 public:
  RandomEdgeSampler(std::size_t number_of_edges, int seed = 10);
  Graph sample(const Graph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_RANDOMEDGESAMPLER_H_
