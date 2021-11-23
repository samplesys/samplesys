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

#include <armadillo>
#include <cmath>
#include <map>
#include <queue>
#include <set>
#include <vector>

#include "graph/kgraph.h"
using namespace arma;
class ANALYSISBACKEND {
   private:
    void get_args();
    int argc;
    char **argv;
    LSGraph *graph;
    sp_mat adj_mat;
    /* dis_degree: 度分布 degree distribution
       clustering_coef: 聚类系数
       transtivity: 图的联通性（三角）
       dis_cluster: 聚类系数分布 clustering coefficient distribution
       assortativty_coef: 度数的相关度 degree correlation coefficient
       adj_matd的k个左奇异向量/奇异值 (versus rank)
       adj_mat的k个特征向量/特征值(versus rank)

       fig_s3: 弱连通分量大小分布
       fig_s4: 强连通分量大小分布
       S5: Hop-plot: N(h)S
       S6: Hop-plot on wCC
    */
    map<int, double> dis_cluster;
    map<int, int> dis_degree;
    double assortativty_coef;
    double *clustering_coef, transtivity;
    vector<double> NeighborInH;
    float max_weight;
    vector<vector<int>> wccs, sccs;
    void _triangles(int *nneighbor, int *ntriangles);
    void _weighted_triangles(int *nneighbor, float *ntriangles);
    void _directed_triangles(int *degreetot, int *degreebid, int *ndiTriangle);
    void _directed_weighted_triangles(int *degreetot, int *degreebid, float *diwTriangle);
    int _tc(vector<int> &ip, vector<int> &is, vector<int> &jp, vector<int> &js);
    float _wt(int i, int j);
    float _twc(int i, int j, vector<int> &ip, vector<int> &is, vector<int> &jp, vector<int> &js);

   public:
    ANALYSISBACKEND(LSGraph *_graph, int args, char **argv);
    ~ANALYSISBACKEND();
    void transGraphToMat();
    void calcDegree();
    void calcCluster();
    void calcTranstivity();
    void calcNei(int K, int R, int H);
    void calcEigenval(int k);
    void calcSingularval(int k);
    void getWccs();
    void getSccs();
};