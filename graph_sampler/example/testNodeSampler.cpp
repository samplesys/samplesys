//
// Created by AutJ on 2021/10/31.
//

#include <engine/node_sampling/DegreeBasedSampler.h>
#include <engine/node_sampling/PageRankBasedSampler.h>
#include <engine/node_sampling/RandomNodeSampler.h>

#include "utils.h"

using namespace std;

void testRandomNodeSampler(const Graph &graph) {
  auto sampler = RandomNodeSampler(10);
  auto subgraph = sampler.sample(graph);
  printGraph(subgraph, "output/testRandomNodeSampler.txt");
}

void testDegreeBasedNodeSampler(const Graph &graph) {
  auto sampler = DegreeBasedSampler(10);
  auto subgraph = sampler.sample(graph);
  printGraph(subgraph, "output/testDegreeBasedSampler.txt");
}

void testPageRankBasedSampler(const Graph &graph) {
  auto sampler = PageRankBasedSampler(10);
  auto subgraph = sampler.sample(graph);
  printGraph(subgraph, "output/testPageRankBasedSampler.txt");
}

int main() {
  auto graph = Graph();
  readGraph(graph, "input/graph.1.txt");

  testRandomNodeSampler(graph);
  testDegreeBasedNodeSampler(graph);
  testPageRankBasedSampler(graph);

  return 0;
}