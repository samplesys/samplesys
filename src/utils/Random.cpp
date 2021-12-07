//
// Created by AutJ on 2021/11/16.
//

#include "utils/Random.h"

#include <omp.h>

#include <cmath>
#include <queue>
#include <random>

using namespace std;

Random::Random(int seed) : gen(seed) {}

vector<size_t> Random::choice(const vector<double>& probability, size_t number_of_sampled,
                              bool replace) {
    vector<size_t> sampled;
    sampled.reserve(number_of_sampled);

    if (replace) {
        discrete_distribution<size_t> dist(probability.begin(), probability.end());
        for (size_t i = 0; i < number_of_sampled; i++) {
            sampled.push_back(dist(gen));
        }
    } else {
        struct cmp {
            bool operator()(const pair<size_t, double>& a, const pair<size_t, double>& b) {
                return a.second < b.second;
            }
        };
        auto dist    = uniform_real_distribution<>(0, 1);
        auto indices = priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, cmp>();

        // O(nlog(s)), (n = size of probaility, s = number_of_sampled)
        for (size_t i = 0; i < min(probability.size(), number_of_sampled); ++i) {
            indices.emplace(i, pow(dist(gen), 1.0 / i));
        }
        for (size_t i = min(probability.size(), number_of_sampled); i < probability.size(); ++i) {
            auto val = pow(dist(gen), 1.0 / i);
            if (val > indices.top().second) {
                indices.pop();
                indices.emplace(i, val);
            }
        }
        for (size_t i = 0; i < number_of_sampled; i++) {
            sampled.push_back(indices.top().first);
            indices.pop();
        }
    }
    return sampled;
}