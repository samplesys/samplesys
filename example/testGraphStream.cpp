/*
 * Created by AutJ on 2021/12/6.
 */

#include <iostream>

#include "utils/GraphStream.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "[usage]: " << argv[0] << " path/to/input"
             << " path/to/output"
             << " direct/undirected" << endl;
    }
    int    _argc = 0;
    string input = argv[++_argc];
    cout << "input: " << input << endl;
    string output = argv[++_argc];
    cout << "output: " << output << endl;
    string graphType = argv[++_argc];
    cout << "graphType: " << graphType << endl;

    auto graph = GraphStream::readText(input, graphType == "directed");
    GraphStream::writeBinary(output, graph);

    GraphStream::text_to_binary(input, output, graphType == "directed");
}