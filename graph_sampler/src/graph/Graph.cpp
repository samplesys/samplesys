//
// Created by AutJ on 2021/10/27.
//

#include "graph/Graph.h"

#include <algorithm>

using namespace std;

const set<size_t> &Graph::get_nodes() const { return nodes; }

set<pair<size_t, size_t>> Graph::get_edges() const {
  auto edges = set<pair<size_t, size_t>>();
  for (const auto &p1 : adj_list) {
    const auto &u = p1.first;
    for (const auto &p2 : p1.second) {
      const auto &v = p2.first;
      edges.insert(make_pair(u, v));
    }
  }
  return edges;
}

size_t Graph::get_degree(size_t node) const {
  if (nodes.find(node) == nodes.end()) {
    return 0;
  }
  return adj_list.find(node)->second.size();
}

set<size_t> Graph::get_neighbour(size_t node) const {
  auto ret = set<size_t>();
  if (nodes.find(node) != nodes.end()) {
    for (const auto &p : adj_list.find(node)->second) {
      ret.insert(p.first);
    }
  }
  return ret;
}

void Graph::add_node(size_t node_for_adding) {
  if (nodes.find(node_for_adding) == nodes.end()) {
    nodes.insert(node_for_adding);
    adj_list[node_for_adding] = map<size_t, int>();
  } else {
    // node existed
  }
}

void Graph::add_nodes_from(const std::vector<std::size_t> &nodes_for_adding) {
  for (const auto &node : nodes_for_adding) {
    add_node(node);
  }
}

void Graph::add_edge(size_t u_of_edge, size_t v_of_edge, int weight) {
  add_node(u_of_edge);
  add_node(v_of_edge);
  adj_list[u_of_edge][v_of_edge] = weight;
}

void Graph::add_edges_from(
    const std::vector<std::tuple<std::size_t, std::size_t, int>>
        &edges_for_adding) {
  size_t u, v;
  int w;
  for (const auto &edge : edges_for_adding) {
    tie(u, v, w) = edge;
    add_edge(u, v, w);
  }
}

size_t Graph::number_of_nodes() const { return nodes.size(); }

Graph Graph::node_subgraph(const set<size_t> &sub_nodes) const {
  auto g = Graph();
  for (const auto &p1 : adj_list) {
    const auto &u = p1.first;
    if (sub_nodes.find(u) == sub_nodes.end()) continue;
    for (const auto &p2 : p1.second) {
      const auto &v = p2.first;
      if (sub_nodes.find(v) == sub_nodes.end()) continue;
      const auto &w = p2.second;
      g.add_edge(u, v, w);
    }
  }
  return g;
}

Graph Graph::node_subgraph(const vector<std::size_t> &sub_nodes) const {
  auto distinct_nodes = set<size_t>(sub_nodes.begin(), sub_nodes.end());
  return node_subgraph(distinct_nodes);
}

Graph Graph::edge_subgraph(
    const set<pair<std::size_t, size_t>> &sub_edges) const {
  auto g = Graph();
  for (const auto &p : sub_edges) {
    const auto &u = p.first;
    const auto &v = p.second;
    if (nodes.find(u) != nodes.end() && nodes.find(v) != nodes.end()) {
      const auto &neighbour = adj_list.find(u)->second;
      const auto &it = neighbour.find(v);
      if (it != neighbour.end()) {
        g.add_edge(u, v, it->second);
      }
    }
  }
  return g;
}
Graph Graph::edge_subgraph(
    const std::vector<std::pair<std::size_t, std::size_t>> &sub_edges) const {
  auto distinct_edges = set<std::pair<std::size_t, std::size_t>>(
      sub_edges.begin(), sub_edges.end());
  return edge_subgraph(distinct_edges);
}
