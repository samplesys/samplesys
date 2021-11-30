// #include "analysis/degree.h"

#include <vector>
using namespace std;

namespace Backend {
void outer(vector<double>& x, vector<double>& y, vector<vector<double>>& xy) {
    xy.clear();
    size_t n = x.size();
    size_t m = y.size();
    for (size_t i = 0; i < n; i++) {
        xy.push_back({});
        for (size_t j = 0; j < m; j++)
            xy[i].push_back(x[i] * y[j]);
    }
}

}  // namespace Backend
