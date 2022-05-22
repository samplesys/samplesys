#ifndef ANALYSIS_HOTPLOT_H
#define ANALYSIS_HOTPLOT_H

#include <algorithm>
#include <map>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
#include "omp.h"
#include "utils/hyperloglog.hpp"

namespace Backend {

template <class Graph>
void get_hop_plot(const Graph &g, std::map<std::size_t, std::size_t> &nbrhd_func_map);

/**
 * Implementation
 */

/**
 * @brief Get the hop plot(neighbouring function of the graph)
 *
 * @tparam Graph
 * @param g graph to get the hop plot
 * @param nbrhd_func_map where to save the hop plot information
 */
template <class Graph>
void get_hop_plot(const Graph &g, std::map<std::size_t, std::size_t> &nbrhd_func_map) {
    std::size_t t      = 0;
    std::size_t nv     = g.number_of_nodes();
    std::size_t ne     = g.number_of_edges();
    const auto &column = g.get_columns();
    const auto &offset = g.get_offsets();

    bool             changed = true;
    hll::HyperLogLog new_hlls[nv];
    hll::HyperLogLog old_hlls[nv];
    nbrhd_func_map.clear();
#pragma omp parallel for
    for (std::size_t i = 0; i < nv; i++) {
        old_hlls[i] = hll::HyperLogLog(12);
        old_hlls[i].add(i);
        new_hlls[i] = old_hlls[i];
    }
    while (changed) {
        changed           = false;
        nbrhd_func_map[t] = 0;
#pragma omp parallel for
        for (std::size_t i = 0; i < nv; i++) {
            nbrhd_func_map[t] += new_hlls[i].estimate();
            old_hlls[i] = new_hlls[i];
        }
#pragma omp parallel for
        for (std::size_t vid = 0; vid < nv; vid++) {
            for (std::size_t p = offset[vid]; p < offset[vid + 1]; p++) {
                std::size_t uid = column[p];
                changed         = new_hlls[vid].merge(old_hlls[uid]) || changed;
            }
        }
        t++;
    }
    std::size_t precnt = 0;
    for (auto it = nbrhd_func_map.begin(); it != nbrhd_func_map.end(); it++) {
        if (it == nbrhd_func_map.begin()) {
            precnt = it->second;
            continue;
        }
        std::size_t nowcnt = it->second;
        it->second         = nowcnt - precnt;
        precnt             = nowcnt;
        // printf("%ld %ld\n", it->first, it->second);
    }
}

}  // namespace Backend

#endif
