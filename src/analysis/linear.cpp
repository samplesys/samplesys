#include "analysis/linear.h"
using namespace arma;

namespace Backend {
void get_eigen(arma::vec &eigval, arma::mat &eigvec, const arma::sp_mat &adj_mat, const int &k) {
    eigs_sym(eigval, eigvec, adj_mat, k, "la");
}

void get_eigen(arma::cx_vec &eigval, arma::cx_mat &eigvec, const arma::sp_mat &adj_mat,
               const int &k) {
    eigs_gen(eigval, eigvec, adj_mat, k, "lm");
}

void get_singular(arma::mat &U, arma::vec &S, arma::mat &V, const arma::sp_mat &adj_mat,
                  const int &k) {
    svds(U, S, V, adj_mat, k);
}
}  // namespace Backend