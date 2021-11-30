cmake_minimum_required(VERSION 3.20)
project(sampler)

set(CMAKE_CXX_STANDARD 17)

add_library(sampler SHARED
        src/utils/Random.cpp
        include/utils/Random.h

        src/graph/Graph.cpp
        include/graph/Graph.h
        src/graph/DirectedGraph.cpp
        include/graph/DirectedGraph.h
        src/graph/UndirectedGraph.cpp
        include/graph/UndirectedGraph.h

        src/engine/BaseSampler.cpp
        include/engine/BaseSampler.h

        src/engine/node_sampling/NodeSampler.cpp
        include/engine/node_sampling/NodeSampler.h
        src/engine/node_sampling/RandomNodeSampler.cpp
        include/engine/node_sampling/RandomNodeSampler.h
        src/engine/node_sampling/DegreeBasedSampler.cpp
        include/engine/node_sampling/DegreeBasedSampler.h
        src/engine/node_sampling/PageRankBasedSampler.cpp
        include/engine/node_sampling/PageRankBasedSampler.h

        src/engine/edge_sampling/EdgeSampler.cpp
        include/engine/edge_sampling/EdgeSampler.h
        src/engine/edge_sampling/RandomEdgeSampler.cpp
        include/engine/edge_sampling/RandomEdgeSampler.h
        src/engine/edge_sampling/RandomNodeEdgeSampler.cpp
        include/engine/edge_sampling/RandomNodeEdgeSampler.h
        src/engine/edge_sampling/HybridNodeEdgeSampler.cpp
        include/engine/edge_sampling/HybridNodeEdgeSampler.h
        src/engine/edge_sampling/RandomEdgeSamplerWithInduction.cpp
        include/engine/edge_sampling/RandomEdgeSamplerWithInduction.h

        src/analysis/Backend.cpp
        include/analysis/Backend.h
        )

target_include_directories(sampler PUBLIC
        include
        )

add_executable(testSampler
        example/testSampler.cpp
        example/utils.cpp
        example/utils.h
        )
target_link_libraries(testSampler sampler)