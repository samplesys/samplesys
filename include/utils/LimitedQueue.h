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
    typedef typename Container::value_type value_type;

    explicit LimitedQueue(std::size_t max_size) : max_size(max_size) {}

    void push(const value_type& __x) {
        if (this->size() == max_size) {
            this->c.pop_front();
        }
        this->c.push_back(__x);
    }

#if __cplusplus >= 201103L
    void push(value_type&& __x) {
        if (this->size() == max_size) {
            this->c.pop_front();
        }
        this->c.push_back(std::move(__x));
    }

#if __cplusplus > 201402L
    template <typename... _Args>
    decltype(auto) emplace(_Args&&... __args) {
        if (this->size() == max_size) {
            this->c.pop_front();
        }
        return this->c.emplace_back(std::forward<_Args>(__args)...);
    }
#else
    template <typename... _Args>
    void emplace(_Args&&... __args) {
        if (this->size() == max_size) {
            this->c.pop_front();
        }
        this->c.emplace_back(std::forward<_Args>(__args)...);
    }
#endif
#endif
};

#endif  // SAMPLER_SRC_UTILS_LIMITEDQUEUE_H_
