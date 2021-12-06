#ifndef ANALYSIS_LINEAR_H
#define ANALYSIS_LINEAR_H

#include <armadillo>
#include <iostream>
namespace Backend {

template <class Graph>
void get_sparse_mat(const Graph &g, arma::sp_mat &adj_mat);

// find k eigen values/vectors
void get_eigenval(arma::sp_mat &adj_mat, const int &k);

// find k singular values/vectors
void get_singularval(arma::sp_mat &adj_mat, const int &k);


/**
 * Implementation
 */
template <class Graph>
void get_sparse_mat(const Graph &g, arma::sp_mat &adj_mat) {
    size_t       nv     = g.number_of_nodes();
    size_t       ne     = g.number_of_edges();
    const auto  &edges  = g.get_columns();
    const auto  &offset = g.get_offsets();
    size_t       cnt    = 0;
    arma::umat   location(2, ne);
    arma::colvec values(ne);
    for (size_t i = 0; i < nv; i++) {
        for (size_t p = offset[i]; p < offset[i + 1]; p++) {
            size_t j         = edges[p];
            values(cnt)      = 1;
            location(0, cnt) = i;
            location(1, cnt) = j;
            cnt++;
        }
    }
    adj_mat = arma::sp_mat(location, values);
}
}  // namespace Backend
#endif