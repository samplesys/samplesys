#ifndef ANALYSIS_LINEAR_H
#define ANALYSIS_LINEAR_H

#include <iostream>

#include "3_party/armadillo/armadillo"
#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"

using namespace arma;

namespace Backend {

template <class Graph>
void get_sparse_mat(const Graph &g, arma::sp_mat &adj_mat);

/**
 * @brief Get k eigen values/vectors for sparse matrix adj_mat.
 *
 * @param eigval where to save eigenvalues
 * @param eigvec where to save eigenvector
 * @param adj_mat
 * @param k the number of eigenvalues and eigenvectors
 * @attention If X is not square sized, a std::logic_error exception is thrown
 * @attention If adj_mat is asymmetric(i.e. Directedgraph), use cx_vec and cx_mats.
 */
void get_eigen(arma::vec &eigval, arma::mat &eigvec, const arma::sp_mat &adj_mat, const int &k);
void get_eigen(arma::cx_vec &eigval, arma::cx_mat &eigvec, const arma::sp_mat &adj_mat,
               const int &k);

/**
 * @brief Get k singular values and singular vectors (truncated SVD) of sparse matrix adj_mat
 *
 * @param U
 * @param S
 * @param V
 * @param adj_mat
 * @param k the number of singular values and singular vectors
 */
void get_singular(arma::mat &U, arma::vec &S, arma::mat &V, const arma::sp_mat &adj_mat,
                  const int &k);

/**
 * Implementation
 */
template <class Graph>
void get_sparse_mat(const Graph &g, arma::sp_mat &adj_mat) {
    std::size_t  nv     = g.number_of_nodes();
    std::size_t  ne     = g.number_of_edges();
    const auto  &edges  = g.get_columns();
    const auto  &offset = g.get_offsets();
    std::size_t  cnt    = 0;
    arma::umat   location(2, ne);
    arma::colvec values(ne);
    // turn to coo format sparse matrix
    for (std::size_t i = 0; i < nv; i++) {
        for (std::size_t p = offset[i]; p < offset[i + 1]; p++) {
            std::size_t j    = edges[p];
            values(cnt)      = 1;
            location(0, cnt) = i;
            location(1, cnt) = j;
            cnt++;
        }
    }
    adj_mat = arma::sp_mat(location, values, nv, nv);
}
}  // namespace Backend
#endif