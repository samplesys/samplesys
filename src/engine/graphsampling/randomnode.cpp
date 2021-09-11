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

#include "engine/graphsampling/randomnode.h"

RNSAMPLE::RNSAMPLE(LSGraph *_graph, int _argc, char **_argv) : EASYSAMPLE(_graph, _argc, _argv) {
    long long nv = this->graph->get_vertex_num();
    is_chosed_node = static_cast<bool *>(malloc(nv * sizeof(bool)));
    weight = static_cast<double *>(malloc(nv * sizeof(double)));
    memset(is_chosed_node, 0, nv * sizeof(bool));
}

RNSAMPLE::~RNSAMPLE() {
    free(this->is_chosed_node);
    free(this->weight);
}

void RNSAMPLE::run() {
    if (this->type == RN) {
        long long nv = this->graph->get_vertex_num();
        for (int i = 0; i < nv; i++)
            weight[i] = 1;
    } else if (this->type == RDN) {
        long long nv = this->graph->get_vertex_num();
        int *degree = this->graph->get_degree();
        for (int i = 0; i < nv; i++)
            weight[i] = 1.0 * degree[i];
    } else if (this->type == PRN)
        this->calc_PageRank();

    if (DEBUG) {
        ofstream output("output/weight.txt", ios::out);
        long long nv = this->graph->get_vertex_num();
        for (int i = 0; i < nv; i++) {
            output << this->weight[i] << std::endl;
        }
    }

    this->Reservoir();
    this->outtofile();
}

void RNSAMPLE::calc_PageRank() {
    const int max_iteration = 100;
    const double epsilon = 1e-6;
    const double alpha = 0.85;

    int *edge = this->graph->get_edges();
    int iteration = 0;
    long long nv = this->graph->get_vertex_num();
    long long *offsets = this->graph->get_offsets();
    double *PR = static_cast<double *>(malloc(nv * sizeof(double)));
    double *PR0 = static_cast<double *>(malloc(nv * sizeof(double)));

    for (int i = 0; i < nv; i++) {
        PR0[i] = 0;
        PR[i] = 1.0 / nv;
    }
    /* O(ne * iteration) */
    while (iteration <= max_iteration && F_norm(PR0, PR, nv) > epsilon) {
        memcpy(PR0, PR, nv * sizeof(double));
        memset(PR, 0, nv * sizeof(double));
        for (int i = 0; i < nv; i++) {
            if (offsets[i] == offsets[i + 1]) {
                for (int j = 0; j < nv; j++)
                    PR[j] += PR0[i] / (nv - 1);
            } else {
                for (int p = offsets[i]; p < offsets[i + 1]; p++)
                    PR[edge[p]] += PR0[i] / (offsets[i + 1] - offsets[i]);
            }
        }
        for (int i = 0; i < nv; i++)
            PR[i] = alpha * PR[i] + (1 - alpha) / nv;
        iteration++;
    }
    memcpy(this->weight, PR, nv * sizeof(double));

    free(PR);
    free(PR0);
}

/* A-ExpJ (efficient version of A-Res Algorithm */
void RNSAMPLE::Reservoir() {
    long long nv = this->graph->get_vertex_num();
    int smp_num = (int)(this->percent * nv);
    int chosed_node = 0;
    double tot_weight = 0;
    double wi;
    double ui;
    double ki;
    double X;
    struct cmp {
        bool operator()(std::pair<int, double> a, std::pair<int, double> b) {
            return a.second > b.second;
        }
    };
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, cmp> pq;
    ofstream output("output/ki.txt", ios::out);  // DEBUG

    if (DEBUG) spdlog::info("starting A-Res.");
    
    for (int i = 0; i < nv; i++) {
        tot_weight += this->weight[i];
        is_chosed_node[i] = false;
    }
    for (int i = 0; i < smp_num; i++) {
        wi = this->weight[i] / tot_weight;
        ui = ddrand(0, 1);
        ki = log(ui) / wi;
        pq.push(std::make_pair(i, ki));
    }

    X = log(mainrandom.drand()) / pq.top().second;
    for (int i = smp_num; i < nv; i++) {
        wi = this->weight[i] / tot_weight;
        X -= wi;
        if (X <= 0) {
            ui = ddrand(exp(pq.top().second * wi), 1);
            ki = log(ui) / wi;
            pq.pop();
            pq.push(std::make_pair(i, ki));
            X = log(ddrand(0, 1)) / pq.top().second;
            if (DEBUG) output << i << ' ' << ki << std::endl;
        }
    }
    while (!pq.empty()) {
        is_chosed_node[pq.top().first] = true;
        pq.pop();
        chosed_node++;
    }
}

void RNSAMPLE::outtofile() {
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long chosed_node_num = (long long)(this->percent * nv);
    long long chosed_edge_num = 0;
    long long *offset = this->graph->get_offsets();
    FILE *fp = fopen(this->outpath.c_str(), "w");

    /* add all edegs between sampled nodes to sample gragh*/
    for (int i = 0; i < nv; i++) {
        if (!is_chosed_node[i]) continue;
        for (int p = offset[i]; p < offset[i + 1]; p++) {
            if (i < edges[p] && is_chosed_node[edges[p]]) {
                fprintf(fp, "%d %d\n", i, edges[p]);
                chosed_edge_num++;
            }
        }
    }
    spdlog::info("Sample {} nodes and {} edges.", chosed_node_num, chosed_edge_num);
    spdlog::info("Result saved to: {}", outpath);
}