//
// Created by AutJ on 2021/11/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_

#include <map>
#include <set>
#include <vector>

#include "graph/Graph.h"

/**
 * Undirected Graph
 * the edge between i and j is saved as both i->j and j->i
 */
class UndirectedGraph : public Graph {
   public:
    UndirectedGraph(std::map<std::size_t, std::set<std::size_t>>& adjList,
                    std::size_t                                   number_of_nodes);
};

#endif  // SAMPLER_INCLUDE_GRAPH_UNDIRECTEDGRAPH_H_
