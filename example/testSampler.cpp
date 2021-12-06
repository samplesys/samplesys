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
    if (argc < 4) {
        cout << "[usage]: " << argv[0]
             << " path/to/input"
             // << "directed/undirected"
             << " path/to/output"
             << " rn/rdn/prn/re" << endl;
        exit(1);
    }
    double percent = 0.1;
    int    _argc   = 0;
    string input   = argv[++_argc];
    // string graphType      = argv[++_argc];
    string output         = argv[++_argc];
    string samplingMethod = argv[++_argc];

    size_t number_of_nodes;
    auto   adjList = map<std::size_t, set<std::size_t>>();
    readGraph(number_of_nodes, adjList, input);

    auto graph = UndirectedGraph(number_of_nodes, adjList);

    auto nodeSamplers = map<string, shared_ptr<BaseSampler>>{
        {"rn", make_shared<RandomNodeSampler>(graph.number_of_nodes() * percent)},
        {"rdn", make_shared<DegreeBasedSampler>(graph.number_of_nodes() * percent)},
        {"prn", make_shared<PageRankBasedSampler>(graph.number_of_nodes() * percent)},
        {"re", make_shared<RandomEdgeSampler>(graph.number_of_edges() * percent)},
    };

    auto start = high_resolution_clock::now();

    auto it = nodeSamplers.find(samplingMethod);
    if (it == nodeSamplers.end()) {
        cout << samplingMethod << " not found";
        exit(1);
    }
    auto sampler = it->second;
    auto edges   = sampler->sample(graph);
    printGraph(edges, output);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by " << samplingMethod << ": " << duration.count() << " microseconds"
         << endl;

    return 0;
}