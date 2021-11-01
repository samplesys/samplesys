//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_EDGESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_EDGESAMPLER_H_

#include <engine/BaseSampler.h>

#include <cstddef>

class EdgeSampler : public BaseSampler {
 protected:
  std::size_t number_of_edges;

 public:
  explicit EdgeSampler(std::size_t number_of_edges, int seed);
};

#endif  // SAMPLER_INCLUDE_ENGINE_EDGE_SAMPLING_EDGESAMPLER_H_
