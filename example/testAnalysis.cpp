#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include "analysis/Backend.h"
#include "utils.h"

using namespace Backend;
using namespace std;
using namespace std::chrono;

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
    // string graphType      = argv[++_argc];

    size_t number_of_nodes;
    auto   adjList = map<std::size_t, set<std::size_t>>();
    readGraph(number_of_nodes, adjList, input);

    auto                graph = UndirectedGraph(number_of_nodes, adjList);
    map<size_t, size_t> deg_disb;
    double              avg_degree;
    double              asso_coef;

    auto start = high_resolution_clock::now();

    Backend::get_degree_avg(graph, avg_degree);
    printf("Average degree: %lf\n", avg_degree);
    Backend::get_degree_disb(graph, deg_disb);
    printf("Degree distribution:\n");
    for (auto x : deg_disb) {
        printf("%zd %zd\n", x.first, x.second);
    }
    Backend::get_degree_asso(graph, asso_coef);
    printf("%lf", asso_coef);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time"
         << ": " << duration.count() << " microseconds" << endl;
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);

    return 0;
}