/**
 * MIT License
 *
 * Copyright (c) 2020, Beijing University of Posts and Telecommunications.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
#ifndef SAMPLEUTILS_H
#define SAMPLEUTILS_H

#pragma once
#include <algorithm>
#include <cmath>

#include"utils/utils.h"
#include "graph/kgraph.h"
#include "utils/utils.h"

enum SampleType { RN, RDN, PRN, RE, RNE, HRNE, TIES, PIES, BFS, DFS, SB, FF };
/* F-nom of vector x-y */
double F_norm(double *x, double *y, int n);
double ddrand(double a, double b);

void shuffle(int *a, int size);

long long find_backward_edge(LSGraph *graph, int p);

#endif