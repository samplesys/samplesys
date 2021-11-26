//
// Created by AutJ on 2021/10/31.
//

#include <engine/edge_sampling/RandomEdgeSampler.h>
#include <engine/node_sampling/DegreeBasedSampler.h>
#include <engine/node_sampling/PageRankBasedSampler.h>
#include <engine/node_sampling/RandomNodeSampler.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>

#include "utils.h"
using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "[usage]: " << argv[0] << " path/to/input"
         << " rn/rdn/prn/re"
         << " path/to/output" << endl;
    exit(1);
  }
  size_t number_of_nodes;
  double percent = 0.1;
  map<std::size_t, set<std::size_t>> adjList;
  readGraph(number_of_nodes, adjList, argv[1]);
  auto graph = Graph(number_of_nodes, adjList);
  auto nodeSamplers = map<string, shared_ptr<BaseSampler>>{
      {"rn", make_shared<RandomNodeSampler>(graph.number_of_nodes() * percent)},
      {"rdn",
       make_shared<DegreeBasedSampler>(graph.number_of_nodes() * percent)},
      {"prn",
       make_shared<PageRankBasedSampler>(graph.number_of_nodes() * percent)},
      {"re", make_shared<RandomEdgeSampler>(graph.number_of_edges() * percent)},
  };
  string method = argv[3];

  auto start = high_resolution_clock::now();

  auto it = nodeSamplers.find(method);
  if (it == nodeSamplers.end()) {
    cout << argv[3] << " not found";
    exit(1);
  }
  auto sampler = it->second;
  auto edges = sampler->sample(graph);
  printGraph(edges, argv[2]);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Time taken by " << method << ": " << duration.count()
       << " microseconds" << endl;

  return 0;
}