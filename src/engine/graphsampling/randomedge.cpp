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
 * The above copyright notice and this permission notice shall be included in
 *all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include "engine/graphsampling/randomedge.h"

RESAMPLE::RESAMPLE(LSGraph *_graph, int _argc, char **_argv) : EASYSAMPLE(_graph, _argc, _argv) {
    this->argc = _argc;
    this->argv = _argv;
    this->get_args(_argc, _argv);
    long long ne = this->graph->get_edge_num();
    long long nv = this->graph->get_vertex_num();
    is_chosed_edge = static_cast<bool *>(malloc(ne * sizeof(bool)));
    is_chosed_node = static_cast<bool *>(malloc(nv * sizeof(bool)));
    memset(is_chosed_edge, 0, ne * sizeof(bool));
    memset(is_chosed_node, 0, nv * sizeof(bool));
}

RESAMPLE::~RESAMPLE() {
    free(this->is_chosed_edge);
    free(this->is_chosed_node);
}

void RESAMPLE::run() {
    if (this->type == RE) {
        this->runRE();
    } else if (this->type == RNE) {
        this->runRNE();
    } else if (this->type == HRNE) {
        this->runHRNE();
    } else if (this->type == TIES) {
        this->runTIES();
    } else if (this->type == PIES) {
        this->runPIES();
    }
    this->outtofile();
}

/* random edge sampling. Edges are sampled with the same uniform probability randomly. */
void RESAMPLE::runRE(long long smp_num) {
    long long ne = this->graph->get_edge_num();
    long long chosed_num = 0;

    if (!smp_num) smp_num = (long long)(this->percent * (ne / 2));
    /* small sample */
    if (1.0 * ne < exp(1) * (ne - smp_num)) {
        long long p;
        while (chosed_num < smp_num) {
            p = mainrandom.irand(ne);
            if (!this->is_chosed_edge[p]) {
                this->is_chosed_edge[p] = this->is_chosed_edge[find_backward_edge(this->graph, p)] =
                    true;
                chosed_num++;
            }
        }
    } else {
        /* large sample */
        int cnt = 0;
        for (long long p = 0; p < ne; p++) {
            if (this->is_chosed_edge[p]) {
                cnt++;
                continue;
            }
            if (mainrandom.drand() * (ne - p - chosed_num + cnt) < 1.0 * (smp_num - chosed_num)) {
                this->is_chosed_edge[p] = this->is_chosed_edge[find_backward_edge(this->graph, p)] =
                    true;
                chosed_num++;
            }
        }
    }
}

/* random node-edge sampling. The algorithm first randomly samples a node. From
 * this node it samples an edge with a neighbor. */
void RESAMPLE::runRNE() {
    int now_vertex, p;
    int *degrees = this->graph->get_degree();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long *offsets = this->graph->get_offsets();
    long long smp_num, chosed_num = 0;

    smp_num = (long long)(this->percent * (ne / 2));
    while (chosed_num < smp_num) {
        now_vertex = mainrandom.irand(nv);
        p = offsets[now_vertex] + mainrandom.irand(degrees[now_vertex]);
        if (is_chosed_edge[p]) continue;
        is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
        chosed_num++;
    }
}

/* hybrid node-edge sampling. The algorithm alternates between two sampling methods.
 * (A) Random uniform edge sampling.
 * (B) The algorithm first randomly samples a node. From this node it samples an edge with a
 * neighbor. */
void RESAMPLE::runHRNE() {
    int now_vertex, p;
    int *degrees = this->graph->get_degree();
    long long ne = this->graph->get_edge_num();
    long long nv = this->graph->get_vertex_num();
    long long *offsets = this->graph->get_offsets();
    long long smp_num, chosed_num = 0;

    if (!this->hyb_percent) this->hyb_percent = 0.5;
    smp_num = (long long)(this->percent * (ne / 2));
    while (chosed_num < smp_num) {
        if (mainrandom.drand() < this->hyb_percent)
            p = mainrandom.irand(ne);
        else {
            now_vertex = mainrandom.irand(nv);
            p = offsets[now_vertex] + mainrandom.irand(degrees[now_vertex]);
        }
        if (is_chosed_edge[p]) continue;
        is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
        chosed_num++;
    }
}

/* random edge sampling with edge set induction. The algorithm randomly samples edges with a fixed
 * probability. Edges between nodes which are already in the sample are retained with an induction
 * step.*/
void RESAMPLE::runTIES() {
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long *offsets = this->graph->get_offsets();
    long long smp_num = 100;  // Default

    this->runRE(smp_num);

    /* induce stage (get subgraph from sampled edges) */
    int now_vertex = 0;
    for (long long p = 0; p < ne; p++) {
        if (is_chosed_edge[p]) {
            while (now_vertex < nv - 1 && p >= offsets[now_vertex + 1])
                now_vertex++;
            is_chosed_node[now_vertex] = is_chosed_node[edges[p]] = true;
        }
    }
    for (int i = 0; i < nv; i++) {
        for (int p = offsets[i]; p < offsets[i + 1]; p++)
            is_chosed_edge[p] = is_chosed_node[i] && is_chosed_node[edges[p]];
    }
}

/* random edge sampling with partial edge set induction. The algorithm randomly samples edges in a
 * streaming fashion with a fixed probability. Edges between nodes which are already in the sample
 * are retained with an induction step.*/
void RESAMPLE::runPIES() {
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long *offsets = this->graph->get_offsets();
    int *id = static_cast<int *>(malloc(ne * sizeof(int)));
    int *fr = static_cast<int *>(malloc(ne * sizeof(int)));

    shuffle(id, ne);
    int now_vertex = 0;
    for (long long p = 0; p < ne; p++) {
        while (now_vertex < nv - 1 && p >= offsets[now_vertex + 1])
            now_vertex++;
        fr[p] = now_vertex;
    }
    for (long long i = 0; i < ne; i++) {
        long long p = id[i];
        if (is_chosed_edge[p]) continue;
        if (is_chosed_node[fr[p]] && is_chosed_node[edges[p]]) {
            is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
        } else {
            if (mainrandom.drand() < this->percent / 2) { /* bidirect -> p/2 */
                is_chosed_node[fr[p]] = is_chosed_node[edges[p]] = true;
                is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
            }
        }
    }
    free(id);
    free(fr);

    // RANDOM?
    // for (long long p = 0; p < ne; p++) {
    //     if (p >= offsets[now_vertex + 1]) now_vertex++;
    //     if (is_chosed_edge[p]) continue;
    //     if (is_chosed_node[now_vertex] && is_chosed_node[edges[p]]) {
    //         is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
    //     } else {
    //         if (mainrandom.drand() < this->percent) {
    //             is_chosed_edge[p] = is_chosed_edge[find_backward_edge(graph, p)] = true;
    //             is_chosed_node[now_vertex] = is_chosed_node[edges[p]] = true;
    //         }
    //     }
    // }
}

void RESAMPLE::outtofile() {
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long *offsets = this->graph->get_offsets();
    long long chosed_node_num = 0;
    long long chosed_edge_num = 0;
    FILE *fp = fopen(this->outpath.c_str(), "w");

    memset(is_chosed_node, 0, nv * sizeof(bool));

    /* add all sampled edges and nodes to sample graph. (one-way edges only) */
    int now_vertex = 0;
    for (long long p = 0; p < ne; p++) {
        while (now_vertex < nv - 1 && p >= offsets[now_vertex + 1])
            now_vertex++;
        if (now_vertex < edges[p] && this->is_chosed_edge[p]) {
            chosed_edge_num++;
            fprintf(fp, "%d %d\n", now_vertex, edges[p]);
            if (!is_chosed_node[now_vertex]) {
                is_chosed_node[now_vertex] = true;
                chosed_node_num++;
            }
            if (!is_chosed_node[edges[p]]) {
                is_chosed_node[edges[p]] = true;
                chosed_node_num++;
            }
        }
    }
    spdlog::info("Sample {} nodes and {} edges.", chosed_node_num, chosed_edge_num);
    spdlog::info("Result saved to: {}", outpath);
}

void RESAMPLE::get_args(int argc, char **argv) {
    int a;
    if ((a = argPos(const_cast<char *>("-hybp"), argc, argv)) > 0)
        this->hyb_percent = atof(argv[a + 1]);
}