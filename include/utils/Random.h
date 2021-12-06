//
// Created by AutJ on 2021/11/16.
//

#ifndef SAMPLER_INCLUDE_UTILS_RANDOM_H_
#define SAMPLER_INCLUDE_UTILS_RANDOM_H_

#include <algorithm>
#include <random>
#include <vector>

class Random {
   private:
    std::mt19937 gen;

   public:
    explicit Random(int seed);

    [[nodiscard]] std::vector<std::size_t> choice(const std::vector<double> &probability,
                                                  std::size_t number_of_sampled, bool replace);

    template <typename dtype = int>
    dtype randint(dtype low, dtype high) {
        auto dist = std::uniform_int_distribution<dtype>(low, high - 1);
        return dist(gen);
    }
    template <typename dtype = int>
    dtype randint(dtype high) {
        auto dist = std::uniform_int_distribution<dtype>(0, high - 1);
        return dist(gen);
    }

    template <typename dtype = double>
    dtype uniform(dtype low, dtype high) {
        auto dist = std::uniform_real_distribution<dtype>(low, high);
        return dist(gen);
    }

    template <typename dtype = double>
    dtype uniform() {
        auto dist = std::uniform_real_distribution<dtype>();
        return dist(gen);
    }

    template <typename dtype = int>
    void shuffle(std::vector<dtype> &x) {
        std::shuffle(x.begin(), x.end(), gen);
    }

    template <typename dtype = int>
    dtype geometric_distribution(double p) {
        auto dist = std::geometric_distribution<dtype>(p);
        return dist(gen);
    }
};

#endif  // SAMPLER_INCLUDE_UTILS_RANDOM_H_
