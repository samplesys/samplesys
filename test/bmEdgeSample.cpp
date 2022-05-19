#include "3_party/benchmark/benchmark.h"
#include "engine/edge_sampling/HybridNodeEdgeSampler.h"
#include "engine/edge_sampling/RandomEdgeSampler.h"
#include "engine/edge_sampling/RandomEdgeSamplerWithInduction.h"
#include "engine/edge_sampling/RandomEdgeSamplerWithPartialInduction.h"
#include "engine/edge_sampling/RandomNodeEdgeSampler.h"
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
// BENCHMARK(BM_hello);

/**
 * Node Sample Benchmark test
 */
class EdgeSampleFixture : public benchmark::Fixture {
   protected:
    std::shared_ptr<Graph>       graph;
    std::shared_ptr<BaseSampler> sampler;
    double                       percent;

    void SetUp(const ::benchmark::State& state) {
        std::string input = "example/input/facebook_edges.csv";
        // printf("Starting loading %s.\n", input.c_str());
        graph = GraphStream::readText(input, false, "%zd,%zd");
        // printf("Graph loaded.\n");
        percent = 0.1;
        // printf("%zd %zd\n", graph->number_of_nodes(), graph->number_of_edges());
    }
    void TearDown(const ::benchmark::State& state) {}
};

BENCHMARK_DEFINE_F(EdgeSampleFixture, RE)(benchmark::State& state) {
    sampler = make_shared<RandomEdgeSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(EdgeSampleFixture, RNE)(benchmark::State& state) {
    sampler = make_shared<RandomNodeEdgeSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(EdgeSampleFixture, HRNE)(benchmark::State& state) {
    sampler = make_shared<HybridNodeEdgeSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(EdgeSampleFixture, TIES)(benchmark::State& state) {
    sampler = make_shared<RandomEdgeSamplerWithInduction>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(EdgeSampleFixture, PIES)(benchmark::State& state) {
    sampler =
        make_shared<RandomEdgeSamplerWithPartialInduction>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_REGISTER_F(EdgeSampleFixture, RE)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(EdgeSampleFixture, RNE)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(EdgeSampleFixture, HRNE)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(EdgeSampleFixture, TIES)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(EdgeSampleFixture, PIES)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_MAIN();