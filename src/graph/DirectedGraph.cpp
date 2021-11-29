//
// Created by AutJ on 2021/10/27.
//

#include "graph/DirectedGraph.h"

using namespace std;

DirectedGraph::DirectedGraph(size_t number_of_nodes, map<size_t, set<size_t>> &adjList) {
    offsets.assign(number_of_nodes + 1, 0);
    degrees.assign(number_of_nodes, 0);
    for (size_t i = 0; i < number_of_nodes; i++) {
        const auto &neighbour = adjList[i];
        offsets[i + 1]        = offsets[i] + neighbour.size();
        degrees[i]            = neighbour.size();
        for (auto &&j : neighbour) {
            edges.push_back(1);
            columns.push_back(j);
        }
    }
    edges.shrink_to_fit();
    columns.shrink_to_fit();
}