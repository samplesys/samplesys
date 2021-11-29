//
// Created by AutJ on 2021/11/16.
//

#ifndef SAMPLER_INCLUDE_UTILS_RANDOM_H_
#define SAMPLER_INCLUDE_UTILS_RANDOM_H_

#include <vector>

class Random {
   private:
    int seed;

   public:
    explicit Random(int seed);
    std::vector<std::size_t> choice(const std::vector<double> &probability,
                                    std::size_t number_of_sampled, bool replace) const;
};

#endif  // SAMPLER_INCLUDE_UTILS_RANDOM_H_
