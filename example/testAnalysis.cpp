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

int                    is_directed;
string                 input_path;
string                 origin_path;
bool                   t_degree, t_cluster, t_hot_polt, t_linear, t_cncomp;
bool                   t_timer, t_origin;
std::shared_ptr<Graph> origin_graph;

// Degree distribution and assortativity
void test_degree(std::shared_ptr<Graph> graph) {
    size_t              nv = graph->number_of_nodes();
    map<size_t, size_t> deg_disb;
    double              avg_degree;
    double              asso_coef;
    auto                start = high_resolution_clock::now();
    Backend::get_degree_avg(*graph, avg_degree);
    Backend::get_degree_disb(*graph, deg_disb);
    if (t_origin) {
        size_t  nvori   = origin_graph->number_of_nodes();
        double *dgnew   = (double *)malloc(nv * sizeof(double));
        double *dgori   = (double *)malloc(nvori * sizeof(double));
        auto    degrees = graph->get_degrees();
        double  ks_stats;
        double  avg_degree_ori;

        Backend::get_degree_avg(*origin_graph, avg_degree_ori);
        for (size_t i = 0; i < nv; i++)
            dgnew[i] = degrees[i];
        degrees = origin_graph->get_degrees();
        for (size_t i = 0; i < nvori; i++)
            dgori[i] = degrees[i];
        getKSStats(dgnew, dgori, nv, nvori, ks_stats);

        double              ks_stats1;
        map<size_t, size_t> deg_disb_ori;
        Backend::get_degree_disb(*origin_graph, deg_disb_ori);
        getKSStatsMap(deg_disb, deg_disb_ori, ks_stats1);

        delete dgnew;
        delete dgori;
        std::cout << "[Degree]Degree distribution: " << ks_stats << "----" << ks_stats1 << endl;
    } else {
        std::cout << "[Degree]Average degree: " << avg_degree << endl;
    }
    Backend::get_degree_asso(*graph, asso_coef);
    std::cout << "[Degree]Assortativity coefficient: " << asso_coef << endl;
    if (t_timer) {
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Degree Analysis Time"
                  << ": " << duration.count() << " microseconds" << endl;
    }
}

// Clustering coefficient
void test_cluster(std::shared_ptr<Graph> graph) {
    auto                start = high_resolution_clock::now();
    size_t              nv    = graph->number_of_nodes();
    map<double, size_t> cluster_coef;
    Backend::get_cluster_coef_distb(*graph, cluster_coef);
    if (t_origin) {
        map<double, size_t> cluster_coef_ori;
        double              ks_stats;

        Backend::get_cluster_coef_distb(*origin_graph, cluster_coef_ori);
        getKSStatsMap(cluster_coef, cluster_coef_ori, ks_stats);
        std::cout << "[Cluster]Clustering coefficient distribution: " << ks_stats << endl;
    }
    if (t_timer) {
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Clustering Analysis Time: " << duration.count() << " microseconds" << endl;
    }
}

// Hot-plot neighborhood
void test_hot_plot(std::shared_ptr<Graph> graph) {
    auto                start = high_resolution_clock::now();
    map<size_t, size_t> nbrhd_func_map;
    Backend::get_hop_plot(*graph, nbrhd_func_map);
    if (t_origin) {
        map<size_t, size_t> nbrhd_func_map_ori;
        double              ks_stats;
        Backend::get_hop_plot(*origin_graph, nbrhd_func_map_ori);
        getKSStatsMap(nbrhd_func_map, nbrhd_func_map_ori, ks_stats);
        std::cout << "[Hot-plot]Hot-plot neighborhood function distribution: " << ks_stats << endl;
    }
    if (t_timer) {
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Hot-plot Analysis Time: " << duration.count() << " microseconds" << endl;
    }
}

// Eigen value and Singular value
void test_linear(std::shared_ptr<Graph> graph) {
    auto   start = high_resolution_clock::now();
    sp_mat spr_mat;
    Backend::get_sparse_mat(*graph, spr_mat);
    size_t k = 4;
    if (!is_directed) {
        vec eigval;
        mat eigvec;
        Backend::get_eigen(eigval, eigvec, spr_mat, k);
        std::cout << "[Linear]First " << k << " eigenvalue: " << endl;
        eigval.print();
    } else {
        cx_vec eigval;
        cx_mat eigvec;
        Backend::get_eigen(eigval, eigvec, spr_mat, k);
        std::cout << "[Linear]First " << k << " eigenvalue: " << endl;
        eigval.print();
    }

    mat U, V;
    vec S;
    Backend::get_singular(U, S, V, spr_mat, 4);
    std::cout << "[Linear]First " << k << " singular values:" << endl;
    S.print();
    if (t_timer) {
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Linear Analysis Time: " << duration.count() << " microseconds" << endl;
    }
}

// Connected component
void test_cncomp(std::shared_ptr<Graph> graph) {
    auto                start = high_resolution_clock::now();
    map<size_t, size_t> sccs_distb;
    map<size_t, size_t> wccs_distb;
    Backend::get_wccs_distb(*graph, wccs_distb);
    if (is_directed) Backend::get_sccs_distb(*graph, sccs_distb);
    if (t_origin) {
        map<size_t, size_t> sccs_distb_ori;
        map<size_t, size_t> wccs_distb_ori;
        Backend::get_wccs_distb(*origin_graph, wccs_distb_ori);
        if (is_directed) Backend::get_sccs_distb(*origin_graph, sccs_distb_ori);
        double wcc_ks_stats, scc_ks_stats;
        getKSStatsMap(wccs_distb, wccs_distb_ori, wcc_ks_stats);
        if (is_directed) getKSStatsMap(sccs_distb, sccs_distb_ori, scc_ks_stats);
        std::cout << "[Cncomp]Wccs distribution: " << wcc_ks_stats << endl;
        if (is_directed) std::cout << "[Cncomp]Sccs distribution: " << scc_ks_stats << endl;
    }
    // std::cout"Total %ld weakly connected component.\n", wccs.size());
    if (t_timer) {
        auto stop     = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Connected Component Analysis Time: " << duration.count() << " microseconds"
                  << endl;
    }
}

void cmdInp(int argc, char **argv) {
    if (argc < 2) {
        cout << "[usage]: " << argv[0] << "path/to/input"
             << "directed/undirected" << endl
             << "[Options]: " << endl
             << "\t-degree\t\tDegree test." << endl
             << "\t-cluster\t\tClustring coeifficient test." << endl
             << "\t-hot_plot\t\tHot-plot coefficient test." << endl
             << "\t-cncomp\t\tConnected component test." << endl
             << "\t-linear\t\tLinear test." << endl
             << "\t-origin <path>\t\tOriginal graph to compare with. Make sure the original graph "
                "is also directed/undirected, same with the input graph."
             << endl
             << "\t-timer\t\tUse timer in test." << endl;
        exit(1);
    }
    int _argc   = 0;
    input_path  = argv[++_argc];
    is_directed = false;
    t_degree = t_cluster = t_hot_polt = t_linear = t_cncomp = t_timer = t_origin = false;
    if (argc > 2) {
        if (strcmp(argv[++_argc], "directed") == 0)
            is_directed = true;
        else if (strcmp(argv[++_argc], "undirected") == 0)
            is_directed = false;
        else
            throw std::invalid_argument(
                "Please add directed/undirect to specific the graph before other options.");
    }
    while (++_argc < argc) {
        if (strcmp(argv[_argc], "-degree") == 0)
            t_degree = true;
        else if (strcmp(argv[_argc], "-cluster") == 0)
            t_cluster = true;
        else if (strcmp(argv[_argc], "-hot_plot") == 0)
            t_hot_polt = true;
        else if (strcmp(argv[_argc], "-linear") == 0)
            t_linear = true;
        else if (strcmp(argv[_argc], "-cncomp") == 0)
            t_cncomp = true;
        else if (strcmp(argv[_argc], "-timer") == 0)
            t_timer = true;
        else if (strcmp(argv[_argc], "-origin") == 0) {
            t_origin    = true;
            origin_path = argv[++_argc];
        }
    }
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);
    auto graph = GraphStream::readText(input_path, is_directed);
    if (t_origin) origin_graph = GraphStream::readText(origin_path, is_directed);

    if (t_degree) test_degree(graph);
    if (t_cluster) test_cluster(graph);
    if (t_cncomp) test_cncomp(graph);
    if (t_hot_polt) test_hot_plot(graph);
    if (t_linear) test_linear(graph);

    return 0;
}