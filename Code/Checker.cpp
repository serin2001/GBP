#include "Checker.hpp"

Checker::Checker(GraphReader graphReader) {
	this->nodes = graphReader.getNodes();
	this->edges = graphReader.getEdges();
	this->adjList = graphReader.getAdjList();
}

bool Checker::check(vector<int> solution) {
    int len = (int)solution.size();
    vector<bool> nodeMark(nodes, 0); // No node is on fire yet
    queue<int> BFSQueue;
    int count = 0; 

	for (int t = 0; t < len; ++t) {
	    int sol = solution[t];
	    if (nodeMark[sol] == 0) {
			BFSQueue.push(sol);
			nodeMark[sol] = 1; // sol is queued
	    }
	    queue<int> newBFSQueue;
	    cout << " | t = " << t << "\n";
	    while (!BFSQueue.empty()) {
			int u = BFSQueue.front();
			BFSQueue.pop();
			nodeMark[u] = 2; // u is burned
				count++;
				cout << " " << u;
			for (auto v : adjList[u]) {
				if (nodeMark[v] == 0) {
					newBFSQueue.push(v);
					nodeMark[v] = 1; // v is queued
				}
			}
	    }
		BFSQueue = newBFSQueue;
			if (count == nodes) {
				cout << endl;
				return true;
			}
	}
       cout << "Unburned nodes: ";
            for (int node = 0; node < nodes; node++)
                if (!nodeMark[node])
                    cout << node << " !!";
            cout << endl;
        return false;
    }
