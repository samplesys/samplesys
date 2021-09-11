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

#ifndef KGRAPH_H
#define KGRAPH_H

#include <string.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "utils/utils.h"

using namespace std;
/** 
 * large scale graph
 * The graph storage model should be updated, in order to store large scale labeled weighted graphs.
 * Here we assume node has types, edge has types and weights.
 **/

class LSGraph {
private:


    bool        tossWeight;
    bool        tossReverse;
    
    bool        hetro;
    int         nv;
    long long   ne;
    int         type_num;

public:
    long long   *offsets;
    int         *edges;
    int         *degrees;
    int         *node_types;
    float       *weights;
    long long   *edges_r;
    
    bool weighted;
    bool temporal;  /* tempotal random walk */
    float           *edge_timestamp;    /* temporal random walk */
    LSGraph(){};
    ~LSGraph() {
        free(this->offsets);
        free(this->edges);
        free(this->degrees);
        free(this->weights);
        free(this->edges_r);
    }
    bool load_csr_graph(string network_file);
    bool load_csr_graph(int argc, char **argv);

    void print_graph_info();
    
    void init_reverse();

    long long find_edge(int src, int dst);

    int has_edge(int from, int to);

    long long get_vertex_num();

    long long get_edge_num();

    bool is_hetro();

    int* get_degree();

    long long *get_offsets();

    int *get_edges();

    int *get_types();

    int get_type_num();

    float* get_weights();

    long long *get_edges_r();

    long long get_rev_edge(int src, long long idx);

    float get_edge_weight(long long idx);
};

#endif