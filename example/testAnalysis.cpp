#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include "analysis/Backend.h"
#include "utils.h"
#include "utils/GraphStream.h"

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

    auto graph = GraphStream::readText(input);

    map<size_t, size_t> deg_disb;
    double              avg_degree;
    double              asso_coef;

    auto start = high_resolution_clock::now();

    Backend::get_degree_avg(*graph, avg_degree);
    printf("Average degree: %lf\n", avg_degree);
    Backend::get_degree_disb(*graph, deg_disb);
    printf("Degree distribution:\nDegree\tCount\n");
    for (auto x : deg_disb) {
        printf("%zd\t%zd\n", x.first, x.second);
    }
    Backend::get_degree_asso(*graph, asso_coef);
    printf("Degree assortativity coefficient: %lf\n", asso_coef);

    auto stop     = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time"
         << ": " << duration.count() << " microseconds" << endl;
}

int main(int argc, char *argv[]) {
    cmdInp(argc, argv);

    return 0;
}