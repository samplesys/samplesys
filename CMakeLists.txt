cmake_minimum_required(VERSION 3.20)
project(sampler)

set(CMAKE_CXX_STANDARD 17)
set(3_PARTY_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/3_party)

# basic modules
add_library(sampler SHARED
        src/utils/Random.cpp
        include/utils/Random.h
        include/utils/LimitedQueue.h
        src/utils/GraphStream.cpp
        include/utils/GraphStream.h

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

        src/engine/edge_sampling/RandomEdgeSampler.cpp
        include/engine/edge_sampling/RandomEdgeSampler.h
        src/engine/edge_sampling/RandomNodeEdgeSampler.cpp
        include/engine/edge_sampling/RandomNodeEdgeSampler.h
        src/engine/edge_sampling/HybridNodeEdgeSampler.cpp
        include/engine/edge_sampling/HybridNodeEdgeSampler.h
        src/engine/edge_sampling/RandomEdgeSamplerWithInduction.cpp
        include/engine/edge_sampling/RandomEdgeSamplerWithInduction.h
        src/engine/edge_sampling/RandomEdgeSamplerWithPartialInduction.cpp
        include/engine/edge_sampling/RandomEdgeSamplerWithPartialInduction.h

        src/engine/exploration_sampling/BreadthFirstSearchSampler.cpp
        include/engine/exploration_sampling/BreadthFirstSearchSampler.h
        src/engine/exploration_sampling/DepthFirstSearchSampler.cpp
        include/engine/exploration_sampling/DepthFirstSearchSampler.h
        src/engine/exploration_sampling/SnowBallSampler.cpp
        include/engine/exploration_sampling/SnowBallSampler.h
        src/engine/exploration_sampling/ForestFireSampler.cpp
        include/engine/exploration_sampling/ForestFireSampler.h
        src/engine/exploration_sampling/RandomWalkSampler.cpp include/engine/exploration_sampling/RandomWalkSampler.h)

target_include_directories(sampler PUBLIC
        include
        )

add_library(backend SHARED
        src/graph/Graph.cpp
        include/graph/Graph.h
        src/graph/DirectedGraph.cpp
        include/graph/DirectedGraph.h
        src/graph/UndirectedGraph.cpp
        include/graph/UndirectedGraph.h
        src/utils/GraphStream.cpp
        include/utils/GraphStream.h

        src/analysis/backend.cpp
        include/analysis/Backend.h
        )

target_include_directories(backend PUBLIC
        include
        )

target_link_libraries(backend PRIVATE
        ${3_PARTY_LIB_PATH}/libopenblas.a
        ${3_PARTY_LIB_PATH}/liblapack.a
        ${3_PARTY_LIB_PATH}/libblas.a
        ${3_PARTY_LIB_PATH}/libarpack_Linux.a
        ${3_PARTY_LIB_PATH}/libsuperlu.a
        pthread
        gfortran
        )

# functional tests
add_executable(testSampler
        example/testSampler.cpp
        example/utils.cpp
        example/utils.h
        )
target_link_libraries(testSampler sampler)

add_executable(testAnalysis
        example/testAnalysis.cpp
        example/utils.cpp
        example/utils.h
        )
target_link_libraries(testAnalysis PUBLIC backend)

add_executable(testGraphStream
        example/testGraphStream.cpp
        )
target_link_libraries(testGraphStream sampler)

# Google benchmark tests
file(GLOB BM_TESTS ${CMAKE_SOURCE_DIR}/test/*.cpp)
foreach (bm_test_name ${BM_TESTS})
    STRING(REGEX REPLACE "/.*/test/" "" target_name ${bm_test_name})
    STRING(REGEX REPLACE ".cpp" "" target_name ${target_name})
    add_executable(${target_name} ${bm_test_name})
    target_link_libraries(${target_name} ${3_PARTY_LIB_PATH}/libbenchmark.a sampler pthread)
endforeach ()
