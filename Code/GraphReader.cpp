#include "GraphReader.hpp"

void GraphReader::readGraph(const string& graphFileName) {
    string filePath = "./Data/" + graphFileName;
    cout << filePath << endl;
    ifstream graphFile(filePath);

    if (!graphFile.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }
    
    string graphName;
    graphFile >> graphName; 
    graphFile >> nodes >> edges;
    int oneIndexed; 
    graphFile >> oneIndexed;
    int weighted; 
    graphFile >> weighted;

    vector<pair<int, int>> edgeList;
    for (int i = 0; i < edges; i++) {
        int u, v; double w;
        graphFile >> u >> v;
        if (oneIndexed) {
            u--; v--;
        }
        if (weighted == 1)
            graphFile >> w;

        if (u > v)
            swap(u, v);
        edgeList.push_back({u, v});
    }
    graphFile.close();

    sort(edgeList.begin(), edgeList.end());
    edgeList.erase(unique(edgeList.begin(), edgeList.end()), edgeList.end());

    edges = static_cast<int>(edgeList.size());
    adjList = vector<vector<int>>(nodes, vector<int>());

    for (const auto& edge : edgeList) {
        int u = edge.first;
        int v = edge.second;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    cout << graphName << " " << nodes << " " << edges << endl;
}

