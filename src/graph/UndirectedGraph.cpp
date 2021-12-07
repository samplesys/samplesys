//
// Created by AutJ on 2021/11/27.
//

#include "graph/UndirectedGraph.h"

using namespace std;

UndirectedGraph::UndirectedGraph(map<size_t, set<size_t>> &adjList, size_t number_of_nodes) {
    size_t number_of_edges = 0;

    auto copy = vector<set<size_t>>(number_of_nodes);
    for (auto &&p : adjList) {
        for (auto &&j : p.second) {
            ++number_of_edges;
            copy[p.first].insert(j);
            if (j != p.first) copy[j].insert(p.first);
        }
    }

    edges.reserve(number_of_edges * 2);
    columns.reserve(number_of_edges * 2);
    offsets.assign(number_of_nodes + 1, 0);
    degrees.assign(number_of_nodes, 0);

    for (size_t i = 0; i < number_of_nodes; i++) {
        const auto &neighbour = copy[i];
        offsets[i + 1]        = offsets[i] + neighbour.size();
        degrees[i]            = neighbour.size();
        for (auto &&j : neighbour) {
            edges.push_back(1);
            columns.push_back(j);
        }
    }
}