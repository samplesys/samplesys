#include <algorithm>
#include <cstring>
#include <queue>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"
using namespace std;

namespace Backend {
/**
 * get WCCs(weakly-connected-component) of graph
 * If graph is undirected, wcc is connected component.
 * If graph is directed, wcc is conneccted component of base graph.
 */
void get_wccs(const UndirectedGraph *g, vector<vector<size_t>> *wccs) {
    size_t      nv     = g->number_of_nodes();
    const auto &column = g->get_columns();
    const auto &offset = g->get_offsets();
    bool        visited[nv];

    memset(visited, 0, sizeof(visited));
    vector<size_t> wccV;
    for (size_t i = 0; i < nv; i++) {
        if (visited[i]) continue;
        wccV.clear();
        wccV.push_back(i);
        queue<size_t> nowQ({i});
        visited[i] = 1;
        while (!nowQ.empty()) {
            size_t nowId = nowQ.front();
            nowQ.pop();
            for (size_t p = offset[nowId]; p < offset[nowId + 1]; p++) {
                size_t j = column[p];
                if (!visited[j]) {
                    wccV.push_back(j);
                    nowQ.push(j);
                    visited[j] = 1;
                }
            }
        }
        sort(wccV.begin(), wccV.end());
        wccs->push_back(wccV);
    }
    sort(wccs->begin(), wccs->end());
}

void get_wccs(const DirectedGraph *g, vector<vector<size_t>> *wccs) {
    size_t         nv     = g->number_of_nodes();
    const auto    &column = g->get_columns();
    const auto    &offset = g->get_offsets();
    bool           visited[nv];
    vector<size_t> wccV, pred[nv];

    memset(visited, 0, sizeof(visited));
    for (size_t i = 0; i < nv; i++) {
        for (size_t p = offset[i]; p < offset[i + 1]; p++) {
            pred[column[p]].push_back(i);
        }
    }
    for (size_t i = 0; i < nv; i++) {
        if (visited[i]) continue;
        wccV.clear();
        wccV.push_back(i);
        queue<size_t> nowQ({i});
        visited[i] = 1;
        while (!nowQ.empty()) {
            size_t nowId = nowQ.front();
            nowQ.pop();
            for (auto j : pred[nowId])
                if (!visited[j]) {
                    wccV.push_back(j);
                    nowQ.push(j);
                    visited[j] = 1;
                }
            for (size_t p = offset[nowId]; p < offset[nowId + 1]; p++) {
                size_t j = column[p];
                if (!visited[j]) {
                    wccV.push_back(j);
                    nowQ.push(j);
                    visited[j] = 1;
                }
            }
        }
        sort(wccV.begin(), wccV.end());
        wccs->push_back(wccV);
    }
    sort(wccs->begin(), wccs->end());
}

void sccTarjan(const DirectedGraph *g, size_t st, size_t *dfn, size_t *low, size_t *belong,
               size_t &dfs_clock, vector<vector<size_t>> *sccs) {
    size_t      nv = g->number_of_nodes();
    size_t      tp = 0, top = 0;
    size_t      nowi, nxti;
    size_t      sta[nv];
    const auto &offset = g->get_offsets();
    const auto &edge   = g->get_edges();
    struct dsu {
        size_t nowi;
        size_t edgei;
        size_t down;
    } stk[nv];

    stk[++tp] = (dsu){st, offset[st], 0};
    while (tp) {  // stack is not empty
        dsu &now = stk[tp];
        nowi     = now.nowi;
        if (now.edgei == offset[nowi]) {  // start traverse
            dfn[nowi] = low[nowi] = ++dfs_clock;
            sta[++top]            = nowi;
        }
        if (now.down)  // use low[now.down] to update low[nowi]
            low[nowi] = min(low[nowi], low[now.down]);
        if (now.edgei == offset[nowi + 1]) {  // end traverse
            if (low[nowi] == dfn[nowi]) {
                sccs->push_back({});
                size_t j;
                while (top > 0) {
                    j         = sta[top--];
                    belong[j] = sccs->size() - 1;
                    (*sccs)[sccs->size() - 1].push_back(j);
                    if (j == nowi) break;
                }
            }
            tp--;
            continue;
        }

        nxti = edge[now.edgei], now.edgei += 1;
        if (!dfn[nxti]) {
            now.down  = nxti;  // traverse son = nxti
            stk[++tp] = (dsu){nxti, offset[nxti], 0};
        } else if (!belong[nxti])  // son traversed but doesn't belong to SCC
            low[nowi] = min(low[nowi], dfn[nxti]);
    }
}

void get_sccs(const DirectedGraph *g, vector<vector<size_t>> *sccs) {
    size_t  nv        = g->number_of_nodes();
    size_t *dfn       = reinterpret_cast<size_t *>(malloc(nv * sizeof(size_t)));
    size_t *low       = reinterpret_cast<size_t *>(malloc(nv * sizeof(size_t)));
    size_t *belong    = reinterpret_cast<size_t *>(malloc(nv * sizeof(size_t)));
    size_t  dfs_clock = 0;

    for (size_t i = 0; i < nv; i++)
        dfn[i] = low[i] = belong[i] = 0;

    for (size_t i = 0; i < nv; i++)
        if (!dfn[i]) Backend::sccTarjan(g, i, dfn, low, belong, dfs_clock, sccs);

    printf("%ld SCCs:\n", sccs->size());
    for (auto scc : *sccs) {
        for (auto x : scc)
            printf("%zd ", x);
        printf("\n");
    }
    delete dfn;
    delete low;
    delete belong;
}

}  // namespace Backend