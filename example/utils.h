//
// Created by AutJ on 2021/11/1.
//

#ifndef SAMPLER_EXAMPLE_UTILS_H_
#define SAMPLER_EXAMPLE_UTILS_H_

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <vector>

void printGraph(const std::vector<std::pair<size_t, size_t>> &edges, const std::string &file);

/* KS-test to measure the agreement between the two sampled distributions. */
template <class data>
void getKSStatsSort(data *x, data *y, int lenx, int leny, double &ks_stats);

template <class data>
void getKSStats(data *x, data *y, int lenx, int leny, double &ks_stats);

template <class data>
void getKSStatsMap(const std::map<data, std::size_t> &distbx,
                   const std::map<data, std::size_t> &distby, double &ks_stats);

/**
 * Implementation
 */

template <class data>
void getKSStatsSort(data *x, data *y, int lenx, int leny, double &ks_stats) {
    size_t px = 0;
    size_t py = 0;
    double t  = __DBL_MIN__;
    ks_stats  = 0;
    while (1) {
        while (px < lenx && x[px] <= t)
            px++;
        while (py < leny && y[py] <= t)
            py++;
        double dx = ((double)px) / lenx;
        double dy = ((double)py) / leny;
        double d  = fabs(dx - dy);
        if (d > ks_stats) ks_stats = d;
        double new_t = min(x[px], y[py]);
        if (new_t == t)
            break;
        else
            t = new_t;
    }
}

/**
 * @brief Two-sample two-sided Kolmogorov-Smirnov (KS) test of arrays x and y.
 *
 * @tparam data(available in sort())
 * @param x head of first array
 * @param y head of second array
 * @param lenx length of array x
 * @param leny lengh of array y
 * @param ks_stats ks-test statistic
 */
template <class data>
void getKSStats(data *x, data *y, int lenx, int leny, double &ks_stats) {
    sort(x, x + lenx);
    sort(y, y + leny);
    getKSStatsSort(x, y, lenx, leny, ks_stats);
}

/**
 * @brief Two-sample two-sided Kolmogorov-Smirnov (KS) test of samples distributed in map x and y.
 *
 * @tparam data
 * @param distbx distribution x
 * @param distby distribution y
 * @param ks_stats ks-test statistic
 */
template <class data>
void getKSStatsMap(const std::map<data, std::size_t> &distbx,
                   const std::map<data, std::size_t> &distby, double &ks_stats) {
    std::size_t nfx  = 0;
    std::size_t nfy  = 0;
    std::size_t lenx = 0;
    std::size_t leny = 0;
    double      t    = __DBL_MIN__;
    auto        itx  = distbx.begin();
    auto        ity  = distby.begin();
    for (auto pr : distbx)
        lenx += pr.second;
    for (auto pr : distby)
        leny += pr.second;
    while (1) {
        while (itx != distbx.end() && itx->first <= t) {
            nfx += itx->second;
            itx++;
        }
        while (ity != distby.end() && ity->first <= t) {
            nfy += ity->second;
            ity++;
        }
        double dx = ((double)nfx) / lenx;
        double dy = ((double)nfy) / leny;
        double d  = fabs(dx - dy);
        if (d > ks_stats) ks_stats = d;
        // get next smallest number
        double newt = __DBL_MAX__;
        if (itx != distbx.end() && itx->first < newt) newt = itx->first;
        if (ity != distby.end() && ity->first < newt) newt = ity->first;
        if (newt == __DBL_MAX__ || newt == t) break;
        t = newt;
    }
}

#endif  // SAMPLER_EXAMPLE_UTILS_H_
