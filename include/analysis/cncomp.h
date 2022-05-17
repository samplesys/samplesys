#ifndef ANALYSIS_CNCOMP_H
#define ANALYSIS_CNCOMP_H

#include <algorithm>
#include <cstring>
#include <exception>
#include <map>
#include <queue>
#include <vector>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
namespace Backend {

/**
 * Get WCCs(weekly-connected-component)
 */
void get_wccs_distb(const Graph &g, std::map<std::size_t, std::size_t> &wccs_distb);
void get_wccs(const Graph &g, std::vector<std::vector<std::size_t>> &wccs);
void _get_wccs(const UndirectedGraph *g, std::vector<std::vector<std::size_t>> &wccs);
void _get_wccs(const DirectedGraph *g, std::vector<std::vector<std::size_t>> &wccs);

/**
 * Get SCCs(strongly-connected-component)
 */
void get_sccs_distb(const Graph &g, std::map<std::size_t, std::size_t> &sccs_distb);
void get_sccs(const Graph &g, std::vector<std::vector<std::size_t>> &sccs);

/**
 * Implemention
 */

}  // namespace Backend
#endif