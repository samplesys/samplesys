#include "analysis/linear.h"

namespace Backend {
void get_eigenval(arma::sp_mat &adj_mat, const int &k) {
    arma::vec eigval;
    arma::mat eigvec;
    arma::eigs_sym(eigval, eigvec, adj_mat, k, "la");
    printf("First %d eigenvalue:\n", k);
    eigval.print();
}

void get_singularval(arma::sp_mat &adj_mat, const int &k) {
    arma::mat U;
    arma::vec s;
    arma::mat V;
    arma::svds(U, s, V, adj_mat, k);
    printf("First %d singular values:\n", k);
    std::cout << "U:" << arma::size(U) << std::endl;
    std::cout << "s:" << arma::size(s) << std::endl;
    std::cout << "V:" << arma::size(V) << std::endl;
    s.print();
}
}  // namespace Backend