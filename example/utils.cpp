//
// Created by AutJ on 2021/11/1.
//

#include "utils.h"

using namespace std;

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
