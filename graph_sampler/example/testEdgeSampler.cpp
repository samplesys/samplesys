//
// Created by AutJ on 2021/11/1.
//

#include <engine/edge_sampling/RandomEdgeSampler.h>

#include "utils.h"

using namespace std;

void testRandomEdgeSampler(const Graph &graph) {
  auto sampler = RandomEdgeSampler(10);
  auto subgraph = sampler.sample(graph);
  printGraph(subgraph, "output/testRandomEdgeSampler.txt");
}

int main() {
  auto graph = Graph();
  readGraph(graph, "input/graph.1.txt");

  testRandomEdgeSampler(graph);

  return 0;
}