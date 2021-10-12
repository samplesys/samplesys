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
#include "apps/graphanalysis/analysis.h"

ANALYSISBACKEND::ANALYSISBACKEND(LSGraph *_graph, int _argc, char **_argv) {
    this->argc = _argc;
    this->argv = _argv;
    this->graph = _graph;
    this->get_args();
}

ANALYSISBACKEND::~ANALYSISBACKEND() {}

void ANALYSISBACKEND::get_args() {
    // int a;
    // if ((a = argPos(const_cast<char *>("-pageranek"), argc, argv)) > 0)
    //     pagerankflag = 1;
}

void ANALYSISBACKEND::work() {
    this->calcDegree();
    this->calcCluster();
    // this->calcNei(64,5,100);
}

/* Compute the outer product of two vectors. */
void outer(vector<double> x, vector<double> y, vector<vector<double>> &xy) {
    xy.clear();
    int n = x.size(), m = y.size();
    for (int i = 0; i < n; i++) {
        xy.push_back({});
        for (int j = 0; j < m; j++)
            xy[i].push_back(x[i] * y[j]);
    }
}

/* Degree correlation: The Pearson correlation coefficient between the starting and ending degrees
 * of all edges. Assortativity measures the similarity of connections in the graph with respect to
 * the node degree.
 */
void ANALYSISBACKEND::calcDegree() {
    int nv = graph->get_vertex_num();
    int *degree = graph->get_degree();
    int *edge = graph->get_edges();
    long long *offsets = graph->get_offsets();
    dis_degree.clear();
    for (int i = 0; i < nv; i++) {
        dis_degree[degree[i]]++;
    }
    printf("degree distribution:\n");
    for (auto deg : dis_degree) {
        printf("%d %d\n", deg.first, deg.second);
    }

    // degree assortativity coefficient: modified from networkx.degree_assortativity_coefficient()
    int n = dis_degree.size();
    vector<int> degrees(n);
    map<int, int> mapping;
    int id = 0;
    for (auto it = dis_degree.begin(); it != dis_degree.end(); it++, id++) {
        degrees[id] = it->first;
        mapping[it->first] = id;
    }

    // get degree_mixing_matrix M
    int sum = 0;
    double M[n][n];
    memset(M, 0, sizeof(M));
    for (int i = 0; i < nv; i++) {
        for (int p = offsets[i]; p < offsets[i + 1]; p++) {
            int x = degree[i], y = degree[edge[p]];
            sum++;
            M[mapping[x]][mapping[y]]++;
            // printf("%d->%d, %d %d %lf\n", i, edge[p], x, y, M[mapping[x]][mapping[y]]);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] /= sum;
            // printf("%lf ", M[i][j]);
        }
        // printf("\n");
    }

    // numeric assortativity coefficient, pearsonr
    vector<double> x, y, a(n, 0), b(n, 0);
    vector<vector<double>> xy, ab;
    id = 0;
    for (auto it = mapping.begin(); it != mapping.end(); it++, id++) {
        x.push_back(it->first);
        y.push_back(it->first);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            a[j] += M[i][j], b[i] += M[i][j];
    }
    double vara = 0, varb = 0, Ex = 0, Ex2 = 0, Ey = 0, Ey2 = 0;
    for (int i = 0; i < n; i++) {
        Ex += a[i] * x[i], Ex2 += a[i] * x[i] * x[i];
        Ey += b[i] * y[i], Ey2 += b[i] * y[i] * y[i];
    }
    vara = Ex2 - Ex * Ex, varb = Ey2 - Ey * Ey;
    outer(x, y, xy);
    outer(a, b, ab);
    double dsum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
            dsum += xy[i][j] * (M[i][j] - ab[i][j]);
    }
    assortativty_coef = dsum / sqrt(vara * varb);
    printf("Assortativity coefficient: %lf\n", assortativty_coef);
}

/* local clustering coefficient
 * C[i] = 2*nei[i]/( degree[i]*(degree[i]-1) ) / 2    (undirected edge)
 * dis_cluster[d] = average{C[i] | degree[i]=d}
 */
void ANALYSISBACKEND::calcCluster() {
    int nv = graph->get_vertex_num();
    int *degree = graph->get_degree();
    int *edges = graph->get_edges();

    long long *offset = graph->get_offsets();
    vector<vector<int>> neighbor(nv);
    vector<int> T(nv, 0);
    clustering_coef = (double *)(malloc(nv * sizeof(double)));
    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            neighbor[i].push_back(edges[p]);
            // neighbor[edges[p]].push_back(i);
        }
    }
    // printf("neighbor:\n");
    // for (int i = 0; i < nv; i++) {
    //     for (auto id : neighbor[i])
    //         printf("%d ", id);
    //     printf("\n");
    // }
    if (!graph->weighted) {
        vector<int> tmp(nv);
        FILE *fp = fopen("result.txt", "w");
        for (int i = 0; i < nv; i++) {
            for (int p = offset[i]; p < offset[i + 1]; p++) {
                auto it = set_intersection(neighbor[i].begin(), neighbor[i].end(),
                                           neighbor[edges[p]].begin(), neighbor[edges[p]].end(),
                                           tmp.begin());
                T[i] += it - tmp.begin();
            }
            fprintf(fp,"%d %lf\n", T[i] / 2,
                   T[i] / (1.0 * neighbor[i].size() * (neighbor[i].size() - 1)));
        }
        for (int i = 0; i < nv; i++)
            if (neighbor[i].size() > 1) {
                clustering_coef[i] = T[i] / (1.0 * neighbor[i].size() * (neighbor[i].size() - 1));
                dis_cluster[degree[i]] += clustering_coef[i] / dis_degree[degree[i]];
            }
        fclose(fp);
    }
    printf("Clustering coef:\n");
    for (auto ttt : dis_cluster)
        printf("%d: %lf\n", ttt.first, ttt.second);

    free(clustering_coef);
}

/*
    ANF-0

    IN(u, h) = | {v: v in V, dist(u, v) <= h} |
    N(h) = | {(u,v): u in V, v in V, dist(u, v) <= h} | or N(h) = sum{IN(u, h) | u in V}

    IN+(u, h, C) = | v: v in C, dist(u, v) <= h |
        IN(u, h) = IN+(u, h, V)
    N+(h, S, C) = | {(u, v): u in S, v in C, dist(u, v) <= h}
    N+(h, S, C) = sum{IN+(u, h, C) | u in S}
        N(h) = N+(h, V, V)
*/
void ANALYSISBACKEND::calcNei(int K = 64, int R = 5, int H = 100) {
    int nv = graph->get_vertex_num();
    int *edges = graph->get_edges();
    long long *offsets = graph->get_offsets();
    int Len = log2(nv) + R;
    vector<vector<bool>> M(nv, vector<bool>(K * Len, 0)), M0(nv, vector<bool>(K * Len, 0));
    vector<vector<double>> IN(nv, vector<double>(H + 1));
    vector<double> Nei(H + 1, 0);
    for (int i = 0; i < nv; i++) {
        for (int j = 0; j < K; j++) {
            for (int p = 0; p < Len; p++)
                M[i][p + Len * j] = mainrandom.drand() < pow(0.5, p + 1);
        }
    }
    for (int h = 1; h <= H; h++) {
        for (int i = 0; i < nv; i++) {
            M0[i] = M[i];
            for (int p = offsets[i]; p < offsets[i + 1]; p++)
                for (int x = 0; x < K * Len; x++)
                    M0[i][x] = M0[i][x] | M[edges[p]][x];
        }
        for (int i = 0; i < nv; i++) {
            int b = 0;  // average position of the least zero bit in k bitmasks
            for (int k = 0, x; k < K; k++) {
                for (x = 0; x < Len; x++)
                    if (M0[i][x + k * Len] == 0) break;
                b += x;
            }
            IN[i][h] = pow(2, b / K) / 0.77351;
            Nei[h] += IN[i][h];
        }
    }
    M = M0;
    for (int h = 1; h <= H; h++)
        printf("%lf\n", Nei[h]);
}
