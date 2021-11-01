//
// Created by AutJ on 2021/10/31.
//

#ifndef SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_

#include "NodeSampler.h"
#include "graph/Graph.h"

class DegreeBasedSampler : public NodeSampler {
 public:
  explicit DegreeBasedSampler(std::size_t number_of_nodes, int seed = 10);
  Graph sample(const Graph &g) override;
};

#endif  // SAMPLER_INCLUDE_ENGINE_NODE_SAMPLING_DEGREEBASEDSAMPLER_H_
