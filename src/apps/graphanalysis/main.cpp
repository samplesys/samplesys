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
#include <chrono>
#include <string>

#include "apps/graphanalysis/analysis.h"
#include "engine/graphsampling/explore.h"
#include "engine/graphsampling/randomedge.h"
#include "engine/graphsampling/randomnode.h"
#include "graph/kgraph.h"
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"
#include "utils/sampleutils.h"
using namespace arma;
using namespace std;
using namespace std::chrono;

enum AnalysisType { SAMPLE, ANALYSIS, TEST };

std::string graph_path;
AnalysisType analysisType;

void args(int argc, char **argv) {
    int a = 0;
    if ((a = argPos(const_cast<char *>("-input"), argc, argv)) > 0) {
        graph_path = std::string(argv[a + 1]);
    }
    if ((a = argPos(const_cast<char *>("-sample"), argc, argv)) > 0) {
        analysisType = SAMPLE;
    } else if ((a = argPos(const_cast<char *>("-analysis"), argc, argv)) > 0) {
        analysisType = ANALYSIS;
    } else if ((a = argPos(const_cast<char *>("-test"), argc, argv)) > 0) {
        analysisType = TEST;
    }
}

int main(int argc, char **argv) {
    args(argc, argv);
    LSGraph *graph = new LSGraph();
    spdlog::info("Graph Analysis Entry:\nLoading Graph: {}", graph_path);
    std::cout << graph_path << std::endl;
    graph->load_csr_graph(argc, argv);

    if (analysisType == SAMPLE) {
        std::cout << "sampling" << std::endl;
        int a;

        auto start = high_resolution_clock::now();

        if ((a = argPos(const_cast<char *>("-rn"), argc, argv)) > 0 ||
            (a = argPos(const_cast<char *>("-rdn"), argc, argv)) > 0 ||
            (a = argPos(const_cast<char *>("-prn"), argc, argv)) > 0) {
            RNSAMPLE *sampler = new RNSAMPLE(graph, argc, argv);
            sampler->run();
            delete sampler;
        } else if ((a = argPos(const_cast<char *>("-re"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-rne"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-hrne"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-ties"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-pies"), argc, argv)) > 0) {
            RESAMPLE *sampler = new RESAMPLE(graph, argc, argv);
            sampler->run();
            delete sampler;
        } else if ((a = argPos(const_cast<char *>("-bfs"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-dfs"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-sb"), argc, argv)) > 0 ||
                   (a = argPos(const_cast<char *>("-ff"), argc, argv)) > 0) {
            EXSAMPLE *sampler = new EXSAMPLE(graph, argc, argv);
            sampler->run();
            delete sampler;
        }

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    } else if (analysisType == ANALYSIS) {
        std::cout << "analysis" << std::endl;
        ANALYSISBACKEND backend(graph, argc, argv);
        backend.calcDegree();
        backend.calcTranstivity();
        backend.calcCluster();
        backend.calcEigenval(5);
        backend.calcSingularval(5);
        backend.getWccs();
        backend.getSccs();
    } else if (analysisType == TEST) {
        std::cout << "testing:\n";
        mat A(4, 5, fill::randu);
        mat B(4, 5, fill::randu);
        cout << A * B.t() << endl;
    }

    return 0;
}