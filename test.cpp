#include <cmath>
#include <cstdlib>
#include <utility>
#include <vector>

#include "page_rank.h"

// pageRank --
//
// g:           graph to process (see common/graph.h)
// solution:    array of per-vertex vertex scores (length of array is num_nodes(g))
// damping:     page-rank algorithm's damping parameter
// convergence: page-rank algorithm's convergence threshold
//
void pageRank(Graph g, double* solution, double damping, double convergence) {
#define NUM_THREADS 8
    const int     numNodes    = num_nodes(g);
    const double  equal_prob  = 1.0 / numNodes;
    double* const solutionOdd = new double[numNodes];
    double* const array[2]    = {solution, solutionOdd};

    Vertex* const deads    = new Vertex[numNodes];
    int           numDeads = 0;

    const double compensate = (1.0 - damping) * equal_prob;

#pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < numNodes; ++i) {
        solution[i] = equal_prob;
        if (0 == outgoing_size(g, i)) {
#pragma omp critical(in_deads)
            deads[numDeads++] = i;
        }
    }

    register bool isOdd     = false;  // round 0
    register bool converged = false;

    double diffSum;
    while (!converged) {
        isOdd   = !isOdd;
        diffSum = 0;
#pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < numNodes; ++i) {
            const Vertex* start = incoming_begin(g, i);
            const Vertex* end   = incoming_end(g, i);

            double tmp = 0;
#pragma omp parallel for
            for (const Vertex* v = start; v < end; ++v) {
                const int idx  = *v;
                double    temp = array[!isOdd][idx] / outgoing_size(g, idx);
#pragma omp atomic
                tmp += temp;
            }
#pragma omp parallel for
            for (int j = 0; j < numDeads; ++j) {
                double temp = array[!isOdd][deads[j]] * equal_prob;
#pragma omp atomic
                tmp += temp;
            }
            array[isOdd][i] = damping * tmp + compensate;

            tmp = abs(solutionOdd[i] - solution[i]);
#pragma omp atomic
            diffSum += tmp;
        }
        converged = (diffSum < convergence);
    }
    if (isOdd) {
#pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < numNodes; ++i)
            solution[i] = solutionOdd[i];
    }
    delete[] solutionOdd;
    delete[] deads;
}