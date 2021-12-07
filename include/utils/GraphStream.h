/*
 * Created by AutJ on 2021/12/6.
 */

#ifndef SAMPLER_SRC_UTILS_GRAPHSTREAM_H_
#define SAMPLER_SRC_UTILS_GRAPHSTREAM_H_

#include <memory>

#include "graph/DirectedGraph.h"
#include "graph/UndirectedGraph.h"

class GraphStream {
   public:
    static std::shared_ptr<Graph> readText(const std::string &filename, bool directed = true);
    static void writeText(const std::string &filename, const std::shared_ptr<Graph> &g);

    static std::shared_ptr<Graph> readBinary(const std::string &filename, bool directed = true);
    static void writeBinary(const std::string &filename, const std::shared_ptr<Graph> &g);
};

#endif  // SAMPLER_SRC_UTILS_GRAPHSTREAM_H_
