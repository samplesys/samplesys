//
// Created by AutJ on 2021/10/27.
//

#include "graph/Graph.h"

using namespace std;

Graph::Graph(size_t number_of_nodes, map<size_t, set<size_t>> adjList) {
  offsets.assign(number_of_nodes + 1, 0);
  for (size_t i = 0; i < number_of_nodes; i++) {
    const auto &neighbour = adjList[i];
    offsets[i + 1] = offsets[i] + neighbour.size();
    degrees.push_back(neighbour.size());
    for (auto &&j : neighbour) {
      edges.push_back(1);
      columns.push_back(j);
    }
  }
}

const std::vector<std::size_t> &Graph::get_edges() const { return edges; }

const std::vector<std::size_t> &Graph::get_columns() const { return columns; }

const std::vector<std::size_t> &Graph::get_offsets() const { return offsets; }

const std::vector<std::size_t> &Graph::get_degrees() const { return degrees; }

std::size_t Graph::number_of_nodes() const { return offsets.size() - 1; }

std::size_t Graph::number_of_edges() const { return columns.size(); }
