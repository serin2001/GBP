#ifndef GRAPH_READER_HPP
#define GRAPH_READER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <networkit/graph/Graph.hpp>
using namespace std;

class GraphReader {
protected:
    int nodes;
    int edges;
    int oneIndexed;
    int weighted;
    vector<vector<int>> adjList;
    vector<pair<pair<int, int>, double>> weightedEdgeList;
    NetworKit::Graph nkGraph;

public:
    void readGraph(const string& graphFileName); // Declare function here
    // You may also want to add getters to access nodes, edges, and adjList
    int getNodes() const { return nodes; }
    int getEdges() const { return edges; }
    int getOneIndexed() const { return oneIndexed; }
    int getWeighted() const { return weighted; }
    const vector<vector<int>>& getAdjList() const { return adjList; }
    const vector<pair<pair<int, int>, double>>& getWeightedEdgeList() const { return weightedEdgeList; }
    void generateFromWeightedEdgeList();
    void generateNKGraph();
    const NetworKit::Graph& getNKGraph() const { return nkGraph; }
};

#endif // GRAPH_READER_HPP

