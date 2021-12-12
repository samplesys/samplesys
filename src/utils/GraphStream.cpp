/*
 * Created by AutJ on 2021/12/6.
 */
#include "utils/GraphStream.h"

#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

shared_ptr<Graph> GraphStream::readText(const string &filename, bool directed) {
    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
        return nullptr;
    }
    size_t number_of_nodes = 0;
    auto   adjList         = map<size_t, set<size_t>>();
    size_t u, v;
    while (fscanf(fp, "%zd,%zd", &u, &v) != -1) {
        number_of_nodes = max(number_of_nodes, max(u, v) + 1);
        adjList[u].insert(v);
    }
    fclose(fp);
    if (directed) {
        return make_shared<DirectedGraph>(adjList, number_of_nodes);
    } else {
        return make_shared<UndirectedGraph>(adjList, number_of_nodes);
    }
}

void GraphStream::writeText(const string &filename, const shared_ptr<Graph> &g) {
    FILE *fp = fopen(filename.c_str(), "w");

    auto offsets = g->get_offsets();
    auto columns = g->get_columns();
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
    }
    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        for (size_t loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            fprintf(fp, "%zd %zd\n", i, columns[loc]);
        }
    }
    fclose(fp);
}
shared_ptr<Graph> GraphStream::readBinary(const string &filename, bool directed) {
    auto is = ifstream(filename, ios_base::in | ios_base::binary);

    size_t number_of_nodes = 0;
    size_t number_of_edges = 0;
    is.read(reinterpret_cast<char *>(&number_of_nodes), sizeof(size_t));
    is.read(reinterpret_cast<char *>(&number_of_edges), sizeof(size_t));
    auto adjList1 = vector<pair<size_t, size_t>>(number_of_edges);
    is.read(reinterpret_cast<char *>(adjList1.data()),
            number_of_edges * sizeof(decltype(adjList1)::value_type));

    auto adjList2 = map<size_t, set<size_t>>();
    for (const auto &p : adjList1) {
        adjList2[p.first].insert(p.second);
    }

    if (directed) {
        return make_shared<DirectedGraph>(adjList2, number_of_nodes);
    } else {
        return make_shared<UndirectedGraph>(adjList2, number_of_nodes);
    }
}

void GraphStream::writeBinary(const string &filename, const shared_ptr<Graph> &g) {
    auto os = ofstream(filename, ios_base::out | ios_base::binary);

    auto offsets = g->get_offsets();
    auto columns = g->get_columns();
    auto adjList = vector<pair<size_t, size_t>>();

    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        for (size_t loc = offsets[i]; loc < offsets[i + 1]; ++loc) {
            adjList.emplace_back(i, columns[loc]);
        }
    }

    size_t number_of_nodes = g->number_of_nodes();
    size_t number_of_edges = g->number_of_edges();
    os.write(reinterpret_cast<const char *>(&number_of_nodes), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(&number_of_edges), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(adjList.data()),
             number_of_edges * sizeof(decltype(adjList)::value_type));
}

void GraphStream::text_to_binary(const string &input_filename, const string &output_filename,
                                 bool directed) {
    FILE *fp = fopen(input_filename.c_str(), "r");
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
        return;
    }
    size_t number_of_nodes = 0;
    size_t number_of_edges = 0;
    auto   adjList1        = map<size_t, set<size_t>>();
    size_t u, v;
    if (directed) {
        while (fscanf(fp, "%zd,%zd", &u, &v) != -1) {
            number_of_nodes = max(number_of_nodes, max(u, v) + 1);
            number_of_edges += adjList1[u].insert(v).second;
        }
    } else {
        while (fscanf(fp, "%zd,%zd", &u, &v) != -1) {
            number_of_nodes = max(number_of_nodes, max(u, v) + 1);
            number_of_edges += adjList1[u].insert(v).second;
            if (u != v) number_of_edges += adjList1[v].insert(u).second;
        }
    }
    fclose(fp);

    auto os       = ofstream(output_filename, ios_base::out | ios_base::binary);
    auto adjList2 = vector<pair<size_t, size_t>>();

    adjList2.reserve(number_of_edges);
    for (const auto &p : adjList1) {
        auto i = p.first;
        for (auto j : p.second) {
            adjList2.emplace_back(i, j);
        }
    }

    os.write(reinterpret_cast<const char *>(&number_of_nodes), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(&number_of_edges), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(adjList2.data()),
             number_of_edges * sizeof(decltype(adjList2)::value_type));
}
