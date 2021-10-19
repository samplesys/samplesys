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

#include "engine/graphsampling/sample.h"

EASYSAMPLE::EASYSAMPLE(LSGraph *_graph, int _argc, char **_argv) {
    srand((unsigned)time(NULL));
    this->graph = _graph;
    this->argc = _argc;
    this->argv = _argv;
    this->get_args(argc, argv);
}

void EASYSAMPLE::get_args(int argc, char **argv) {
    int a = 0;
    /* Sample models */
    if ((a = argPos(const_cast<char *>("-rn"), argc, argv)) > 0) {
        type = RN;
    } else if ((a = argPos(const_cast<char *>("-rdn"), argc, argv)) > 0) {
        type = RDN;
    } else if ((a = argPos(const_cast<char *>("-prn"), argc, argv)) > 0) {
        type = PRN;
    } else if ((a = argPos(const_cast<char *>("-re"), argc, argv)) > 0) {
        type = RE;
    } else if ((a = argPos(const_cast<char *>("-rne"), argc, argv)) > 0) {
        type = RNE;
    } else if ((a = argPos(const_cast<char *>("-hrne"), argc, argv)) > 0) {
        type = HRNE;
    } else if ((a = argPos(const_cast<char *>("-ties"), argc, argv)) > 0) {
        type = TIES;
    } else if ((a = argPos(const_cast<char *>("-pies"), argc, argv)) > 0) {
        type = PIES;
    } else if ((a = argPos(const_cast<char *>("-bfs"), argc, argv)) > 0) {
        type = BFS;
    } else if ((a = argPos(const_cast<char *>("-dfs"), argc, argv)) > 0) {
        type = DFS;
    } else if ((a = argPos(const_cast<char *>("-sb"), argc, argv)) > 0) {
        type = SB;
    } else if ((a = argPos(const_cast<char *>("-ff"), argc, argv)) > 0) {
        type = FF;
    }

    /* Sampling size */
    if ((a = argPos(const_cast<char *>("-percent"), argc, argv)) > 0)
        this->percent = atof(argv[a + 1]);
    else
        this->percent = 0.5;

    /* Sample result */
    if ((a = argPos(const_cast<char *>("-output"), argc, argv)) > 0) {
        this->outpath = std::string(argv[a + 1]);
    } else
        this->outpath = std::string("output/sample.txt");

    if ((a = argPos(const_cast<char *>("-debug"), argc, argv)) > 0) {
        this->DEBUG = 1;
    } else
        this->DEBUG = 0;
}