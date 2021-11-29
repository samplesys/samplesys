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
    this->transGraphToMat();
    this->get_args();
}

ANALYSISBACKEND::~ANALYSISBACKEND() {}

void ANALYSISBACKEND::get_args() {
    // int a;
    // if ((a = argPos(const_cast<char *>("-pageranek"), argc, argv)) > 0)
    //     pagerankflag = 1;
}

void ANALYSISBACKEND::transGraphToMat() {
    int nv = graph->get_vertex_num();
    long long ne = graph->get_edge_num();
    int *edges = graph->get_edges(), cnt = 0;
    long long *offset = graph->get_offsets();
    umat location(2, ne);
    colvec values(ne);
    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            int j = edges[p];
            values(cnt) = 1;
            location(0, cnt) = i;
            location(1, cnt) = j;
            cnt++;
        }
    }
    adj_mat = sp_mat(location, values);
    // printf("%lld %lld %lld %lld\n", location.n_cols, location.n_rows, location.n_elem,
    //        values.n_elem);
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
 * of all edges.
 * Assortativity measures the similarity of connections in the graph with respect to
 * the node degree.
 *
 * referrence: networkx.degree_assortativity_coefficient()
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
    long long sumd = 0;
    printf("Degree distribution:\n");
    for (auto deg : dis_degree)
        printf("%d %d\n", deg.first, deg.second);
    for (int i = 0; i < nv; i++) {
        int d = 0;
        for (long long p = offsets[i]; p < offsets[i + 1]; p++) {
            if (i == edge[p]) continue;  // cut off self loops
            d++;
            sumd++;
        }
        dis_degree[d]++;
    }
    printf("Average degree: %lf\n", 1.0 * sumd / graph->get_exist_vertex_num());

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
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            M[i][j] /= sum;
        }
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
    if (!vara || !varb) printf("Network has perfect assortative mixing patterns.\n");
    assortativty_coef = dsum / sqrt(vara * varb);
    printf("Degree assortativity coefficient: %lf\n", assortativty_coef);
}

/**
 * return undirected and unweighted triangles
 */
void ANALYSISBACKEND::_triangles(int *nnbr, int *nTriangles) {
    // This double counts triangles so you may want to divide by 2.
    int nv = graph->get_vertex_num();
    int *edges = graph->get_edges();
    long long *offset = graph->get_offsets();
    vector<int> nbr[nv], tmp(nv);

    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            if (i != edges[p]) {
                nbr[i].push_back(edges[p]);
            }
        }
        nnbr[i] = nbr[i].size();
    }
    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            auto it = set_intersection(nbr[i].begin(), nbr[i].end(), nbr[edges[p]].begin(),
                                       nbr[edges[p]].end(), tmp.begin());
            nTriangles[i] += it - tmp.begin();
        }
    }
}

/**
 * calculate normlized weight of i->j, with maximum weight
 */
float ANALYSISBACKEND::_wt(int i, int j) {
    return max(graph->get_edge_weight(graph->find_edge(i, j)), 1.0f) / max_weight;
}

/**
 * Return vectors (number_of_neighbors(degree), weighted_triangles).
 */
void ANALYSISBACKEND::_weighted_triangles(int *nnbr, float *nweightedTriangle) {
    // this returns the geometric average weight of edges in the triangle. Also, each triangle is
    // counted twice (each direction). So you may want to divide by 2.
    max_weight = 1;
    int ne = graph->get_edge_num();
    int nv = graph->get_vertex_num();
    int *edges = graph->get_edges();
    long long *offset = graph->get_offsets();
    float *weights = graph->get_weights();
    vector<int> nbr[nv];

    for (int i = 0; i < ne; i++)
        max_weight = max(max_weight, weights[i]);
    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            if (i != edges[p]) {
                nbr[i].push_back(edges[p]);
            }
        }
        nnbr[i] = nbr[i].size();
    }
    for (int i = 0; i < nv; i++) {
        vector<int> seen, inbr, jnbr, conbr;
        int j;
        for (int x = 0; x < nbr[i].size(); x++) {
            j = nbr[i][x];
            seen.push_back(j);
            auto it = set_difference(nbr[j].begin(), nbr[j].end(), seen.begin(), seen.end(),
                                     jnbr.begin());
            it = set_intersection(nbr[i].begin(), nbr[i].end(), jnbr.begin(), jnbr.end(),
                                  conbr.begin());
            int njnbrs = it - conbr.begin();
            float wij = _wt(i, j);
            for (int y = 0, k; y < njnbrs; y++) {
                k = conbr[y];
                nweightedTriangle[i] += cbrt(wij * _wt(j, k) * _wt(k, i));
            }
        }
        nweightedTriangle[i] *= 2;
    }
}

/**
 * Return number of directed triangles in (i,k,j)
 */
int ANALYSISBACKEND::_tc(vector<int> &ip, vector<int> &is, vector<int> &jp, vector<int> &js) {
    vector<int> pp, ps, sp, ss;
    set_intersection(ip.begin(), ip.end(), jp.begin(), jp.end(), std::back_inserter(pp));
    set_intersection(ip.begin(), ip.end(), js.begin(), js.end(), std::back_inserter(ps));
    set_intersection(is.begin(), is.end(), jp.begin(), jp.end(), std::back_inserter(sp));
    set_intersection(is.begin(), is.end(), js.begin(), js.end(), std::back_inserter(ss));
    return pp.size() + ps.size() + sp.size() + ss.size();
}

/**
 * Return vectors ( total_degree, reciprocal_degree, num_of_directed_triangles).
 */
void ANALYSISBACKEND::_directed_triangles(int *degreetot, int *degreebid, int *ndiTriangle) {
    int nv = graph->get_vertex_num();
    int *edge = graph->get_edges();
    long long *offset = graph->get_offsets();
    vector<int> pred[nv], succ[nv];

    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            int j = edge[p];
            if (i == j) continue;
            pred[j].push_back(i);
            succ[i].push_back(j);
        }
    }
    for (int i = 0; i < nv; i++) {
        sort(pred[i].begin(), pred[i].end());
        sort(succ[i].begin(), succ[i].end());
        vector<int> bid;
        set_intersection(pred[i].begin(), pred[i].end(), succ[i].begin(), succ[i].end(),
                         std::back_inserter(bid));
        degreebid[i] = bid.size();
        degreetot[i] = pred[i].size() + succ[i].size();
    }
    for (int i = 0; i < nv; i++) {
        ndiTriangle[i] = 0;
        for (auto &j : pred[i])
            ndiTriangle[i] += _tc(pred[i], succ[i], pred[j], succ[j]);
        for (auto &j : succ[i])
            ndiTriangle[i] += _tc(pred[i], succ[i], pred[j], succ[j]);
    }
}

float ANALYSISBACKEND::_twc(int i, int j, vector<int> &ip, vector<int> &is, vector<int> &jp,
                            vector<int> &js) {
    float sum = 0;
    vector<int> pp, ps, sp, ss;
    set_intersection(ip.begin(), ip.end(), jp.begin(), jp.end(), std::back_inserter(pp));
    set_intersection(ip.begin(), ip.end(), js.begin(), js.end(), std::back_inserter(ps));
    set_intersection(is.begin(), is.end(), jp.begin(), jp.end(), std::back_inserter(sp));
    set_intersection(is.begin(), is.end(), js.begin(), js.end(), std::back_inserter(ss));
    for (auto &k : pp)
        sum += cbrt(_wt(i, j) * _wt(k, i) * _wt(k, j));
    for (auto &k : ps)
        sum += cbrt(_wt(i, j) * _wt(k, i) * _wt(j, k));
    for (auto &k : sp)
        sum += cbrt(_wt(i, j) * _wt(i, k) * _wt(k, j));
    for (auto &k : ss)
        sum += cbrt(_wt(i, j) * _wt(i, k) * _wt(j, k));
    return sum;
}

void ANALYSISBACKEND::_directed_weighted_triangles(int *degreetot, int *degreebid,
                                                   float *diwTriangle) {
    max_weight = 1;
    int ne = graph->get_edge_num();
    int nv = graph->get_vertex_num();
    int *edge = graph->get_edges();
    long long *offset = graph->get_offsets();
    float *weights = graph->get_weights();
    vector<int> pred[nv], succ[nv];

    for (int i = 0; i < ne; i++)
        max_weight = max(max_weight, weights[i]);

    for (int i = 0; i < nv; i++) {
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            int j = edge[p];
            if (i == j) continue;
            pred[j].push_back(i);
            succ[i].push_back(j);
        }
    }
    for (int i = 0; i < nv; i++) {
        sort(pred[i].begin(), pred[i].end());
        sort(succ[i].begin(), succ[i].end());
        vector<int> bid;
        set_intersection(pred[i].begin(), pred[i].end(), succ[i].begin(), succ[i].end(),
                         std::back_inserter(bid));
        degreebid[i] = bid.size();
        degreetot[i] = pred[i].size() + succ[i].size();
    }

    for (int i = 0; i < nv; i++) {
        diwTriangle[i] = 0;
        for (auto &j : pred[i])
            diwTriangle[i] += _twc(j, i, pred[j], succ[j], pred[i], succ[i]);
        for (auto &j : succ[i])
            diwTriangle[i] += _twc(i, j, pred[i], succ[i], pred[j], succ[j]);
    }
}

/* local clustering coefficient
 *
 * In undirected edge:
 *      clustering_coef_i = \frac{2T(i)}{deg(i)(deg(i)-1)}
 *      T(i) is the number of triangles through node i
 *
 *
 * transtivity = sum_i{clustering_coef[i]} / nv
 * dis_cluster[d] = sum_i{clustering_coef[i]} / sum{degree[i] = d}
 *
 * referrence:
 */
void ANALYSISBACKEND::calcCluster() {
    int nv = graph->get_vertex_num();
    int *degree = graph->get_degree();
    clustering_coef = (double *)(malloc(nv * sizeof(double)));
    if (!graph->is_weighted() && !graph->is_directed()) {
        // undirected and unweighted
        printf("Undirected and unweighted clustering.\n");
        int *nTriangles = (int *)(malloc(nv * sizeof(int)));
        int *nnbr = (int *)(malloc(nv * sizeof(int)));
        _triangles(nnbr, nTriangles);
        for (int i = 0; i < nv; i++) {
            if (nnbr[i] > 1)
                clustering_coef[i] = nTriangles[i] / (1.0 * nnbr[i] * (nnbr[i] - 1));
            else
                clustering_coef[i] = 0;
        }
        delete nTriangles;
        delete nnbr;
    } else if (!graph->is_directed()) {
        // weighted
        printf("weighted clustering.\n");
        int *nnbr = (int *)(malloc(nv * sizeof(int)));
        float *nweightedTriangle = (float *)(malloc(nv * sizeof(float)));
        _weighted_triangles(nnbr, nweightedTriangle);
        for (int i = 0; i < nv; i++) {
            if (nweightedTriangle[i] == 0)
                clustering_coef[i] = 0;
            else
                clustering_coef[i] = nweightedTriangle[i] / (nnbr[i] * (nnbr[i] - 1));
        }
        delete nnbr;
        delete nweightedTriangle;
    } else if (!graph->is_weighted()) {
        // directed
        printf("directed clustering.\n");
        int *degreetot = (int *)(malloc(nv * sizeof(int)));
        int *degreebid = (int *)(malloc(nv * sizeof(int)));
        int *ndiTriangle = (int *)(malloc(nv * sizeof(int)));
        _directed_triangles(degreetot, degreebid, ndiTriangle);
        for (int i = 0; i < nv; i++) {
            if (ndiTriangle[i] == 0)
                clustering_coef = 0;
            else
                clustering_coef[i] =
                    ndiTriangle[i] / ((degreetot[i] * (degreetot[i] - 1) - 2 * degreebid[i]) * 2);
        }
        delete degreetot;
        delete degreebid;
        delete ndiTriangle;
    } else {
        // directed and weighted
        printf("directed and weighted clustering.\n");
        int *degreetot = (int *)(malloc(nv * sizeof(int)));
        int *degreebid = (int *)(malloc(nv * sizeof(int)));
        float *diwTriangle = (float *)(malloc(nv * sizeof(float)));
        _directed_weighted_triangles(degreetot, degreebid, diwTriangle);
        for (int i = 0; i < nv; i++) {
            if (diwTriangle == 0)
                clustering_coef[i] = 0;
            else
                clustering_coef[i] =
                    diwTriangle[i] / ((degreetot[i] * (degreetot[i] - 1) - 2 * degreebid[i]) * 2);
        }
        delete degreetot;
        delete degreebid;
        delete diwTriangle;
    }

    for (int i = 0; i < nv; i++)
        dis_cluster[degree[i]] += clustering_coef[i] / dis_degree[degree[i]];

    printf("Clustering coef distribution:\n");
    for (auto ttt : dis_cluster)
        printf("%d: %lf\n", ttt.first, ttt.second);
    free(clustering_coef);
}

void ANALYSISBACKEND::calcTranstivity() {
    int nv = graph->get_vertex_num();
    int *nnbr = (int *)(malloc(nv * sizeof(int)));
    int *nTriangles = (int *)(malloc(nv * sizeof(int)));
    long long triangles = 0, triads = 0;
    _triangles(nnbr, nTriangles);
    for (int i = 0; i < nv; i++) {
        triangles += nTriangles[i];
        triads += nnbr[i] * (nnbr[i] - 1);
    }
    if (triads)
        transtivity = 1.0 * triangles / triads;
    else
        transtivity = 0;
    printf("transtivity: %lf\n", transtivity);
}

/*
    ANF-0

    Definition:
        individual neighbourhood function:
        IN(u, h) = | {v: v in V, dist(u, v) <= h} |
        neighbourhood function:
        N(h) = | {(u,v): u in V, v in V, dist(u, v) <= h} | or N(h) = sum{IN(u, h) | u in V}

    Algorithm:
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
    NeighborInH = vector<double>(H + 1, 0);
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
            NeighborInH[h] += IN[i][h];
        }
    }
    M = M0;
    for (int h = 1; h <= H; h++)
        printf("%lf\n", NeighborInH[h]);
}

// find k eigen values/vectors
void ANALYSISBACKEND::calcEigenval(int k) {
    vec eigval;
    mat eigvec;
    eigs_sym(eigval, eigvec, adj_mat, k, "la");
    printf("First %d eigenvalue:\n", k);
    eigval.print();
}

// find k singular values/vectors
void ANALYSISBACKEND::calcSingularval(int k) {
    mat U;
    vec s;
    mat V;
    svds(U, s, V, adj_mat, k);
    printf("First %d singular values:\n", k);
    cout << "U:" << arma::size(U) << std::endl;
    cout << "s:" << arma::size(s) << std::endl;
    cout << "V:" << arma::size(V) << std::endl;
    // printf("%d %d %d\n", U.size(), s.size(), V.size() );
    s.print();
}

/**
 * get WCCs(weakly-connected-component) of graph
 * If graph is undirected, wcc is connected component.
 * If graph is directed, wcc is conneccted component of base graph.
 */
void ANALYSISBACKEND::getWccs() {
    int nv = graph->get_vertex_num();
    int *edge = graph->get_edges();
    bool visited[nv];
    long long *offset = graph->get_offsets();
    memset(visited, 0, sizeof(visited));

    vector<int> wccV, pred[nv];
    if (graph->is_directed()) {
        for (int i = 0; i < nv; i++) {
            for (int p = offset[i]; p < offset[i + 1]; p++) {
                int j = edge[p];
                pred[j].push_back(i);
            }
        }
    }
    for (int i = 0; i < nv; i++) {
        if (visited[i]) continue;
        wccV.clear();
        wccV.push_back(i);
        queue<int> nowQ({i});
        visited[i] = 1;
        while (!nowQ.empty()) {
            int nowId = nowQ.front();
            nowQ.pop();
            if (graph->is_directed()) {
                for (auto j : pred[nowId])
                    if (!visited[j]) {
                        wccV.push_back(j);
                        nowQ.push(j);
                        visited[j] = 1;
                    }
            }
            for (int p = offset[nowId]; p < offset[nowId + 1]; p++) {
                int j = edge[p];
                if (!visited[j]) {
                    wccV.push_back(j);
                    nowQ.push(j);
                    visited[j] = 1;
                }
            }
        }
        sort(wccV.begin(), wccV.end());
        wccs.push_back(wccV);
    }
    sort(wccs.begin(), wccs.end());
}

void ANALYSISBACKEND::sccTarjan(int st, int *dfn, int *low, int *belong, int &dfs_clock) {
    int nv = graph->get_vertex_num();
    long long *offset = graph->get_offsets();
    int *edge = graph->get_edges();
    int tp = 0, top = 0;
    struct dsu {
        int nowi;
        long long edgei;
        int down;
    } stk[nv];
    stk[++tp] = (dsu){st, offset[st], 0};
    int sta[nv];

    int nowi, nxti;
    while (tp) {  // stack is not empty
        dsu &now = stk[tp];
        nowi = now.nowi;
        if (now.edgei == offset[nowi]) {  // start traverse
            dfn[nowi] = low[nowi] = ++dfs_clock;
            sta[++top] = nowi;
        }
        if (now.down)  // use low[now.down] to update low[nowi]
            low[nowi] = min(low[nowi], low[now.down]);
        if (now.edgei == offset[nowi + 1]) {  // end traverse
            if (low[nowi] == dfn[nowi]) {
                sccs.push_back({});
                int j;
                while (top > 0) {
                    j = sta[top--];
                    belong[j] = sccs.size() - 1;
                    sccs[sccs.size() - 1].push_back(j);
                    if (j == nowi) break;
                }
            }
            tp--;
            continue;
        }

        nxti = edge[now.edgei], now.edgei += 1;
        if (!dfn[nxti]) {
            now.down = nxti;  // traverse son = nxti
            stk[++tp] = (dsu){nxti, offset[nxti], 0};
        } else if (!belong[nxti])  // son traversed but doesn't belong to SCC
            low[nowi] = min(low[nowi], dfn[nxti]);
    }
}

/**
 * get SCCs(strongly-connected-component)
 */
void ANALYSISBACKEND::getSccs() {
    int nv = graph->get_vertex_num();
    int *dfn = (int *)(malloc(nv * sizeof(int)));
    int *low = (int *)(malloc(nv * sizeof(int)));
    int *belong = (int *)(malloc(nv * sizeof(int)));
    int dfs_clock = 0;

    for (int i = 0; i < nv; i++)
        dfn[i] = low[i] = belong[i] = 0;

    for (int i = 0; i < nv; i++)
        if (!dfn[i]) sccTarjan(i, dfn, low, belong, dfs_clock);

    printf("%ld SCCs:\n", sccs.size());
    for (auto scc : sccs) {
        for (auto x : scc)
            printf("%d ", x);
        printf("\n");
    }
}