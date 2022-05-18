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
string samplingMethod;
double percent;
int    is_directed;
int    seed;

void cmdInp(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "[Usage]: " << argv[0] << " path/to/input"
                  << " path/to/output" << endl
                  << "[Options]:" << endl
                  << "\t-directed/undirected, default directed." << endl
                  << "\t-percent <num>\t\tPercent of graph to be sampled, default 0.1." << endl
                  << "\t-method <name>\t\tSample method(rn/rdn/prn/re/ff), default \"rn\"" << endl
                  << "\t-seed <num>\t\tRandom seed." << endl;
        exit(1);
    }
    int _argc      = 0;
    is_directed    = true;
    percent        = 0.5;
    samplingMethod = "rn";
    input          = argv[++_argc];
    output         = argv[++_argc];
    while (++_argc < argc) {
        if (strcmp(argv[_argc], "--directed") == 0)
            is_directed = true;
        else if (strcmp(argv[_argc], "--undirected") == 0)
            is_directed = false;
        else if (strcmp(argv[_argc], "--method") == 0)
            samplingMethod = argv[++_argc];
        else if (strcmp(argv[_argc], "--percent") == 0)
            percent = atof(argv[++_argc]);
        else if (strcmp(argv[_argc], "-seed") == 0)
            seed = atoi(argv[++_argc]);
    }
    if (input == "" || output == "") {
        std::cout << "input path and output path must be specified." << endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);

    auto graph = GraphStream::readText(input, is_directed);

    auto nodeSamplers = map<string, shared_ptr<BaseSampler>>{
        {"rn", make_shared<RandomNodeSampler>(graph->number_of_nodes() * percent, seed)},
        {"rdn", make_shared<DegreeBasedSampler>(graph->number_of_nodes() * percent, seed)},
        {"prn", make_shared<PageRankBasedSampler>(graph->number_of_nodes() * percent, seed)},
        {"re", make_shared<RandomEdgeSampler>(graph->number_of_edges() * percent, seed)},
        {"rne", make_shared<RandomNodeEdgeSampler>(graph->number_of_edges() * percent, seed)},
        {"hrne", make_shared<HybridNodeEdgeSampler>(graph->number_of_edges() * percent, seed)},
        {"ties",
         make_shared<RandomEdgeSamplerWithInduction>(graph->number_of_edges() * percent, seed)},
        {"pies", make_shared<RandomEdgeSamplerWithPartialInduction>(
                     graph->number_of_edges() * percent, seed)},
        {"bfs", make_shared<BreadthFirstSearchSampler>(graph->number_of_nodes() * percent, seed)},
        {"dfs", make_shared<DepthFirstSearchSampler>(graph->number_of_nodes() * percent, seed)},
        {"ff", make_shared<ForestFireSampler>(graph->number_of_nodes() * percent, seed)},
        {"snb", make_shared<SnowBallSampler>(graph->number_of_nodes() * percent, seed)},
        {"rw", make_shared<RandomWalkSampler>(graph->number_of_nodes() * percent, seed)},
        {"rwj", make_shared<RandomWalkWithJumpSampler>(graph->number_of_nodes() * percent, seed)},
        {"rwr",
         make_shared<RandomWalkWithRestartSampler>(graph->number_of_nodes() * percent, seed)}};

    auto start = high_resolution_clock::now();

    auto it = nodeSamplers.find(samplingMethod);
    if (it == nodeSamplers.end()) {
        cout << samplingMethod << " not found";
        exit(1);
    }
    auto sampler = it->second;
    auto edges   = sampler->sample(*graph);
    printGraph(edges, output);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by " << samplingMethod << ": " << duration.count() << " microseconds"
         << endl;

    return 0;
}