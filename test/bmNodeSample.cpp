#include "benchmark/benchmark.h"
#include "engine/edge_sampling/RandomEdgeSampler.h"
#include "engine/edge_sampling/RandomNodeEdgeSampler.h"
#include "engine/node_sampling/DegreeBasedSampler.h"
#include "engine/node_sampling/PageRankBasedSampler.h"
#include "engine/node_sampling/RandomNodeSampler.h"
#include "utils/GraphStream.h"

using namespace std;

/**
 * Example benchmark test
 */
int f(std::shared_ptr<Graph>& g) {
    int cnt = 0;
    for (size_t i = 0; i < g->number_of_nodes(); i++)
        cnt++;
    return cnt;
}

static void BM_hello(benchmark::State& state) {
    auto graph = GraphStream::readText("example/input/simple.csv");
    int  ans;
    for (auto _ : state) {
        ans = f(graph);
    }
}
BENCHMARK(BM_hello);

/**
 * Node Sample Benchmark test
 */
class NodeSampleFixture : public benchmark::Fixture {
   protected:
    std::shared_ptr<Graph>       graph;
    std::shared_ptr<BaseSampler> sampler;
    double                       percent;

    void SetUp(const ::benchmark::State& state) {
        std::string input = "example/input/soc-Slashdot0902.bin";
        // printf("Starting loading %s.\n", input.c_str());
        graph = GraphStream::readBinary(input);
        // printf("Graph loaded.\n");
        percent = 0.5;
        // printf("%zd %zd\n", graph->number_of_nodes(), graph->number_of_edges());
    }
    void TearDown(const ::benchmark::State& state) {}
};

BENCHMARK_DEFINE_F(NodeSampleFixture, RN)(benchmark::State& state) {
    sampler = make_shared<RandomNodeSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(NodeSampleFixture, RDN)(benchmark::State& state) {
    sampler = make_shared<DegreeBasedSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(NodeSampleFixture, PRN)(benchmark::State& state) {
    sampler = make_shared<PageRankBasedSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_REGISTER_F(NodeSampleFixture, RN)
    ->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime();
    // ->Threads(8)
    // ->Threads(4)
    // ->Threads(2);
BENCHMARK_REGISTER_F(NodeSampleFixture, RDN)
    ->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime();
    // ->Threads(8)
    // ->Threads(4)
    // ->Threads(2);
BENCHMARK_REGISTER_F(NodeSampleFixture, PRN)
    ->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime();
    // ->Threads(8)
    // ->Threads(4)
    // ->Threads(2);

BENCHMARK_MAIN();