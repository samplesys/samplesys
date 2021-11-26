//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_EXAMPLE_UTILS_H_
#define SAMPLER_EXAMPLE_UTILS_H_

#include <map>
#include <set>
#include <string>
#include <vector>

void readGraph(std::size_t &number_of_nodes,
               std::map<std::size_t, std::set<std::size_t>> &adjList,
               const std::string &file);
void printGraph(const std::vector<std::pair<size_t, size_t>> &edges,
                const std::string &file);

#endif  // SAMPLER_EXAMPLE_UTILS_H_
