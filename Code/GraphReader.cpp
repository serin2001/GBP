#include "GraphReader.hpp"

void GraphReader::generateFromWeightedEdgeList() {
	NetworKit::Graph graph(nodes, edges, weighted == 1);

    // Add edges with weights
    for (const auto& edge : weightedEdgeList) {
        int u = edge.first.first;      // Get the first node
        int v = edge.first.second;     // Get the second node
        double weight = edge.second;    // Get the corresponding weight

        graph.addEdge(u, v);           // Add the edge

        if (weighted == 1) {
            graph.setWeight(u, v, weight); // Set the weight if the graph is weighted
        }
    }
    std::cout << "Generating NetworKit Graph with Nodes: " << nodes << " and Edges: " << weightedEdgeList.size() << std::endl;
    nkGraph = graph;
}

void GraphReader::generateNKGraph() {
    // Call the new helper function using the member variable weightedEdgeList
    generateFromWeightedEdgeList();
}

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
    //int oneIndexed;
    graphFile >> oneIndexed;
    //int weighted;
    graphFile >> weighted;

    for (int i = 0; i < edges; i++) {
        int u, v; double w = 0.0;
        graphFile >> u >> v;
        if (oneIndexed) {
            u--; v--;
        }
        if (weighted == 1)
            graphFile >> w;
        else
            w = 1.0; // Default weight for unweighted graphs

        if (u > v)
            swap(u, v);
        weightedEdgeList.emplace_back(make_pair(u, v), w);
    }
    graphFile.close();

    sort(weightedEdgeList.begin(), weightedEdgeList.end());
    auto last = unique(weightedEdgeList.begin(), weightedEdgeList.end(),
                       [](const auto& a, const auto& b) {
                           return a.first == b.first; // Compare only the edges
                       });
    // Resize the vector to the new size after removing duplicates
    weightedEdgeList.erase(last, weightedEdgeList.end());

    edges = static_cast<int>(weightedEdgeList.size());
    adjList = vector<vector<int>>(nodes, vector<int>());

    for (const auto& edge : weightedEdgeList) {
        int u = edge.first.first;
        int v = edge.first.second;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    cout << graphName << " " << nodes << " " << edges << endl;
}

