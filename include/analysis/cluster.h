#ifndef ANALYSIS_CLUSTER_H
#define ANALYSIS_CLUSTER_H

#include <algorithm>
#include <vector>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
#include "omp.h"
namespace Backend {
/**
 * return undirected and unweighted triangles
 */
template <class Graph>
void _triangles(Graph &g, std::size_t *nnbr, std::size_t *nTriangles);

/**
 * Return number of directed triangles in (i,k,j)
 */
size_t _tc(std::vector<std::size_t> &ip, std::vector<std::size_t> &is, std::vector<std::size_t> &jp,
           std::vector<std::size_t> &js);

/*
 * local clustering coefficient
 */
void get_cluster_coef(DirectedGraph &g, double *cluster_coef);
void get_cluster_coef(UndirectedGraph &g, double *cluster_coef);

template <class Graph>
void get_transtivity(Graph g, double &transtivity);

/**
 *  Implementation
 */
template <class Graph>
void _triangles(Graph &g, std::size_t *nnbr, std::size_t *nTriangles) {
    // This double counts triangles so you may want to divide by 2.
    size_t              nv     = g.number_of_nodes();
    const auto         &column = g.get_columns();
    const auto         &offset = g.get_offsets();
    std::vector<size_t> nbr[nv];

    for (size_t i = 0; i < nv; i++) {
        for (size_t p = offset[i]; p < offset[i + 1]; p++) {
            if (i != column[p]) {  // remove self-loop
                nbr[i].push_back(column[p]);
            }
        }
        nnbr[i] = nbr[i].size();
    }
    std::vector<size_t> tmp(nv);
#pragma omp parallel for
    for (size_t i = 0; i < nv; i++) {
        nTriangles[i] = 0;
        for (size_t p = offset[i]; p < offset[i + 1]; p++) {
            auto it = set_intersection(nbr[i].begin(), nbr[i].end(), nbr[column[p]].begin(),
                                       nbr[column[p]].end(), tmp.begin());
            nTriangles[i] += it - tmp.begin();
        }
    }
}

template <class Graph>
void get_transtivity(Graph g, double &transtivity) {
    std::size_t  nv         = g.get_vertex_num();
    std::size_t *nnbr       = (std::size_t *)(malloc(nv * sizeof(std::size_t)));
    std::size_t *nTriangles = (std::size_t *)(malloc(nv * sizeof(std::size_t)));
    size_t       triangles = 0, triads = 0;
    _triangles(g, nnbr, nTriangles);
    for (int i = 0; i < nv; i++) {
        triangles += nTriangles[i];
        triads += nnbr[i] * (nnbr[i] - 1);
    }
    if (triads)
        transtivity = 1.0 * triangles / triads;
    else
        transtivity = 0;
    delete nnbr;
    delete nTriangles;
}

}  // namespace Backend

#endif