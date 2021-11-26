//
// Created by AutJ on 2021/10/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_GRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_GRAPH_H_

#include <map>
#include <set>
#include <vector>

/**
 * directed graph
 */
class Graph {
 private:
  std::vector<std::size_t> edges;
  std::vector<std::size_t> columns;
  std::vector<std::size_t> offsets;
  std::vector<std::size_t> degrees;

 public:
  Graph(std::size_t number_of_nodes,
        std::map<std::size_t, std::set<std::size_t>> adjList);
  [[nodiscard]] const decltype(edges) &get_edges() const;
  [[nodiscard]] const decltype(columns) &get_columns() const;
  [[nodiscard]] const decltype(offsets) &get_offsets() const;
  [[nodiscard]] const decltype(degrees) &get_degrees() const;
  [[nodiscard]] std::size_t number_of_nodes() const;
  [[nodiscard]] std::size_t number_of_edges() const;
};

#endif  // SAMPLER_INCLUDE_GRAPH_GRAPH_H_
