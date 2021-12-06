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

    auto offsets = g->offsets;
    auto columns = g->columns;
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
    }
    for (size_t i = 0; i < g->number_of_nodes(); ++i) {
        for (size_t loc = g->offsets[i]; loc < g->offsets[i + 1]; ++loc) {
            fprintf(fp, "%zd %zd\n", i, g->columns[loc]);
        }
    }
    fclose(fp);
}
shared_ptr<Graph> GraphStream::readBinary(const string &filename, bool directed) {
    auto   g  = shared_ptr<Graph>();
    auto   is = ifstream(filename, ios_base::in | ios_base::binary);
    size_t size;

    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    g->edges.assign(size, 0);
    is.read(reinterpret_cast<char *>(g->edges.data()),
            size * sizeof(decltype(g->edges)::value_type));

    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    g->columns.assign(size, 0);
    is.read(reinterpret_cast<char *>(g->columns.data()),
            size * sizeof(decltype(g->columns)::value_type));

    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    g->offsets.assign(size, 0);
    is.read(reinterpret_cast<char *>(g->offsets.data()),
            size * sizeof(decltype(g->offsets)::value_type));

    is.read(reinterpret_cast<char *>(&size), sizeof(size_t));
    g->degrees.assign(size, 0);
    is.read(reinterpret_cast<char *>(g->degrees.data()),
            size * sizeof(decltype(g->degrees)::value_type));

    return g;
}

void GraphStream::writeBinary(const string &filename, const shared_ptr<Graph> &g) {
    auto os = ofstream(filename, ios_base::out | ios_base::binary);

    os.write(reinterpret_cast<const char *>(g->edges.size()), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(g->edges.data()),
             g->edges.size() * sizeof(decltype(g->edges)::value_type));

    os.write(reinterpret_cast<const char *>(g->columns.size()), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(g->columns.data()),
             g->columns.size() * sizeof(decltype(g->columns)::value_type));

    os.write(reinterpret_cast<const char *>(g->offsets.size()), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(g->offsets.data()),
             g->offsets.size() * sizeof(decltype(g->offsets)::value_type));

    os.write(reinterpret_cast<const char *>(g->degrees.size()), sizeof(size_t));
    os.write(reinterpret_cast<const char *>(g->degrees.data()),
             g->degrees.size() * sizeof(decltype(g->degrees)::value_type));
}