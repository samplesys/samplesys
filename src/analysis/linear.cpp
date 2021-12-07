#include <armadillo>
using namespace arma;
namespace Backend {
void get_eigen(vec &eigval, mat &eigvec, const sp_mat &adj_mat, const int &k) {
    eigs_sym(eigval, eigvec, adj_mat, k, "la");
}

void get_singular(mat &U, vec &S, mat &V, const sp_mat &adj_mat, const int &k) {
    svds(U, S, V, adj_mat, k);
}
}  // namespace Backend