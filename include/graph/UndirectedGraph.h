//
// Created by AutJ on 2021/11/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_

#include <map>
#include <set>
#include <vector>

#include "Graph.h"

/**
 * Undirected Graph
 * Convert the upper triangular matrix to CSR
 */
class UndirectedGraph : public Graph {
   public:
    UndirectedGraph(std::size_t                                   number_of_nodes,
                    std::map<std::size_t, std::set<std::size_t>>& adjList);
};

#endif  // SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_
