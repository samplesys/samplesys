/*
 * Created by AutJ on 2022-05-17.
 */

#include "engine/exploration_sampling/RandomWalkSampler.h"

using namespace std;

RandomWalkSampler::RandomWalkSampler(size_t number_of_nodes, size_t start_node, int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes), start_node(start_node) {}

vector<pair<size_t, size_t>> RandomWalkSampler::_sample(const DirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    size_t source;
    if (start_node > g->number_of_nodes()) {
        source = random.randint(g->number_of_nodes());
    } else {
        source = start_node;
    }

    auto   node_is_sampled       = vector<bool>(g->number_of_nodes());
    size_t current_sampled_nodes = 0;

    while (current_sampled_nodes < number_of_nodes) {
        if (!node_is_sampled[source]) {
            node_is_sampled[source] = true;
            ++current_sampled_nodes;
        }
        source = columns[offsets[source] + random.randint(degrees[source])];
    }

    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }

    return ret;
}

vector<pair<size_t, size_t>> RandomWalkSampler::_sample(const UndirectedGraph* g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g->get_columns();
    const auto& offsets = g->get_offsets();
    const auto& degrees = g->get_degrees();

    size_t source;
    if (start_node > g->number_of_nodes()) {
        source = random.randint(g->number_of_nodes());
    } else {
        source = start_node;
    }

    auto   node_is_sampled       = vector<bool>(g->number_of_nodes());
    size_t current_sampled_nodes = 0;

    while (current_sampled_nodes < number_of_nodes) {
        if (!node_is_sampled[source]) {
            node_is_sampled[source] = true;
            ++current_sampled_nodes;
        }
        source = columns[offsets[source] + random.randint(degrees[source])];
    }

    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (i < columns[loc] && node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }

    return ret;
}