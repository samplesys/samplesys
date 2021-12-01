//
// Created by AutJ on 2021/12/1.
//

#include "engine/exploration_sampling/DepthFirstSearchSampler.h"

#include <algorithm>
#include <stack>

using namespace std;

DepthFirstSearchSampler::DepthFirstSearchSampler(size_t number_of_nodes, size_t start_node,
                                                 int seed)
    : BaseSampler(seed), number_of_nodes(number_of_nodes), start_node(start_node) {}

vector<pair<size_t, size_t>> DepthFirstSearchSampler::sample(const DirectedGraph& g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g.get_columns();
    const auto& offsets = g.get_offsets();
    const auto& degrees = g.get_degrees();

    auto node_is_sampled = vector<bool>(g.number_of_nodes());

    size_t source_i = 0;
    if (start_node == -1 || start_node > g.number_of_nodes()) {
        source_i = random.randint(g.number_of_nodes());
    } else {
        source_i = start_node;
    }
    size_t current_sampled_nodes = 0;
    auto   _stack                = stack<pair<size_t, size_t>>();
    do {
        /* find a start node */
        while (node_is_sampled[source_i]) {
            source_i = random.randint(g.number_of_nodes());
        }
        node_is_sampled[source_i] = true;
        if (++current_sampled_nodes == number_of_nodes) break;
        if (!degrees[source_i]) continue;
        /* find a neighbour to form an edge */
        auto neighbours = g.calc_neighbours(source_i);
        random.shuffle(neighbours);
        auto it = find_if(neighbours.begin(), neighbours.end(),
                          [&node_is_sampled](const auto& e) { return !node_is_sampled[e]; });
        if (it == neighbours.end()) continue;
        /* the first node has been traversed, while the second one is to be traversed */
        _stack.push({source_i, *it});
        while (!_stack.empty() && current_sampled_nodes < number_of_nodes) {
            const auto& p = _stack.top();
            size_t      i = p.first, j = p.second;
            _stack.pop();
            if (node_is_sampled[j]) continue;
            // traverse the second node
            ret.emplace_back(i, j);
            if (++current_sampled_nodes == number_of_nodes) break;
            node_is_sampled[j] = true;
            // push the neighbours
            if (degrees[j] == 0) continue;
            auto seq = vector<size_t>(degrees[j]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            for (auto delta : seq) {
                _stack.push({j, columns[offsets[j] + delta]});
            }
        }
    } while (current_sampled_nodes < number_of_nodes);
    return ret;
}

vector<pair<size_t, size_t>> DepthFirstSearchSampler::sample(const UndirectedGraph& g) {
    auto ret = vector<pair<size_t, size_t>>();

    const auto& columns = g.get_columns();
    const auto& offsets = g.get_offsets();
    const auto& degrees = g.get_degrees();

    auto node_is_sampled = vector<bool>(g.number_of_nodes());

    size_t source_i = 0;
    if (start_node == -1 || start_node > g.number_of_nodes()) {
        source_i = random.randint(g.number_of_nodes());
    } else {
        source_i = start_node;
    }
    size_t current_sampled_nodes = 0;
    auto   _stack                = stack<pair<size_t, size_t>>();
    do {
        /* find a start node */
        while (node_is_sampled[source_i]) {
            source_i = random.randint(g.number_of_nodes());
        }
        node_is_sampled[source_i] = true;
        if (++current_sampled_nodes == number_of_nodes) break;
        if (!degrees[source_i]) continue;
        /* find a neighbour to form an edge */
        auto neighbours = g.calc_neighbours(source_i);
        random.shuffle(neighbours);
        auto it = find_if(neighbours.begin(), neighbours.end(),
                          [&node_is_sampled](const auto& e) { return !node_is_sampled[e]; });
        if (it == neighbours.end()) continue;
        /* the first node has been traversed, while the second one is to be traversed */
        _stack.push({source_i, *it});
        while (!_stack.empty() && current_sampled_nodes < number_of_nodes) {
            const auto& p = _stack.top();
            size_t      i = p.first, j = p.second;
            _stack.pop();
            if (node_is_sampled[j]) continue;
            // traverse the second node
            if (i < j) {
                ret.emplace_back(i, j);
            } else {
                ret.emplace_back(j, i);
            }
            if (++current_sampled_nodes == number_of_nodes) break;
            node_is_sampled[j] = true;
            // push the neighbours
            if (degrees[j] == 0) continue;
            auto seq = vector<size_t>(degrees[j]);
            iota(seq.begin(), seq.end(), 0);
            random.shuffle(seq);
            for (auto delta : seq) {
                _stack.push({j, columns[offsets[j] + delta]});
            }
        }
    } while (current_sampled_nodes < number_of_nodes);
    return ret;
}
