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

#include <map>
#include <set>
#include <vector>

#include "graph/kgraph.h"
class ANALYSISBACKEND {
   private:
    void get_args();
    int argc;
    char **argv;
    LSGraph *graph;
    /* dis_degree: 度分布 degree distribution
       dis_cluster: 聚类系数clustering coefficient distribution
       fig_s3: 弱连通分量大小分布
       fig_s4: 强连通分量大小分布
       S5: Hop-plot: N(h)
       S6: Hop-plot on wCC
       S7: adj_m的第一个左奇异向量 versus rank
       S8: adj_m的奇异值 versus rank
    */
    map<int, double> dis_cluster;
    map<int, int> dis_degree;
    double assortativty_coef, *clustering_coef;

   public:
    ANALYSISBACKEND(LSGraph *_graph, int args, char **argv);
    ~ANALYSISBACKEND();
    void calcDegree();
    void calcCluster();
    void calcNei(int K, int R, int H);
    void work();
};