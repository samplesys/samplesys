//
// Created by AutJ on 2021/11/1.
//

#include "utils.h"

#include <cstdio>
#include <cstring>

using namespace std;

void readGraph(size_t &number_of_nodes, map<size_t, set<size_t>> &adjList, const string &file) {
    FILE *fp = fopen(file.c_str(), "r");
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
    } else {
        size_t u, v;
        number_of_nodes = 0;
        while (fscanf(fp, "%zd,%zd", &u, &v) != -1) {
            number_of_nodes = max(number_of_nodes, max(u, v) + 1);
            adjList[u].insert(v);
        }
        fclose(fp);
    }
}

void printGraph(const vector<pair<size_t, size_t>> &edges, const string &file) {
    FILE *fp = fopen(file.c_str(), "w");
    if (!fp) {
        printf("error %d: %s \n", errno, strerror(errno));
    } else {
        for (const auto &e : edges) {
            fprintf(fp, "%lu,%lu\n", e.first, e.second);
        }
        fclose(fp);
    }
}
