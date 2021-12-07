#ifndef ANALYSIS_DEGREE_H
#define ANALYSIS_DEGREE_H

#include <cmath>
#include <cstring>
#include <map>
#include <vector>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"

namespace Backend {

template <class Graph>
void get_degree_disb(const Graph& g, std::map<std::size_t, std::size_t>& disb_degree);

template <class Graph>
void get_degree_avg(const Graph& g, double& avg_degree);

template <class Graph>
void get_degree_asso(const Graph& g, double& assortativty_coef);

/* Compute the outer product of two vectors. */
void outer(std::vector<double>& x, std::vector<double>& y, std::vector<std::vector<double>>& xy);

template <class Graph>
void get_degree_disb(const Graph& g, std::map<std::size_t, std::size_t>& disb_degree) {
    std::size_t nv     = g.number_of_nodes();
    const auto& degree = g.get_degrees();
    disb_degree.clear();
    for (std::size_t i = 0; i < nv; i++)
        disb_degree[degree[i]]++;
}

template <class Graph>
void get_degree_avg(const Graph& g, double& avg_degree) {
    std::size_t nv     = g.number_of_nodes();
    const auto& offset = g.get_offsets();
    const auto& column = g.get_columns();
    std::size_t sumd   = 0;
    std::size_t sumv   = 0;
    for (std::size_t i = 0; i < nv; i++) {
        std::size_t d = 0;
        for (std::size_t p = offset[i]; p < offset[i + 1]; p++) {
            if (i == column[p]) continue;  // cut off self loops
            sumd++;
            d++;
        }
        sumv += (d != 0);
    }
    avg_degree = 1.0 * sumd / sumv;
}

template <class Graph>
void get_degree_asso(const Graph& g, double& assortativty_coef) {
    std::map<std::size_t, std::size_t> dis_degree;
    get_degree_disb(g, dis_degree);

    std::size_t                        nv     = g.number_of_nodes();
    std::size_t                        n      = dis_degree.size();
    std::size_t                        id     = 0;
    const auto&                        offset = g.get_offsets();
    const auto&                        degree = g.get_degrees();
    const auto&                        column = g.get_columns();
    std::map<std::size_t, std::size_t> mapping;
    std::vector<std::size_t>           degrees(n);

    for (auto it = dis_degree.begin(); it != dis_degree.end(); it++, id++) {
        degrees[id]        = it->first;
        mapping[it->first] = id;
    }
    // get degree_mixing_matrix M
    std::size_t sum = 0;
    double      M[n][n];
    std::memset(M, 0, sizeof(M));
    for (std::size_t i = 0; i < nv; i++) {
        for (std::size_t p = offset[i]; p < offset[i + 1]; p++) {
            std::size_t x = degree[i], y = degree[column[p]];
            sum++;
            M[mapping[x]][mapping[y]]++;
        }
    }
#pragma omp parallel for
    for (std::size_t i = 0; i < n; i++) {
#pragma omp parallel for
        for (std::size_t j = 0; j < n; j++) {
            M[i][j] /= sum;
        }
    }

    // numeric assortativity coefficient, pearsonr
    std::vector<double>              x, y, a(n, 0), b(n, 0);
    std::vector<std::vector<double>> xy, ab;
    id = 0;
    for (auto it = mapping.begin(); it != mapping.end(); it++, id++) {
        x.push_back(it->first);
        y.push_back(it->first);
    }
    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            a[j] += M[i][j], b[i] += M[i][j];
        }
    }
    double vara = 0, varb = 0, Ex = 0, Ex2 = 0, Ey = 0, Ey2 = 0;
    for (std::size_t i = 0; i < n; i++) {
        Ex += a[i] * x[i], Ex2 += a[i] * x[i] * x[i];
        Ey += b[i] * y[i], Ey2 += b[i] * y[i] * y[i];
    }
    vara = Ex2 - Ex * Ex, varb = Ey2 - Ey * Ey;
    outer(x, y, xy);
    outer(a, b, ab);
    double dsum = 0;
    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j <= n; j++) {
            dsum += xy[i][j] * (M[i][j] - ab[i][j]);
        }
    }
    if (!vara || !varb) printf("Network has perfect assortative mixing patterns.\n");
    assortativty_coef = 1.0 * dsum / std::sqrt(vara * varb);
}

}  // namespace Backend

#endif