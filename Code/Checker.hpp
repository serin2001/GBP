#include <bits/stdc++.h>
#include "GraphReader.hpp"

using namespace std;

class Checker {
    private:
        int nodes, edges;
        vector<vector<int>> adjList; 
    private:
//        void readGraph(string graphFileName); 
    public:
//        Checker(string fileName);
Checker(GraphReader graphReader);
        bool check(vector<int> solution);
};
