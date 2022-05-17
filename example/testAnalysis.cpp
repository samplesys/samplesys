#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <typeinfo>

#include "3_party/armadillo/armadillo"
#include "analysis/Backend.h"
#include "utils.h"
#include "utils/GraphStream.h"

using namespace Backend;
using namespace std;
using namespace std::chrono;
using namespace arma;

void test_Analysis(std::shared_ptr<Graph> graph) {
    map<size_t, size_t> deg_disb;
    double              avg_degree;
    double              asso_coef;

    // Degree distribution and assortativity
    auto start = high_resolution_clock::now();
    Backend::get_degree_avg(*graph, avg_degree);
    printf("Average degree: %lf\n", avg_degree);
    Backend::get_degree_disb(*graph, deg_disb);
    // printf("Degree distribution:\nDegree\tCount\n");
    // for (auto x : deg_disb)
    //     printf("%zd\t%zd\n", x.first, x.second);
    Backend::get_degree_asso(*graph, asso_coef);
    printf("Degree assortativity coefficient: %lf\n", asso_coef);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Degree Analysis Time"
         << ": " << duration.count() << " microseconds" << endl;

    // Clustering coefficient
    start                = high_resolution_clock::now();
    size_t  nv           = graph->number_of_nodes();
    double *cluster_coef = reinterpret_cast<double *>(malloc(nv * sizeof(double)));
    Backend::get_cluster_coef(*graph, cluster_coef);
    // printf("Clustering coefficient:\n");
    // for (size_t i = 0; i < nv; i++)
    //     printf("%zd %lf\n", i, cluster_coef[i]);
    delete cluster_coef;
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Clustering Analysis Time: " << duration.count() << " microseconds" << endl;

    // Hot-plot neighborhood
    start = high_resolution_clock::now();
    map<size_t, size_t> nbrhd_func_map;
    Backend::get_hop_plot(*graph, nbrhd_func_map);
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Hot-plot Analysis Time: " << duration.count() << " microseconds" << endl;

    // Eigen value and Singular value
    start = high_resolution_clock::now();
    sp_mat spr_mat;
    Backend::get_sparse_mat(*graph, spr_mat);
    size_t k = 4;
    if (typeid(*graph) == typeid(UndirectedGraph)) {
        vec eigval;
        mat eigvec;
        Backend::get_eigen(eigval, eigvec, spr_mat, k);
    } else {
        cx_vec eigval;
        cx_mat eigvec;
        Backend::get_eigen(eigval, eigvec, spr_mat, k);
    }
    // printf("First %d eigenvalue:\n", 10);
    // eigval.print();
    mat U, V;
    vec S;
    Backend::get_singular(U, S, V, spr_mat, 4);
    // printf("First %d singular values:\n", 10);
    // std::cout << "U:" << size(U) << std::endl;
    // std::cout << "S:" << size(S) << std::endl;
    // std::cout << "V:" << size(V) << std::endl;
    // S.print();
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Linear Analysis Time: " << duration.count() << " microseconds" << endl;

    // Connected component
    start = high_resolution_clock::now();
    vector<vector<size_t>> sccs;
    vector<vector<size_t>> wccs;
    if (typeid(*graph) == typeid(DirectedGraph)) Backend::get_sccs(*graph, sccs);
    Backend::get_wccs(*graph, wccs);
    // printf("Total %ld weakly connected component.\n", wccs.size());
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Connected Component Analysis Time: " << duration.count() << " microseconds" << endl;
}

void cmdInp(int argc, char **argv) {
    if (argc < 2) {
        cout << "[usage]: " << argv[0] << "path/to/input"
             << "directed/undirected" << endl;
        exit(1);
    }
    int    _argc       = 0;
    string input       = argv[++_argc];
    bool   is_directed = false;
    if (argc > 2) is_directed = (strcmp(argv[++_argc], "directed") == 0 ? true : false);
    auto graph = GraphStream::readText(input, is_directed);
    test_Analysis(graph);
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);
    return 0;
}