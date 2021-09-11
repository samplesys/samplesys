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

#ifndef EXPLORE_H
#define EXPLORE_H

#include <queue>
#include <stack>
#include <random>

#include "engine/graphsampling/sample.h"
#include "graph/kgraph.h"
#include "spdlog/spdlog.h"
#include "utils/utils.h"
#include "utils/sampleutils.h"

class EXSAMPLE : public EASYSAMPLE {
   public:
    EXSAMPLE(LSGraph *_graph, int argc, char **argv);
    ~EXSAMPLE();

    void run();
    void runBFS();
    void runDFS();
    void runSB();
    void runFF();

   private:
    void get_args(int argc, char **argv);
    void outtofile();

    long long start_node;
    
    int max_neighbor; // Snowball Sampling expanding rate

    double forward_p; // Forest Fire forward burning probability
};

#endif