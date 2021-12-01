//
// Created by AutJ on 2021/11/27.
//

#include "graph/Graph.h"

#include <algorithm>

using namespace std;

const std::vector<std::size_t> &Graph::get_edges() const { return edges; }

const std::vector<std::size_t> &Graph::get_columns() const { return columns; }

const std::vector<std::size_t> &Graph::get_offsets() const { return offsets; }

const std::vector<std::size_t> &Graph::get_degrees() const { return degrees; }

decltype(Graph::columns) Graph::calc_neighbours(size_t i) const {
    return {columns.begin() + offsets[i], columns.begin() + offsets[i + 1]};
}

std::size_t Graph::number_of_nodes() const { return offsets.size() - 1; }

std::size_t Graph::number_of_edges() const { return columns.size(); }

size_t Graph::index_of_offsets(size_t loc) const {
    auto it = upper_bound(offsets.begin(), offsets.end(), loc);
    return it - offsets.begin() - 1;
}

size_t Graph::loc_of_edge_between(size_t i, size_t j) const {
    auto it = lower_bound(columns.begin() + offsets[i], columns.begin() + offsets[i + 1], j);
    if (it == columns.end()) {
        return -1;
    }
    return it - columns.begin();
}
