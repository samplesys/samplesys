#include "analysis/cluster.h"

using namespace std;

namespace Backend {

size_t _tc(vector<size_t> &ip, vector<size_t> &is, vector<size_t> &jp, vector<size_t> &js) {
    vector<int> pp, ps, sp, ss;
    set_intersection(ip.begin(), ip.end(), jp.begin(), jp.end(), std::back_inserter(pp));
    set_intersection(ip.begin(), ip.end(), js.begin(), js.end(), std::back_inserter(ps));
    set_intersection(is.begin(), is.end(), jp.begin(), jp.end(), std::back_inserter(sp));
    set_intersection(is.begin(), is.end(), js.begin(), js.end(), std::back_inserter(ss));
    return pp.size() + ps.size() + sp.size() + ss.size();
}

void _directed_triangles(const DirectedGraph &g, size_t *degreetot, size_t *degreebid,
                         size_t *ndiTriangle) {
    size_t         nv     = g.number_of_nodes();
    const auto    &column = g.get_columns();
    const auto    &offset = g.get_offsets();
    vector<size_t> pred[nv], succ[nv];

    for (size_t i = 0; i < nv; i++) {
        for (size_t p = offset[i]; p < offset[i + 1]; p++) {
            size_t j = column[p];
            if (i == j) continue;
            pred[j].push_back(i);
            succ[i].push_back(j);
        }
    }
#pragma omp parallel for
    for (size_t i = 0; i < nv; i++) {
        sort(pred[i].begin(), pred[i].end());
        sort(succ[i].begin(), succ[i].end());
        vector<size_t> bid;
        set_intersection(pred[i].begin(), pred[i].end(), succ[i].begin(), succ[i].end(),
                         std::back_inserter(bid));
        degreebid[i] = bid.size();
        degreetot[i] = pred[i].size() + succ[i].size();
    }
#pragma omp parallel for
    for (size_t i = 0; i < nv; i++) {
        ndiTriangle[i] = 0;
        for (auto &j : pred[i])
            ndiTriangle[i] += _tc(pred[i], succ[i], pred[j], succ[j]);
        for (auto &j : succ[i])
            ndiTriangle[i] += _tc(pred[i], succ[i], pred[j], succ[j]);
    }
}

void _get_cluster_coef(const DirectedGraph &g, double *cluster_coef) {
    // unweighted graph
    size_t  nv          = g.number_of_nodes();
    size_t *degreetot   = (size_t *)(malloc(nv * sizeof(size_t)));
    size_t *degreebid   = (size_t *)(malloc(nv * sizeof(size_t)));
    size_t *ndiTriangle = (size_t *)(malloc(nv * sizeof(size_t)));
    _directed_triangles(g, degreetot, degreebid, ndiTriangle);
#pragma omp parallel for
    for (size_t i = 0; i < nv; i++) {
        if (ndiTriangle[i] == 0)
            cluster_coef = 0;
        else
            cluster_coef[i] =
                ndiTriangle[i] / ((degreetot[i] * (degreetot[i] - 1) - 2 * degreebid[i]) * 2);
    }
    delete degreetot;
    delete degreebid;
    delete ndiTriangle;
}

void _get_cluster_coef(const UndirectedGraph &g, double *cluster_coef) {
    // unweighted graph
    size_t  nv         = g.number_of_nodes();
    size_t *nTriangles = (size_t *)(malloc(nv * sizeof(size_t)));
    size_t *nnbr       = (size_t *)(malloc(nv * sizeof(size_t)));
    Backend::_triangles(g, nnbr, nTriangles);

#pragma omp parallel for
    for (size_t i = 0; i < nv; i++) {
        if (nnbr[i] > 1)
            cluster_coef[i] = nTriangles[i] / (1.0 * nnbr[i] * (nnbr[i] - 1));
        else
            cluster_coef[i] = 0;
    }
    delete nTriangles;
    delete nnbr;
}

void get_cluster_coef(const Graph &g, double *cluster_coef) {
    auto ptr1 = dynamic_cast<const DirectedGraph *>(&g);
    if (ptr1 != nullptr) Backend::_get_cluster_coef(*ptr1, cluster_coef);
    auto ptr2 = dynamic_cast<const UndirectedGraph *>(&g);
    if (ptr2 != nullptr) Backend::_get_cluster_coef(*ptr2, cluster_coef);
}

}  // namespace Backend
