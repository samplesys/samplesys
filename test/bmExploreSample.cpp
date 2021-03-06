#include "3_party/benchmark/benchmark.h"
#include "engine/exploration_sampling/BreadthFirstSearchSampler.h"
#include "engine/exploration_sampling/DepthFirstSearchSampler.h"
#include "engine/exploration_sampling/ForestFireSampler.h"
#include "engine/exploration_sampling/RandomWalkSampler.h"
#include "engine/exploration_sampling/RandomWalkWithJumpSampler.h"
#include "engine/exploration_sampling/RandomWalkWithRestartSampler.h"
#include "engine/exploration_sampling/SnowBallSampler.h"
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
class ExploreSampleFixture : public benchmark::Fixture {
   protected:
    std::shared_ptr<Graph>       graph;
    std::shared_ptr<BaseSampler> sampler;
    double                       percent;

    void SetUp(const ::benchmark::State& state) {
        std::string input = "example/input/deezer.csv";
        // printf("Starting loading %s.\n", input.c_str());
        graph = GraphStream::readText(input, false, "%zd,%zd");
        // printf("Graph loaded.\n");
        percent = 0.1;
        // printf("%zd %zd\n", graph->number_of_nodes(), graph->number_of_edges());
    }
    void TearDown(const ::benchmark::State& state) {}
};

BENCHMARK_DEFINE_F(ExploreSampleFixture, BFS)(benchmark::State& state) {
    sampler = make_shared<BreadthFirstSearchSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, DFS)(benchmark::State& state) {
    sampler = make_shared<DepthFirstSearchSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, FF)(benchmark::State& state) {
    sampler = make_shared<ForestFireSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, SNB)(benchmark::State& state) {
    sampler = make_shared<SnowBallSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, RW)(benchmark::State& state) {
    sampler = make_shared<RandomWalkSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, RWJ)(benchmark::State& state) {
    sampler = make_shared<RandomWalkWithJumpSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_DEFINE_F(ExploreSampleFixture, RWR)(benchmark::State& state) {
    sampler = make_shared<RandomWalkWithRestartSampler>(graph->number_of_nodes() * percent);
    for (auto _ : state) {
        auto edges = sampler->sample(*graph);
    }
}

BENCHMARK_REGISTER_F(ExploreSampleFixture, BFS)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, DFS)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, FF)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, SNB)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, RW)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, RWJ)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_REGISTER_F(ExploreSampleFixture, RWR)
    ->Unit(benchmark::kMillisecond)
    ->Threads(8)
    ->Threads(4)
    ->Threads(2);

BENCHMARK_MAIN();