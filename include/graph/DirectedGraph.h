//
// Created by AutJ on 2021/10/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_DIRECTEDGRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_DIRECTEDGRAPH_H_

#include <map>
#include <set>
#include <vector>

#include "graph/Graph.h"

/**
 * Directed Graph
 */
class DirectedGraph : public Graph {
   public:
    DirectedGraph() = default;
    DirectedGraph(std::map<std::size_t, std::set<std::size_t>>& adjList,
                  std::size_t                                   number_of_nodes);
};

#endif  // SAMPLER_INCLUDE_GRAPH_DIRECTEDGRAPH_H_
