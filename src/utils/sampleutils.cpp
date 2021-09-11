/**
 * MIT License
 *
 * Copyright (c) 2020, Beijing University of Posts and Telecommunications.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include "utils/sampleutils.h"

/* f-norm of vector x-y */
double F_norm(double *x, double *y, int n) {
    double norm = 0;
    for (int i = 0; i < n; i++)
        norm += (x[i] - y[i]) * (x[i] - y[i]);
    return sqrtf32(norm);
}

double ddrand(double a, double b) {
    double tmp = mainrandom.drand();
    return a + tmp * (b - a);
}

/* shuffle an array to random permutation of 0 ~ size-1 */
void shuffle(int *a, int size) {
    int j;
    for (int i = 0; i < size; i++) {
        a[i] = i;
        j = mainrandom.irand(i + 1);
        std::swap(a[i], a[j]);
    }
}

long long find_backward_edge(LSGraph *graph, int p) {
    long long *offsets = graph->get_offsets();
    long long nv = graph->get_vertex_num();
    int to_vertex = graph->edges[p];
    int from_vertex = std::upper_bound(offsets, offsets + nv, p) - offsets - 1;
    return std::lower_bound(graph->edges + offsets[to_vertex],
                            graph->edges + offsets[to_vertex + 1], from_vertex) -
           graph->edges;
}