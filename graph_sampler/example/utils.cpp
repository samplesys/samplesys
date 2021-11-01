//
// Created by AutJ on 2021/11/1.
//

#include "utils.h"

#include <graph/Graph.h>

#include <fstream>
#include <iterator>

using namespace std;

void readGraph(Graph &graph, const string &file) {
  ifstream is(file);
  size_t nv, u, v;
  is >> nv;
  for (int i = 0; i < nv; ++i) {
    is >> u >> v;
    graph.add_edge(u, v, 1);
  }
}

void printGraph(Graph &graph, const string &file) {
  ofstream os(file);
  const auto &nodes = graph.get_nodes();
  const auto &edges = graph.get_edges();
  copy(nodes.begin(), nodes.end(), ostream_iterator<size_t>(os, " "));
  os << endl;
  size_t u, v;
  for (const auto &t : edges) {
    os << t.first << ' ' << t.second << endl;
  }
}
