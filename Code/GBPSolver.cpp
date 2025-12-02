/*
 * GBPSolver.cpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#include "GBPSolver.hpp"

GBPSolver::GBPSolver(GraphReader graphReader, string graphFileName, int burningLength, int sampleSize)
    : graphAnalyzer(graphReader, sampleSize), algorithm(graphAnalyzer, burningLength) {
    // Constructor body (if needed) can go here
}

vector<int> GBPSolver::solve() {
//    runAnalyzer();  // Call to analyze the graph
	try {
		std::cout << "Entering GA.solve()" << std::endl;
		std::cout << "Middle Matrix Size: " << graphAnalyzer.getMiddleMatrix().size() << std::endl;
		std::cout << "Distance Matrix Size: " << graphAnalyzer.getDistanceMatrix().size() << std::endl;
		std::cout << "Middle Matrix Sample Value: " << graphAnalyzer.getMiddleMatrix()[10][5] << std::endl;
		std::cout << "Distance Matrix Sample Value: " << graphAnalyzer.getDistanceMatrix()[10][5] << std::endl;
		std::cout << "Centr Size: " << graphAnalyzer.getCentrality().size() << std::endl;
		vector<int> solution = algorithm.solve();     // Call to execute the genetic algorithm
		return solution;
	} catch (const std::exception& e) {
	    std::cerr << "Exception in GA.solve(): " << e.what() << std::endl;
	    return {};
	}
}
