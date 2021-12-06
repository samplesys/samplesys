//
// Created by AutJ on 2021/11/16.
//

#include "utils/Random.h"

#include <omp.h>

#include <algorithm>
#include <cmath>
#include <queue>
#include <random>
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
        for (size_t i = 0; i < number_of_sampled; i++) {
            sampled.push_back(dist(gen));
        }
    } else {
        struct cmp {
            bool operator()(std::pair<size_t, double> a, std::pair<size_t, double> b) {
                return a.second < b.second;
            }
        };
        uniform_real_distribution<> dist(0, 1);
        priority_queue<std::pair<size_t, double>, vector<std::pair<size_t, double>>, cmp> indices;
        size_t vsize = probability.size();

        // O(nlog(s)), (n = size of probaility, s = number_of_sampled)
        for (size_t i = 0; i < std::min(vsize, number_of_sampled); i++)
            indices.push(make_pair(i, std::pow(dist(gen), 1.0 / i)));
        for (size_t i = std::min(vsize, number_of_sampled); i < number_of_sampled; i++) {
            auto new_pair = make_pair(i, std::pow(dist(gen), 1.0 / i));
            if (new_pair.second > indices.top().second) {
                indices.pop();
                indices.push(new_pair);
            }
        }
        for (size_t i = 0; i < number_of_sampled; i++) {
            sampled.push_back(indices.top().first);
            indices.pop();
        }

        //  O(nlog(n)), (n = size of probaility)

        // uniform_real_distribution<double> dist(0, 1);
        // vector<double>                    vals;
        // vals.reserve(probability.size());
        // for (auto i : probability) {
        //     vals.push_back(std::pow(dist(gen), 1. / i));
        // }
        // vector<std::pair<int, double>> indices;
        // for (size_t iter = 0; iter < vals.size(); iter++) {
        //     indices.emplace_back(iter, vals[iter]);
        // }
        // sort(indices.begin(), indices.end(),
        //      [](const auto &x, const auto &y) { return x.second > y.second; });
        // for (auto i = 0; i < number_of_sampled; i++) {
        //     sampled.push_back(indices[i].first);
        // }
    }
    return sampled;
}