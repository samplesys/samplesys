#ifndef ANALYSIS_CNCOMP_H
#define ANALYSIS_CNCOMP_H

#include <algorithm>
#include <cstring>
#include <queue>
#include <vector>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
namespace Backend {

/**
 * get WCCs(weekly-connected-component)
 */
void get_wccs(const Graph &g, std::vector<std::vector<std::size_t>> *wccs);
void _get_wccs(const UndirectedGraph *g, std::vector<std::vector<std::size_t>> *wccs);
void _get_wccs(const DirectedGraph *g, std::vector<std::vector<std::size_t>> *wccs);

/**
 * get SCCs(strongly-connected-component)
 */
void get_sccs(const DirectedGraph *g, std::vector<std::vector<std::size_t>> *sccs);

/**
 * Implemention
 */

}  // namespace Backend
#endif