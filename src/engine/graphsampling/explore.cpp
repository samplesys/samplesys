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

#include "engine/graphsampling/explore.h"

EXSAMPLE::EXSAMPLE(LSGraph *_graph, int _argc, char **_argv) : EASYSAMPLE(_graph, _argc, _argv) {
    this->argc = _argc;
    this->argv = argv;
    this->get_args(_argc, _argv);
    long long ne = this->graph->get_edge_num();
    long long nv = this->graph->get_vertex_num();
    is_chosed_edge = static_cast<bool *>(malloc(ne * sizeof(bool)));
    is_chosed_node = static_cast<bool *>(malloc(nv * sizeof(bool)));
    memset(is_chosed_edge, 0, ne * sizeof(bool));
    memset(is_chosed_node, 0, nv * sizeof(bool));
}

EXSAMPLE::~EXSAMPLE() {
    free(is_chosed_edge);
    free(is_chosed_node);
}

void EXSAMPLE::get_args(int argc, char **argv) {
    int a;
    if ((a = argPos(const_cast<char *>("-start"), argc, argv)) > 0)
        this->start_node = atoll(argv[a + 1]);
    else
        this->start_node = -1;
    if ((a = argPos(const_cast<char *>("-maxneighbor"), argc, argv)) > 0)
        this->max_neighbor = atoll(argv[a + 1]);
    else
        this->max_neighbor = 50;
    if ((a = argPos(const_cast<char *>("-forward"), argc, argv)) > 0)
        this->forward_p = atof(argv[a + 1]);
    else
        this->forward_p = 0.4;
}

void EXSAMPLE::run() {
    if (this->type == BFS) {
        this->runBFS();
    } else if (this->type == DFS) {
        this->runDFS();
    } else if (this->type == SB) {
        this->runSB();
    } else if (this->type == FF) {
        this->runFF();
    }
    this->outtofile();
}

void EXSAMPLE::runBFS() {
    int chosed_num = 0, source, neighbor, *id;
    int *edges = this->graph->get_edges();
    int *degrees = this->graph->get_degree();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long smp_num = (long long)(this->percent * nv);
    long long *offsets = this->graph->get_offsets();

    source = start_node == -1 ? mainrandom.irand(nv) : start_node;
    id = static_cast<int *>(malloc(ne * sizeof(int)));
    while (chosed_num < smp_num) {
        std::queue<int> _queue;
        while (is_chosed_node[source])
            source = mainrandom.irand(nv);
        _queue.push(source);
        if (DEBUG) spdlog::info("BFS starting with node {}", source);
        is_chosed_node[source] = true;
        if (++chosed_num == smp_num) break;
        while (!_queue.empty() && chosed_num < smp_num) {
            source = _queue.front();
            _queue.pop();
            shuffle(id, degrees[source]);
            for (int i = 0; i < degrees[source]; i++) {
                int p = id[i] + offsets[source];
                neighbor = edges[p];
                if (!is_chosed_node[neighbor]) {
                    is_chosed_node[neighbor] = true;
                    is_chosed_edge[p] = is_chosed_edge[find_backward_edge(this->graph, p)] = true;
                    if (++chosed_num == smp_num) break;
                    _queue.push(neighbor);
                }
            }
        }
    }
    free(id);
}

void EXSAMPLE::runDFS() {
    int *degrees = this->graph->get_degree();
    int *edges = this->graph->get_edges();
    int chosed_num = 0, source_edge, source_node, size, *id;
    long long *offsets = this->graph->get_offsets();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long smp_num = (long long)(this->percent * nv);
    long long start_edge;

    source_node = start_node == -1 ? mainrandom.irand(nv) : start_node;
    id = static_cast<int *>(malloc(ne * sizeof(int)));
    while (chosed_num < smp_num) {
        while (is_chosed_node[source_node])
            source_node = mainrandom.irand(nv);
        is_chosed_node[source_node] = true;
        if (++chosed_num == smp_num) break;
        if (!degrees[source_node]) continue;
        do {
            start_edge = mainrandom.irand(degrees[source_node]) + offsets[source_node];
        } while (is_chosed_edge[start_edge]);
        std::stack<long long> _stack;
        _stack.push(start_edge);
        is_chosed_edge[start_edge] = is_chosed_edge[find_backward_edge(graph, start_edge)] = true;
        if (DEBUG) spdlog::info("DFS starting with node :{}", source_node);
        while (!_stack.empty() && chosed_num < smp_num) {
            source_edge = _stack.top();
            _stack.pop();
            source_node = edges[source_edge];
            if (is_chosed_node[source_node]) continue;
            is_chosed_node[source_node] = true;
            is_chosed_edge[source_edge] = is_chosed_edge[find_backward_edge(graph, source_edge)] =
                true;
            if (++chosed_num == smp_num) break;
            size = degrees[source_node];
            shuffle(id, size);  // time-consuming
            for (int p = 0; p < size; p++)
                _stack.push(id[p] + offsets[source_node]);
        }
    }

    free(id);
}

/* snow ball search. Starting from a source node the algorithm places a fixed number of neighbors in
 * a queue of nodes to explore. The expansion goes on until the target number of sampled vertices is
 * reached.*/
void EXSAMPLE::runSB() {
    int *degrees = this->graph->get_degree();
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long smp_num = (long long)(this->percent * nv);
    long long *offsets = this->graph->get_offsets();
    int chosed_num = 0, source, neighbor, *id;

    source = start_node == -1 ? mainrandom.irand(nv) : start_node;
    id = static_cast<int *>(malloc(ne * sizeof(int)));
    while (chosed_num < smp_num) {
        while (is_chosed_node[source])
            source = mainrandom.irand(nv);
        std::queue<int> _queue;
        _queue.push(source);
        is_chosed_node[source] = true;
        if (DEBUG) spdlog::info("SB starting with node {}", source);
        if (++chosed_num == smp_num) break;
        while (!_queue.empty() && chosed_num < smp_num) {
            source = _queue.front();
            _queue.pop();
            shuffle(id, degrees[source]);
            if (DEBUG) spdlog::info("now vertex: {}, degrees: {}", source, degrees[source]);
            for (int i = 0; i < std::min(this->max_neighbor, degrees[source]); i++) {
                int p = id[i] + offsets[source];
                neighbor = edges[p];
                if (!is_chosed_node[neighbor]) {
                    is_chosed_node[neighbor] = true;
                    is_chosed_edge[p] = is_chosed_edge[find_backward_edge(this->graph, p)] = true;
                    _queue.push(neighbor);
                    if (++chosed_num == smp_num) break;
                }
            }
        }
        if (DEBUG) spdlog::info("{} nodes chosed", chosed_num);
    }

    free(id);
}

/* forest fire sampling. The procedure is a stochastic snowball sampling method where the expansion
 * is proportional to the burning probability. */
void EXSAMPLE::runFF() {
    int chosed_num = 0, source_node, score, count, *id;
    int *edges = this->graph->get_edges();
    int *degrees = this->graph->get_degree();
    long long nv = this->graph->get_vertex_num();
    long long ne = this->graph->get_edge_num();
    long long *offsets = this->graph->get_offsets();
    long long smp_num = (long long)(this->percent * nv);
    std::default_random_engine _generator;
    std::geometric_distribution<int> _geo_distribution(this->forward_p);

    source_node = start_node == -1 ? mainrandom.irand(nv) : start_node;
    id = static_cast<int *>(malloc(ne * sizeof(int)));
    while (chosed_num < smp_num) {
        while (is_chosed_node[source_node])
            source_node = mainrandom.irand(nv);
        is_chosed_node[source_node] = true;
        if (DEBUG) spdlog::info("start new fire on node {}.", source_node);
        if (++chosed_num == smp_num) break;
        std::queue<int> _queue;
        _queue.push(source_node);
        /* expand fire */
        while (!_queue.empty() && chosed_num < smp_num) {
            source_node = _queue.front();
            _queue.pop();
            int unvis_num = 0, tmp_chosed = 0, size = degrees[source_node];
            score = _geo_distribution(_generator) + 1;
            for (int p = offsets[source_node]; p < offsets[source_node + 1]; p++)
                if (!is_chosed_node[edges[p]]) unvis_num++;
            count = min(unvis_num, score);
            if (DEBUG)
                spdlog::info("node: {}, score: {}, unvisited: {}, count: {}", source_node, score,
                             unvis_num, count);
            /* burn {count} neighbours of source_node */
            if (!count) continue;
            shuffle(id, degrees[source_node]);
            for (int i = 0; i < size; i++) {
                int p = id[i] + offsets[source_node];
                if (is_chosed_node[edges[p]]) continue;
                is_chosed_node[edges[p]] = true;
                _queue.push(edges[p]);
                if (++chosed_num == smp_num || ++tmp_chosed == count) break;
            }

            // meaningless
            // int skipped = 0;
            // for (int p = 0; p < size; p++) {
            //     neighbor_node = edges[p + offsets[source_node]];
            //     if (is_chosed_node[neighbor_node]) {
            //         skipped++;
            //         continue;
            //     }
            //     if (mainrandom.drand() <
            //         1.0 * (count - tmp_chosed) / (size - p - unvis_num + skipped)) {
            //         is_chosed_node[neighbor_node] = true;
            //         _queue.push(neighbor_node);
            //         if (DEBUG) spdlog::info("choose {}", neighbor_node);
            //         tmp_chosed++;
            //         chosed_num++;
            //         if (chosed_num == smp_num || tmp_chosed == count) break;
            //     }
            // }
        }
        if (DEBUG) spdlog::info("{} nodes chosen.", chosed_num);
    }
    free(id);
}

void EXSAMPLE::outtofile() {
    int *edges = this->graph->get_edges();
    long long nv = this->graph->get_vertex_num();
    long long *offsets = this->graph->get_offsets();
    long long chosed_node_num = (long long)(this->percent * nv);
    long long chosed_edge_num = 0;
    FILE *fp = fopen(this->outpath.c_str(), "w");

    /* add all edegs between sampled nodes to sample gragh*/
    for (int i = 0; i < nv; i++) {
        if (!is_chosed_node[i]) continue;
        for (int p = offsets[i]; p < offsets[i + 1]; p++) {
            if (i < edges[p] && is_chosed_node[edges[p]]) {
                fprintf(fp, "%d %d\n", i, edges[p]);
                chosed_edge_num++;
            }
        }
    }
    spdlog::info("Sample {} nodes and {} edges.", chosed_node_num, chosed_edge_num);
    spdlog::info("Result saved to: {}", outpath);
}