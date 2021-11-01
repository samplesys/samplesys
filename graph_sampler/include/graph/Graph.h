//
// Created by AutJ on 2021/10/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_GRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_GRAPH_H_

#include <map>
#include <set>
#include <vector>

/**
 * directed weighed graph
 */
class Graph {
 private:
  std::set<std::size_t> nodes;
  std::map<std::size_t, std::map<std::size_t, int>> adj_list;

 public:
  [[nodiscard]] const std::set<std::size_t> &get_nodes() const;
  [[nodiscard]] std::set<std::pair<std::size_t, std::size_t>> get_edges() const;
  [[nodiscard]] std::size_t get_degree(std::size_t node) const;
  [[nodiscard]] std::set<std::size_t> get_neighbour(std::size_t node) const;
  void add_node(std::size_t node_for_adding);
  void add_nodes_from(const std::vector<std::size_t> &nodes_for_adding);
  void add_edge(std::size_t u_of_edge, std::size_t v_of_edge, int weight);
  void add_edges_from(
      const std::vector<std::tuple<std::size_t, std::size_t, int>>
          &edges_for_adding);
  [[nodiscard]] std::size_t number_of_nodes() const;
  [[nodiscard]] Graph node_subgraph(
      const std::set<std::size_t> &sub_nodes) const;
  [[nodiscard]] Graph node_subgraph(
      const std::vector<std::size_t> &sub_nodes) const;
  [[nodiscard]] Graph edge_subgraph(
      const std::set<std::pair<std::size_t, std::size_t>> &sub_edges) const;
  [[nodiscard]] Graph edge_subgraph(
      const std::vector<std::pair<std::size_t, std::size_t>> &sub_edges) const;
};

#endif  // SAMPLER_INCLUDE_GRAPH_GRAPH_H_
