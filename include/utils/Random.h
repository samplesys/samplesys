//
// Created by AutJ on 2021/11/16.
//

#ifndef SAMPLER_INCLUDE_UTILS_RANDOM_H_
#define SAMPLER_INCLUDE_UTILS_RANDOM_H_

#include <random>
#include <vector>

class Random {
   private:
    std::mt19937 gen;

   public:
    explicit Random(int seed);
    [[nodiscard]] std::vector<std::size_t> choice(const std::vector<double> &probability,
                                                  std::size_t                number_of_sampled,
                                                  bool                       replace) const;

    template <typename dtype>
    dtype randint(dtype low, dtype high);
    template <typename dtype>
    dtype randint(dtype high);
};

#endif  // SAMPLER_INCLUDE_UTILS_RANDOM_H_
