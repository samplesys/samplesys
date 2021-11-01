//
// Created by AutJ on 2021/10/26.
//

#ifndef SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_

#include <graph/Graph.h>

class BaseSampler {
 protected:
  int seed;

 public:
  explicit BaseSampler(int seed);
  virtual Graph sample(const Graph &g) = 0;
};

#endif  // SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_
