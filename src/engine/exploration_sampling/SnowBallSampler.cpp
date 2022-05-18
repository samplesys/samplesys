#include "engine/exploration_sampling/SnowBallSampler.h"

#include <queue>

using namespace std;

SnowBallSampler::SnowBallSampler(size_t number_of_nodes, size_t start_node, size_t k, int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes), start_node(start_node), k(k) {}

vector<pair<size_t, size_t>> SnowBallSampler::_sample(const DirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    auto node_is_sampled = vector<bool>(g->number_of_nodes());

    size_t source;
    if (start_node > g->number_of_nodes()) {
        source = random.randint(g->number_of_nodes());
    } else {
        source = start_node;
    }
    size_t current_sampled_nodes = 0;
    auto   _queue                = queue<size_t>();
    do {
        // find a new start node in case the graph is not connected
        while (node_is_sampled[source]) {
            source = random.randint(g->number_of_nodes());
        }
        ++current_sampled_nodes;
        node_is_sampled[source] = true;
        _queue.push(source);

        // BFS sample no more than k neighbours
        while (!_queue.empty() && current_sampled_nodes < number_of_nodes) {
            size_t i = _queue.front();
            _queue.pop();
            if (degrees[i] == 0) continue;
            auto seq = vector<size_t>(degrees[i]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            for (size_t delta = 0; delta < seq.size() && delta < k; ++delta) {
                size_t loc = offsets[i] + seq[delta];
                auto   j   = columns[loc];
                if (node_is_sampled[j]) continue;
                ret.emplace_back(i, j);
                if (++current_sampled_nodes == number_of_nodes) {
                    break;
                }
                node_is_sampled[j] = true;
                _queue.push(j);
            }
        }
    } while (current_sampled_nodes < number_of_nodes);
    return ret;
}

vector<pair<size_t, size_t>> SnowBallSampler::_sample(const UndirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    auto node_is_sampled = vector<bool>(g->number_of_nodes());

    size_t source;
    if (start_node > g->number_of_nodes()) {
        source = random.randint(g->number_of_nodes());
    } else {
        source = start_node;
    }
    size_t current_sampled_nodes = 0;
    auto   _queue                = queue<size_t>();
    do {
        // find a new start node in case the graph is not connected
        while (node_is_sampled[source]) {
            source = random.randint(g->number_of_nodes());
        }
        ++current_sampled_nodes;
        node_is_sampled[source] = true;
        _queue.push(source);

        // BFS sample no more than k neighbours
        while (!_queue.empty() && current_sampled_nodes < number_of_nodes) {
            size_t i = _queue.front();
            _queue.pop();
            if (degrees[i] == 0) continue;
            auto seq = vector<size_t>(degrees[i]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            for (size_t delta = 0; delta < seq.size() && delta < k; ++delta) {
                size_t loc = offsets[i] + seq[delta];
                auto   j   = columns[loc];
                if (node_is_sampled[j]) continue;
                if (i < j) {
                    ret.emplace_back(i, j);
                } else {
                    ret.emplace_back(j, i);
                }
                if (++current_sampled_nodes == number_of_nodes) {
                    break;
                }
                node_is_sampled[j] = true;
                _queue.push(j);
            }
        }
    } while (current_sampled_nodes < number_of_nodes);
    return ret;
}