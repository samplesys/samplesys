//
// Created by AutJ on 2021/12/1.
//

#ifndef SAMPLER_SRC_UTILS_LIMITEDQUEUE_H_
#define SAMPLER_SRC_UTILS_LIMITEDQUEUE_H_

#include <queue>

template <typename T, typename Container = std::deque<T>>
class LimitedQueue : public std::queue<T, Container> {
   private:
    std::size_t max_size;

   public:
    explicit LimitedQueue(std::size_t max_size) : max_size(max_size) {}
    void push(const T& value) {
        if (this->size() == max_size) {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }
};
#endif  // SAMPLER_SRC_UTILS_LIMITEDQUEUE_H_
