/*
 * Created by AutJ on 2021/12/6.
 */

#include <iostream>

#include "utils/GraphStream.h"

using namespace std;

int main() {
    cout << "123";
    auto graph1 = GraphStream::readText("input/simple.csv");
    GraphStream::writeBinary("input/simple.bin", graph1);
    auto graph2 = GraphStream::readBinary("input/simple.bin");
    GraphStream::writeText("input/simple2.csv", graph2);
}