//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_

#include <cstddef>

#include "engine/BaseSampler.h"

class NodeSampler : public BaseSampler {
 protected:
  std::size_t number_of_nodes;

 public:
  explicit NodeSampler(std::size_t number_of_nodes, int seed);
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_NODESAMPLER_H_