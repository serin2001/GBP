#ifndef GRAPH_READER_HPP
#define GRAPH_READER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class GraphReader {
protected:
    int nodes;
    int edges;
    vector<vector<int>> adjList;

public:
    void readGraph(const string& graphFileName); // Declare function here
    // You may also want to add getters to access nodes, edges, and adjList
    int getNodes() const { return nodes; }
    int getEdges() const { return edges; }
    const vector<vector<int>>& getAdjList() const { return adjList; }
};

#endif // GRAPH_READER_HPP

