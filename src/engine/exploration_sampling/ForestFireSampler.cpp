//
// Created by AutJ on 2021/12/1.
//

#include "engine/exploration_sampling/ForestFireSampler.h"

#include <algorithm>

#include "utils/LimitedQueue.h"

using namespace std;

ForestFireSampler::ForestFireSampler(size_t number_of_nodes, size_t start_node, double p,
                                     size_t max_visited_nodes_backlog, size_t restart_hop_size,
                                     int seed)
    : BaseSampler(seed),
      number_of_nodes(number_of_nodes),
      start_node(start_node),
      p(p),
      max_visited_nodes(max_visited_nodes_backlog),
      restart_hop_size(restart_hop_size) {}

vector<pair<size_t, size_t>> ForestFireSampler::_sample(const DirectedGraph* g) {
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
    auto   visited_nodes         = LimitedQueue<size_t>(max_visited_nodes);
    auto   tinder                = queue<size_t>();
    size_t current_sampled_nodes = 0;

    while (current_sampled_nodes < number_of_nodes) {
        // select a source from unsampled nodes
        while (node_is_sampled[source]) {
            source = random.randint(g->number_of_nodes());
        }

        // sample the node when popping out
        tinder.push(source);

        // set a fire from source
        while (current_sampled_nodes < number_of_nodes) {
            if (tinder.empty()) {
                for (size_t i = 0, m = min(restart_hop_size, visited_nodes.size()); i < m; ++i) {
                    tinder.push(visited_nodes.front());
                    visited_nodes.pop();
                }
                if (tinder.empty()) {
                    // the fire went out, need to select a new source
                    break;
                }
            }
            size_t i = tinder.front();
            tinder.pop();
            node_is_sampled[i] = true;
            if (++current_sampled_nodes == number_of_nodes) {
                break;
            }
            auto   score      = random.geometric_distribution<size_t>(p);
            auto   neighbours = g->calc_neighbours(i);
            size_t unvisited =
                count_if(neighbours.begin(), neighbours.end(),
                         [&node_is_sampled](const auto& e) { return !node_is_sampled[e]; });
            auto count = min(score, unvisited);
            auto seq   = vector<size_t>(degrees[i]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            size_t temp_sampled_nodes = 0;
            for (auto delta : seq) {
                size_t loc = offsets[i] + delta;
                size_t j   = columns[loc];
                if (node_is_sampled[j]) continue;
                if (++temp_sampled_nodes < count) {
                    tinder.push(j);
                } else {
                    visited_nodes.push(j);
                }
            }
        }
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

vector<pair<size_t, size_t>> ForestFireSampler::_sample(const UndirectedGraph* g) {
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
    auto   visited_nodes         = LimitedQueue<size_t>(max_visited_nodes);
    auto   tinder                = queue<size_t>();
    size_t current_sampled_nodes = 0;

    while (current_sampled_nodes < number_of_nodes) {
        // select a source from unsampled nodes
        while (node_is_sampled[source]) {
            source = random.randint(g->number_of_nodes());
        }

        // sample the node when popping out
        tinder.push(source);

        // set a fire from source
        while (current_sampled_nodes < number_of_nodes) {
            if (tinder.empty()) {
                for (size_t i = 0, m = min(restart_hop_size, visited_nodes.size()); i < m; ++i) {
                    tinder.push(visited_nodes.front());
                    visited_nodes.pop();
                }
                if (tinder.empty()) {
                    // the fire went out, need to select a new source
                    break;
                }
            }
            size_t i = tinder.front();
            tinder.pop();
            node_is_sampled[i] = true;
            if (++current_sampled_nodes == number_of_nodes) {
                break;
            }
            auto   score      = random.geometric_distribution<size_t>(p);
            auto   neighbours = g->calc_neighbours(i);
            size_t unvisited =
                count_if(neighbours.begin(), neighbours.end(),
                         [&node_is_sampled](const auto& e) { return !node_is_sampled[e]; });
            auto count = min(score, unvisited);
            auto seq   = vector<size_t>(degrees[i]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            size_t temp_sampled_nodes = 0;
            for (auto delta : seq) {
                size_t loc = offsets[i] + delta;
                size_t j   = columns[loc];
                if (node_is_sampled[j]) continue;
                if (++temp_sampled_nodes < count) {
                    tinder.push(j);
                } else {
                    visited_nodes.push(j);
                }
            }
        }
    }
    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        if (!node_is_sampled[i]) continue;
        for (auto loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            if (i < offsets[loc] && node_is_sampled[columns[loc]]) {
                ret.emplace_back(i, columns[loc]);
            }
        }
    }
    return ret;
}