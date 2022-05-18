//
// Created by AutJ on 2021/10/26.
//

#include "engine/BaseSampler.h"

BaseSampler::BaseSampler(int seed) : seed(seed), random(seed) {}

std::vector<std::pair<std::size_t, std::size_t>> BaseSampler::sample(const Graph &g) {
    auto ptr1 = dynamic_cast<const DirectedGraph *>(&g);
    if (ptr1 != nullptr) {
        return this->_sample(ptr1);
    }
    auto ptr2 = dynamic_cast<const UndirectedGraph *>(&g);
    if (ptr2 != nullptr) {
        return this->_sample(ptr2);
    }
    return {};
}
