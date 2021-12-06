
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

size_t  *nodeid;
size_t  *degrees;
size_t  *nodeindex;
size_t **neighbors;

bool csvFlag     = false;
bool undirected  = true;
bool weighted    = false;
bool rand_weight = false;

int argPos(char *str, int argc, char **argv) {
    for (int i = 0; i < argc; ++i)
        if (!strcmp(str, argv[i])) return i;
    return -1;
}

void work(FILE *input, FILE *ot) {
    size_t              n, e;
    std::vector<size_t> xs, ys;
    size_t              maxi = 0;
    size_t              x, y;
    e = 0;

    if (csvFlag) {  // 是否用 ','分隔
        // char s[30];
        // if (!fscanf(input, "%s", s)) std::cout << "Error" << std::endl;
        while (fscanf(input, "%zd,%zd", &x, &y) > 0) {
            e++;
            if (x > maxi) maxi = x;
            if (y > maxi) maxi = y;
            xs.push_back(x);
            ys.push_back(y);
        }
    } else {
        while (fscanf(input, "%zd%zd", &x, &y) > 0) {
            e++;
            if (x > maxi) maxi = x;
            if (y > maxi) maxi = y;
            xs.push_back(x);
            ys.push_back(y);
        }
    }

    std::cout << "max id: " << maxi << std::endl;
    n = maxi + 1;

    fclose(input);

    /* degrees 表示(出)度, nodeindex表示(出)邻居个数 */
    /* graph is directed -> degree[i] = out_degree[i] */
    degrees   = static_cast<size_t *>(malloc(n * sizeof(size_t)));
    nodeindex = static_cast<size_t *>(malloc(n * sizeof(size_t)));
    neighbors = static_cast<size_t **>(malloc(n * sizeof(size_t *)));
    memset(degrees, 0, n * sizeof(size_t));
    memset(nodeindex, 0, n * sizeof(size_t));

    for (size_t j = 0; j < e; j++) {
        degrees[xs[j]]++;
        if (undirected) degrees[ys[j]]++;
    }

    for (size_t i = 0; i < n; i++)
        neighbors[i] = static_cast<size_t *>(malloc(degrees[i] * sizeof(size_t)));

    for (size_t j = 0; j < e; j++) {
        neighbors[xs[j]][nodeindex[xs[j]]++] = ys[j];
        if (undirected) neighbors[ys[j]][nodeindex[ys[j]]++] = xs[j];
    }

    /* 去重边 */
    for (size_t i = 0; i < n; i++) {
        nodeindex[i] = 0;
        if (degrees[i] == 0) continue;
        std::sort(neighbors[i], neighbors[i] + degrees[i]);
        nodeindex[i] = 1;
        for (size_t j = 1; j < degrees[i]; j++)
            if (neighbors[i][j] != neighbors[i][j - 1]) nodeindex[i]++;
    }

    e = 0;
    for (size_t i = 0; i < n; i++)
        e += nodeindex[i];

    fwrite(&n, sizeof(size_t), 1, ot);
    fwrite(&e, sizeof(size_t), 1, ot);

    std::cout << n << " vertex, " << e << " edges" << std::endl;

    size_t x2 = 0;
    for (size_t i = 0; i < n; i++) {
        fwrite(&x2, sizeof(size_t), 1, ot);
        x2 += nodeindex[i];
    }
    for (size_t i = 0; i < n; i++) {
        if (degrees[i] == 0) continue;
        fwrite(neighbors[i], sizeof(size_t), 1, ot);
        for (size_t j = 1; j < degrees[i]; j++)
            if (neighbors[i][j] != neighbors[i][j - 1])
                fwrite(&neighbors[i][j], sizeof(size_t), 1, ot);
    }
    srand((unsigned)time(NULL));

    if (rand_weight) {
        double *weight = new double[e];

        for (size_t i = 0; i < e; i++) {
            weight[i] = rand() / double(RAND_MAX);
        }
        fwrite(weight, sizeof(double), e, ot);
    }
    std::cout << "Graph generated." << std::endl;
    fclose(ot);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage: gen path/to/input path/to/output [Options]\n"
                  << "Options:\n"
                  << "  -weighted\tWeighted graph.(default: unweighted)\n"
                  << "  -rand-w\tGenerate random weight.\n"
                  << "  -directed\tSet graph to be directed. (default: undirected)\n";
        exit(1);
    }
    FILE *inputFile  = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "wb");
    int   a          = 0;
    if (strstr(argv[a + 1], ".csv")) {
        csvFlag = true;
    } else {
        csvFlag = false;
    }
    if ((a = argPos(const_cast<char *>("-weighted"), argc, argv)) > 0) weighted = true;
    if ((a = argPos(const_cast<char *>("-rand-w"), argc, argv)) > 0) rand_weight = true;
    if ((a = argPos(const_cast<char *>("-directed"), argc, argv)) > 0) undirected = false;
    work(inputFile, outputFile);
}