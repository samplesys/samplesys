//
// Created by AutJ on 2021/11/27.
//

#ifndef SAMPLER_INCLUDE_GRAPH_GRAPH_H_
#define SAMPLER_INCLUDE_GRAPH_GRAPH_H_

#include <map>
#include <set>
#include <vector>

class Graph {
    friend class GraphStream;

   protected:
    std::vector<std::size_t> edges;
    std::vector<std::size_t> columns;
    std::vector<std::size_t> offsets;
    std::vector<std::size_t> degrees;

   public:
    [[nodiscard]] virtual const decltype(edges)   &get_edges() const;
    [[nodiscard]] virtual const decltype(columns) &get_columns() const;
    [[nodiscard]] virtual const decltype(offsets) &get_offsets() const;
    [[nodiscard]] virtual const decltype(degrees) &get_degrees() const;

    [[nodiscard]] virtual decltype(columns) calc_neighbours(size_t i) const;

    [[nodiscard]] virtual std::size_t number_of_nodes() const;
    [[nodiscard]] virtual std::size_t number_of_edges() const;

    [[nodiscard]] virtual size_t index_of_offsets(size_t loc) const;
    [[nodiscard]] virtual size_t loc_of_edge_between(size_t from, size_t to) const;
};

#endif  // SAMPLER_INCLUDE_GRAPH_GRAPH_H_
