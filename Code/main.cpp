#include "main.hpp"
#include "GraphReader.hpp"

int32_t main(int argc, char** argv) {
    string fileName = argv[1];
    int b = strtol(argv[2], NULL, 10);
    
    GraphReader graphReader; // Create an instance of GraphReader
    string graphFileName = fileName; // Replace with your graph file name
    graphReader.readGraph(graphFileName); // Call the readGraph method
    graphReader.generateNKGraph();

//    Solver G1(graphReader, b, 1000);
    Solver G1(graphReader, graphFileName, b, 1000);
    vector<int> solution = G1.solve();
//    vector<int> solution = {1, 31, 0, 0};
    
    if(solution.empty()) {
        cout << BOLD(FRED("No feasible solution found!")) << endl;
    } else {
        cout << "Solution: ";
        for (auto node : solution)
            cout << node << " ";
        cout << endl;
        Checker checkG1(graphReader);
        if (checkG1.check(solution))
            cout << BOLD(FGRN("Verified!")) << endl;
        else
            cout << BOLD(FRED("NOT Verified!")) << endl;  
    }    
}
