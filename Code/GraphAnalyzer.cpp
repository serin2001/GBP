/*
 * GraphAnalyzer.cpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#include "GraphAnalyzer.hpp"

#include <networkit/graph/Graph.hpp>
#include <networkit/centrality/Centrality.hpp>
#include <networkit/centrality/Betweenness.hpp>

GraphAnalyzer::GraphAnalyzer(GraphReader graphReader, int sample_size) {
    this->nodes = graphReader.getNodes();
    this->edges = graphReader.getEdges();
    this->adjList = graphReader.getAdjList();
    this->path = graphReader.getPath();
    this->calculateMiddleNodes();
    this->findComponents();
    this->betweennessCentrality(graphReader, sample_size);
}

void GraphAnalyzer::findComponents() {
    nodeMark = vector<int>(nodes, 0);
    componentsSize = vector<int>(nodes, 0);

    for (int i = 0; i < nodes; i++) {
        if (nodeMark[i] != 0)
            continue;

        vector<int> component;
        for (int j = 0; j < nodes; j++) {
            if (distanceMatrix[i][j] < USHRT_MAX) {
                nodeMark[j] = i + 1;
                component.push_back(j);
            }
        }
        components.push_back(component);
        for (int j = 0; j < nodes; j++) {
            if (distanceMatrix[i][j] < USHRT_MAX) {
                componentsSize[j] = component.size();
            }
        }
    }
}

vector<double> GraphAnalyzer::run_RAND1(GraphReader graphReader, int sample_size) {
  sgraph_t comp;
  BC bc(graphReader);
  bc.prepare_sgraph(path, graphReader, comp);

  vector<double> RAND1_between_centrality_result;

  bc.RAND1(comp, RAND1_between_centrality_result, sample_size);

  return RAND1_between_centrality_result;
}

void GraphAnalyzer::betweennessCentrality(GraphReader graphReader, int sampleSize) {
    vector<double> centralityValues(nodes, 0.0), componentsMaxCentrality(nodes, 0.0);

 //   graphFileName = path;
//    sgraph_t comp;
//    prepare_sgraph(graphFileName, graphReader, comp);
    centralityValues = run_RAND1(graphReader, min(sampleSize, nodes));

        std::cout << "Centrality Values: ";
    for (size_t i = 0; i < min(20, static_cast<int>(centralityValues.size())); ++i) {
        // Access the pair at index i
        std::cout << "{" << centralityValues[i] << "}";

        // Add a comma if it's not the last element
        if (i < centralityValues.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "..." << endl;
    std::cout << "Calculating exact Betweenness Centrality using NetworKit...";

    // 1. Instantiate the NetworKit algorithm using the converted graph 'this->nkGraph'
    NetworKit::Betweenness nkBC(graphReader.getNKGraph(), false);

    // 2. Run the algorithm
    nkBC.run();

    // 3. Get the scores (vector of doubles)
    vector<double> nkScores = nkBC.scores();

    for (size_t i = 0; i < min(20, static_cast<int>(nkScores.size())); ++i) {
        // Access the pair at index i
        std::cout << nkScores[i];

        // Add a comma if it's not the last element
        if (i < nkScores.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "..." << endl;
    // 4. Process the scores as needed (e.g., sort them and print)
    // The existing 'centrality' member variable in Solver might be a vector of pairs,
    // so you'd populate that if you need to reuse the existing sorting/printing logic:

centralityValues = nkBC.scores();

    for (int i = 0; i < nodes; i++)
        componentsMaxCentrality[nodeMark[i]] = max(componentsMaxCentrality[nodeMark[i]], centralityValues[i]);

    for (int i = 0; i < nodes; i++) {
        if (componentsMaxCentrality[nodeMark[i]] == 0)
            centrality.push_back({1, i});
        else
            centrality.push_back({centralityValues[i] / componentsMaxCentrality[nodeMark[i]], i});
    }

    sort(centrality.begin(), centrality.end(), greater<pair<double, int>>());
    std::cout << "Sorting  Centrality...";
    for (size_t i = 0; i < min(20, static_cast<int>(centrality.size())); ++i) {
            // Access the pair at index i
            std::cout << "(" << centrality[i].first << "," << centrality[i].second << ")";

            // Add a comma if it's not the last element
            if (i < centrality.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "..." << endl;
}

void GraphAnalyzer::BFSDistance(int start) {
    queue<int> BFSQueue;
    nodeMark = vector<int>(nodes, 0);
    vector<unsigned short int> height(nodes, USHRT_MAX);

    BFSQueue.push(start);
    height[start] = 0;
    nodeMark[start] = 1;

    while (BFSQueue.size()) {
        int u = BFSQueue.front();
        BFSQueue.pop();
        for (int i = 0; i < adjList[u].size(); i++) {
            int v = adjList[u][i];
            if (nodeMark[v] == 0) {
                nodeMark[v] = 1;
                height[v] = height[u] + 1;
                BFSQueue.push(v);
            }
        }
    }
    distanceMatrix[start] = height;
}

void GraphAnalyzer::BFSMiddle(int start) {
    queue<int> BFSQueue;
    vector<int> seq;
    nodeMark = vector<int>(nodes, 0);
    vector<unsigned short int> middle(nodes, USHRT_MAX);
    vector<unsigned short int> parent(nodes, 0);

    BFSQueue.push(start);
    seq.push_back(start);
    nodeMark[start] = 1;

    while (BFSQueue.size()) {
        int u = BFSQueue.front();
        BFSQueue.pop();
        for (int i = 0; i < adjList[u].size(); i++) {
            int v = adjList[u][i];
            if (nodeMark[v] == 0) {
                parent[v] = u;
                nodeMark[v] = 1;
                BFSQueue.push(v);
                seq.push_back(v);
            }
        }
    }
    int i = 0, ptr = 0;
    while (i < seq.size()) {
        int u = seq[i]; int v = seq[ptr];
        if (distanceMatrix[start][u] % 2 == 1) {
            middle[u] = middle[parent[u]];
            i++;
        }
        else if (((int)distanceMatrix[start][v] == (int)distanceMatrix[v][u]) &&
                ((int)distanceMatrix[start][v] + (int)distanceMatrix[v][u] == (int)distanceMatrix[start][u])) {
            middle[u] = v;
            i++;
        } else {
            ptr++;
        }
    }
    middleMatrix[start] = middle;
}

void GraphAnalyzer::calculateMiddleNodes() {
    middleMatrix = vector<vector<unsigned short int>> (nodes, vector<unsigned short int>());
    distanceMatrix = vector<vector<unsigned short int>> (nodes, vector<unsigned short int>());

    for (int i = 0; i < nodes; i++)
        BFSDistance(i);
    for (int i = 0; i < nodes; i++)
        BFSMiddle(i);

    if (false) {
        ofstream middleMatrixFile("middleMatrix.txt");
        for (auto vec : middleMatrix)
            for (int i = 0; i < vec.size(); i++)
                middleMatrixFile << vec[i] << " \n"[i == ((int)vec.size() - 1)];
        middleMatrixFile.close();

        ofstream distanceMatrixFile("distanceMatrix.txt");
        for (auto vec : distanceMatrix)
            for (int i = 0; i < vec.size(); i++)
                distanceMatrixFile << vec[i] << " \n"[i == ((int)vec.size() - 1)];
        distanceMatrixFile.close();
    }
}

