#include "analysis/Backend.h"
#include "benchmark/benchmark.h"
#include "utils/GraphStream.h"

using namespace std;
using namespace Backend;

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
class BackendFixture : public benchmark::Fixture {
   protected:
    std::shared_ptr<Graph> graph;

    void SetUp(const ::benchmark::State& state) {
        std::string input = "example/input/soc-Slashdot0902.bin";
        // printf("Starting loading %s.\n", input.c_str());
        graph = GraphStream::readBinary(input);
        // printf("Graph loaded.\n");
        // printf("%zd %zd\n", graph->number_of_nodes(), graph->number_of_edges());
    }
    void TearDown(const ::benchmark::State& state) {}
};

BENCHMARK_DEFINE_F(BackendFixture, cluster_coef)(benchmark::State& state) {
    double* cluster_coef =
        reinterpret_cast<double*>(malloc(graph->number_of_nodes() * sizeof(double)));
    for (auto _ : state) {
        get_cluster_coef(*graph, cluster_coef);
    }
}

BENCHMARK_DEFINE_F(BackendFixture, transtivity)(benchmark::State& state) {
    double trans_coef;
    for (auto _ : state) {
        get_transtivity(graph, trans_coef);
    }
}

BENCHMARK_DEFINE_F(BackendFixture, cluster_coef)(benchmark::State& state) {
    double* cluster_coef =
        reinterpret_cast<double*>(malloc(graph->number_of_nodes() * sizeof(double)));
    for (auto _ : state) {
        get_cluster_coef(*graph, cluster_coef);
    }
}

BENCHMARK_REGISTER_F(BackendFixture, cluster_coef)->Unit(benchmark::kMillisecond)->MeasureProcessCPUTime();
// ->Threads(8)
// ->Threads(4)
// ->Threads(2);

BENCHMARK_MAIN();