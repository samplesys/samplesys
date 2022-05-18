#ifndef SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_
#define SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_

#include <fstream>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
#include "utils/Random.h"

class BaseSampler {
   protected:
    int    seed;
    Random random;

    virtual std::vector<std::pair<std::size_t, std::size_t>> _sample(const DirectedGraph *g)   = 0;
    virtual std::vector<std::pair<std::size_t, std::size_t>> _sample(const UndirectedGraph *g) = 0;

   public:
    explicit BaseSampler(int seed);
    virtual std::vector<std::pair<std::size_t, std::size_t>> sample(const Graph &g);
};

#endif  // SAMPLER_INCLUDE_ENGINE_BASESAMPLER_H_
