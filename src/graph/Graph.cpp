//
// Created by AutJ on 2021/11/27.
//

#include "graph/Graph.h"

using namespace std;

const std::vector<std::size_t> &Graph::get_edges() const { return edges; }

const std::vector<std::size_t> &Graph::get_columns() const { return columns; }

const std::vector<std::size_t> &Graph::get_offsets() const { return offsets; }

const std::vector<std::size_t> &Graph::get_degrees() const { return degrees; }

std::size_t Graph::number_of_nodes() const { return offsets.size() - 1; }

std::size_t Graph::number_of_edges() const { return columns.size(); }