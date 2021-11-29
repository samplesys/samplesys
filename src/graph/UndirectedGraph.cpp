//
// Created by AutJ on 2021/11/27.
//

#include "graph/UndirectedGraph.h"

using namespace std;

UndirectedGraph::UndirectedGraph(size_t number_of_nodes, map<size_t, set<size_t>> &adjList) {
    size_t number_of_edges = 0;

    auto copy = map<size_t, set<size_t>>();
    for (auto &&p : adjList) {
        for (auto &&j : p.second) {
            ++number_of_edges;
            if (j < p.first) {
                copy[j].insert(p.first);
            } else {
                copy[p.first].insert(j);
            }
        }
    }
    
    edges.reserve(number_of_edges);
    columns.reserve(number_of_edges);
    offsets.assign(number_of_nodes + 1, 0);
    degrees.assign(number_of_nodes, 0);

    for (size_t i = 0; i < number_of_nodes; i++) {
        const auto &neighbour = copy[i];
        offsets[i + 1]        = offsets[i] + neighbour.size();
        for (auto &&j : neighbour) {
            edges.push_back(1);
            columns.push_back(j);
            ++degrees[i];
            ++degrees[j];
        }
    }
}