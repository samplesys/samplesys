#ifndef _VALUEH_
#define _VALUEH_

#include "graph/kgraph.h"
#include "utils/util.h"
using namespace std;
int g_BitLength;

class value {
  /*
  fig_s1: 入度分布
  fig_s2: 出度分布
  fig_s3: 弱连通分量大小分布
  fig_s4: 强连通分量大小分布
  S5: Hop-plot: N(h)
  S6: Hop-plot on wCC
  S7: adj_m的第一个左奇异向量 versus rank
  S8: adj_m的奇异值 versus rank
  fig_s9: 聚类系数C
  */
 public:
  map<int, int> fig_s1, fig_s2, fig_s3, fig_s4;
  map<int, double> fig_s9;
  int SCC_NUM = 0, WCC_NUM = 0, N_NUM, E_NUM;
  value(myGraph G) {
    N_NUM = G.N_NUM;
    E_NUM = G.E_NUM;
  }
  void calcStatic(const myGraph& G) {
    calcS1S2(G);
    calcWcc(G);
    calcScc(G);
    calcNei(G);
    calcCluster(G);
  }

 private:
  stack<int> st;
  vector<int> SccCol, SccSum, WccCol, WccSum;
  void calcS1S2(const myGraph& G) {
    fig_s1.clear();
    fig_s2.clear();
    for (int i = 0; i < N_NUM; i++) fig_s1[G.inD[i]]++, fig_s2[G.outD[i]]++;
  }
  // tarjan求sCC
  void tarjan(const myGraph& G, int now, int& cnt, vector<int>& dfn,
              vector<int>& vis, vector<int>& low) {
    st.push(now);
    vis[now] = 1;
    dfn[now] = low[now] = ++cnt;
    for (int p = G.First_edge[now]; p != -1; p = G.Edge[p].next) {
      int j = G.Edge[p].to;
      if (!dfn[j]) {
        tarjan(G, j, cnt, dfn, vis, low);
        low[now] = min(low[now], low[j]);
      } else if (vis[j])
        low[now] = min(low[now], low[j]);
    }
    if (dfn[now] == low[now]) {
      SCC_NUM++;
      SccSum.push_back(0);
      int j;
      while (!st.empty()) {
        j = st.top();
        st.pop();
        vis[j] = 0;
        SccCol[j] = SCC_NUM, SccSum[SCC_NUM]++;
        if (j == now) break;
      }
    }
  }
  void calcScc(const myGraph& G) {
    int cnt = 0;
    vector<int> dfn(N_NUM, 0), vis(N_NUM, 0), low(N_NUM, 0);
    SccCol = vector<int>(N_NUM, 0);
    SccSum = vector<int>(N_NUM, 0);
    for (int i = 0; i < N_NUM; i++)
      if (!dfn[i]) tarjan(G, i, cnt, dfn, vis, low);
    fig_s4.clear();
    for (int i = 1; i <= SCC_NUM; i++) fig_s4[SccSum[i]]++;
  }

  // 并查集求wCC
  inline int find(int x, vector<int>& fa) {
    return x == fa[x] ? x : fa[x] = find(fa[x], fa);
  }
  void calcWcc(const myGraph& G) {
    vector<int> fa;
    WccCol = vector<int>(N_NUM, 0);
    WccSum = vector<int>(N_NUM, 0);
    for (int i = 0; i < N_NUM; i++) fa.push_back(i);
    int u, v;
    for (int i = 0; i < E_NUM; i++) {
      u = find(G.Edge[i].from, fa), v = find(G.Edge[i].to, fa);
      if (u == v) continue;
      fa[u] = v;
    }
    for (int i = 0; i < N_NUM; i++) {
      WccCol[i] = find(i, fa);
      WccSum[WccCol[i]]++;
    }
    // for(int i=0;i<N_NUM;i++) printf("%d ",WccCol[i]);
    for (int i = 0; i < N_NUM; i++)
      if (WccSum[i]) {
        fig_s3[WccSum[i]]++;
        WCC_NUM++;
      }
  }

  // 看成无向图进行计算local clustering coefficient
  // C[i] = 2*T[i]/( d[i]*(d[i]-1) )
  // 重复计算:T[i]=T[i]/2;
  void calcCluster(const myGraph& G) {
    vector<vector<int>> neighbor(N_NUM);
    vector<int> T(N_NUM, 0);
    int u, v;
    for (int p = 0; p < E_NUM; p++) {
      u = G.Edge[p].from, v = G.Edge[p].to;
      neighbor[u].push_back(v);
      neighbor[v].push_back(u);
    }
    vector<int> tmp(N_NUM);
    vector<int>::iterator it;
    for (int p = 0; p < E_NUM; p++) {
      u = G.Edge[p].from, v = G.Edge[p].to;
      it =
          set_intersection(neighbor[u].begin(), neighbor[u].end(),
                           neighbor[v].begin(), neighbor[v].end(), tmp.begin());
      T[u] += it - tmp.begin();
      T[v] += it - tmp.begin();
    }
    double t;
    for (int i = 0; i < N_NUM; i++)
      if (neighbor[i].size() > 1) {
        t = 1.0 * T[i] / (1.0 * neighbor[i].size() * (neighbor[i].size() - 1));
        fig_s9[G.outD[i]] += t / fig_s2[G.outD[i]];
      }
    for (auto ttt = fig_s9.begin(); ttt != fig_s9.end(); ttt++)
      printf("%lf\n", ttt->second);
  }
  // ANF: K=64, R=5, 向量长度=K*(log2(N_NUM)+R), 最多H跳
  void calcNei(const myGraph& G, int K = 64, int R = 5, int H = 100) {
    int Len = log2(N_NUM) + R;
    vector<vector<bool>> M(N_NUM, vector<bool>(K * Len, 0)),
        M0(N_NUM, vector<bool>(K * Len, 0));
    vector<vector<double>> IN(N_NUM, vector<double>(H + 1));
    vector<double> Nei(H + 1, 0);
    for (int i = 0; i < N_NUM; i++) {
      for (int j = 0; j < K; j++) {
        for (int p = 0; p < Len; p++)
          M[i][p + Len * j] = random01() < pow(0.5, p + 1);
      }
    }
    for (int h = 1; h <= H; h++) {
      for (int i = 0; i < N_NUM; i++) {
        M0[i] = M[i];
        for (int p = G.First_edge[i]; p != -1; p = G.Edge[p].next)
          for (int x = 0; x < K * Len; x++)
            M0[i][x] = M0[i][x] | M[G.Edge[p].to][x];
      }
      int b = 0;
      for (int i = 0; i < N_NUM; i++) {
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
  }
  
  void calcNeiHyperAnf(const myGraph& G){
    
  }
};

void calcTemporal() {}
#endif