#include <armadillo>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include "analysis/Backend.h"
#include "graph/UndirectedGraph.h"
#include "utils.h"

using namespace Backend;
using namespace std;
using namespace std::chrono;
using namespace arma;

void cmdInp(int argc, char **argv) {
    if (argc < 2) {
        cout << "[usage]: " << argv[0]
             << " path/to/input"
             // << "directed/undirected"
             << endl;
        exit(1);
    }
    int    _argc = 0;
    string input = argv[++_argc];

    size_t number_of_nodes;
    auto   adjList = map<std::size_t, set<std::size_t>>();
    readGraph(number_of_nodes, adjList, input);

    auto                graph = UndirectedGraph(number_of_nodes, adjList);
    map<size_t, size_t> deg_disb;
    double              avg_degree;
    double              asso_coef;

    auto start = high_resolution_clock::now();

    Backend::get_degree_avg(graph, avg_degree);
    // printf("Average degree: %lf\n", avg_degree);
    Backend::get_degree_disb(graph, deg_disb);
    // printf("Degree distribution:\nDegree\tCount\n");
    // for (auto x : deg_disb) {
    //     printf("%zd\t%zd\n", x.first, x.second);
    // }
    Backend::get_degree_asso(graph, asso_coef);
    // printf("Degree assortativity coefficient: %lf\n", asso_coef);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Degree Analysis Time"
         << ": " << duration.count() << " microseconds" << endl;

    start                = high_resolution_clock::now();
    size_t  nv           = graph.number_of_nodes();
    double *cluster_coef = reinterpret_cast<double *>(malloc(nv * sizeof(double)));
    Backend::get_cluster_coef(graph, cluster_coef);
    // printf("Clustering coefficient:\n");
    // for (size_t i = 0; i < nv; i++)
    //     printf("%zd %lf\n", i, cluster_coef[i]);
    delete cluster_coef;
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Clustering Analysis Time: " << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    sp_mat spr_mat;
    Backend::get_sparse_mat(graph, spr_mat);
    vec eigval;
    mat eigvec;
    Backend::get_eigen(eigval, eigvec, spr_mat, 10);
    // printf("First %d eigenvalue:\n", 10);
    // eigval.print();
    mat U, V;
    vec S;
    Backend::get_singular(U, S, V, spr_mat, 10);
    // printf("First %d singular values:\n", 10);
    // std::cout << "U:" << size(U) << std::endl;
    // std::cout << "S:" << size(S) << std::endl;
    // std::cout << "V:" << size(V) << std::endl;
    // S.print();
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Linear Analysis Time: " << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();
    // vector<vector<size_t>> *sccs = new vector<vector<size_t>>;
    vector<vector<size_t>> *wccs = new vector<vector<size_t>>;
    // Backend::get_sccs(&graph, sccs);
    Backend::get_wccs(&graph, wccs);
    // printf("%Total zd weakly connected component.\n", wccs->size());
    delete wccs;
    stop     = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Connected Component Analysis Time: " << duration.count() << " microseconds" << endl;
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);

    return 0;
}