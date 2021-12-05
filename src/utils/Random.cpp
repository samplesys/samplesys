//
// Created by AutJ on 2021/11/16.
//

#include "utils/Random.h"

#include <cmath>

using namespace std;

Random::Random(int seed) : gen(seed) {}

vector<size_t> Random::choice(const vector<double> &probability, size_t number_of_sampled,
                              bool replace) {
    if (probability.size() < number_of_sampled) {
        number_of_sampled = probability.size();
    }
    vector<size_t> sampled;
    sampled.reserve(number_of_sampled);

    if (replace) {
        discrete_distribution<size_t> dist(probability.begin(), probability.end());
        for (auto i = 0; i < number_of_sampled; i++) {
            sampled.push_back(dist(gen));
        }
    } else {
        uniform_real_distribution<double> dist(0, 1);
        vector<double>                    vals;
        vals.reserve(probability.size());
        for (auto i : probability) {
            vals.push_back(std::pow(dist(gen), 1. / i));
        }
        vector<std::pair<int, double>> indices;
        for (size_t iter = 0; iter < vals.size(); iter++) {
            indices.emplace_back(iter, vals[iter]);
        }
        sort(indices.begin(), indices.end(),
             [](const auto &x, const auto &y) { return x.second > y.second; });
        for (auto i = 0; i < number_of_sampled; i++) {
            sampled.push_back(indices[i].first);
        }
    }
    sort(sampled.begin(), sampled.end());
    return sampled;
}