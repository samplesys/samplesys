//
// Created by AutJ on 2021/10/26.
//

#ifndef SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_

#include <graph/Graph.h>
#include <utils/Random.h>

#include <fstream>

class BaseSampler {
 protected:
  int seed;
  Random random;

 public:
  explicit BaseSampler(int seed);
  virtual std::vector<std::pair<std::size_t, std::size_t>> sample(
      const Graph &g) = 0;
};

#endif  // SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_