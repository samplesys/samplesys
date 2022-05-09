#include "analysis/degree.h"
using namespace std;

namespace Backend {
void outer(vector<double>& x, vector<double>& y, vector<vector<double>>& xy) {
    xy.clear();
    size_t         n = x.size();
    size_t         m = y.size();
    vector<double> tmp(m, 0);
    xy.resize(n, tmp);
#pragma omp parallel for
    for (size_t i = 0; i < n; i++) {
#pragma omp parallel for
        for (size_t j = 0; j < m; j++) {
            xy[i][j] = x[i] * y[j];
        }
    }
}

}  // namespace Backend
