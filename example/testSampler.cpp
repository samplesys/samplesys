#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>

#include "engine/edge_sampling/HybridNodeEdgeSampler.h"
#include "engine/edge_sampling/RandomEdgeSampler.h"
#include "engine/edge_sampling/RandomEdgeSamplerWithInduction.h"
#include "engine/edge_sampling/RandomEdgeSamplerWithPartialInduction.h"
#include "engine/edge_sampling/RandomNodeEdgeSampler.h"
#include "engine/exploration_sampling/BreadthFirstSearchSampler.h"
#include "engine/exploration_sampling/DepthFirstSearchSampler.h"
#include "engine/exploration_sampling/ForestFireSampler.h"
#include "engine/exploration_sampling/RandomWalkSampler.h"
#include "engine/exploration_sampling/RandomWalkWithJumpSampler.h"
#include "engine/exploration_sampling/RandomWalkWithRestartSampler.h"
#include "engine/exploration_sampling/SnowBallSampler.h"
#include "engine/node_sampling/DegreeBasedSampler.h"
#include "engine/node_sampling/PageRankBasedSampler.h"
#include "engine/node_sampling/RandomNodeSampler.h"
#include "utils.h"
#include "utils/GraphStream.h"

using namespace std;
using namespace std::chrono;

string input;
string output;
int    is_directed = true;
double percent     = 0.1;
string sampling_method;

void cmdInp(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "[Usage]: " << argv[0] << " path/to/input"
                  << " path/to/output" << endl
                  << "[Options]:" << endl
                  << "\t--directed/undirected, default directed." << endl
                  << "\t--percent <num>\t\tPercent of graph to be sampled, default 0.1." << endl
                  << "\t--method <name>\t\tSample method(rn/rdn/prn/re/ff), default \"rn\"" << endl;
        exit(1);
    }
    int _argc       = 0;
    input           = argv[++_argc];
    output          = argv[++_argc];
    sampling_method = "rn";

    while (++_argc < argc) {
        if (strcmp(argv[_argc], "--directed") == 0)
            is_directed = true;
        else if (strcmp(argv[_argc], "--undirected") == 0)
            is_directed = false;
        else if (strcmp(argv[_argc], "--method") == 0)
            sampling_method = argv[++_argc];
        else if (strcmp(argv[_argc], "--percent") == 0)
            percent = atof(argv[++_argc]);
    }
    if (input.empty() || output.empty()) {
        std::cout << "input path and output path must be specified." << endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);

    auto graph = GraphStream::readText(input, is_directed);

    auto nodeSamplers = map<string, shared_ptr<BaseSampler>>{
        {"rn", make_shared<RandomNodeSampler>(graph->number_of_nodes() * percent)},
        {"rdn", make_shared<DegreeBasedSampler>(graph->number_of_nodes() * percent)},
        {"prn", make_shared<PageRankBasedSampler>(graph->number_of_nodes() * percent)},
        {"re", make_shared<RandomEdgeSampler>(graph->number_of_edges() * percent)},
        {"rne", make_shared<RandomNodeEdgeSampler>(graph->number_of_edges() * percent)},
        {"hrne", make_shared<HybridNodeEdgeSampler>(graph->number_of_edges() * percent)},
        {"ties",
         make_shared<RandomEdgeSamplerWithInduction>(graph->number_of_edges() * percent)},
        {"pies", make_shared<RandomEdgeSamplerWithPartialInduction>()},
        {"bfs", make_shared<BreadthFirstSearchSampler>(graph->number_of_nodes() * percent)},
        {"dfs", make_shared<DepthFirstSearchSampler>(graph->number_of_nodes() * percent)},
        {"ff", make_shared<ForestFireSampler>(graph->number_of_nodes() * percent)},
        {"snb", make_shared<SnowBallSampler>(graph->number_of_nodes() * percent)},
        {"rw", make_shared<RandomWalkSampler>(graph->number_of_nodes() * percent)},
        {"rwj", make_shared<RandomWalkWithJumpSampler>(graph->number_of_nodes() * percent)},
        {"rwr",
         make_shared<RandomWalkWithRestartSampler>(graph->number_of_nodes() * percent)}};

    auto start = high_resolution_clock::now();

    auto it = nodeSamplers.find(sampling_method);
    if (it == nodeSamplers.end()) {
        cout << sampling_method << " not found";
        exit(1);
    }
    auto sampler = it->second;
    auto edges   = sampler->sample(*graph);
    printGraph(edges, output);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by " << sampling_method << ": " << duration.count() << " microseconds"
         << endl;

    return 0;
}