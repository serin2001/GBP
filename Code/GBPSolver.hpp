/*
 * GBPSolver.hpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#ifndef CODE_GBPSOLVER_HPP_
#define CODE_GBPSOLVER_HPP_

#include "GraphReader.hpp"
#include "GraphAnalyzer.hpp"
#include "GA.hpp"
#include <networkit/graph/Graph.hpp>

class GBPSolver {
public:
	GBPSolver(GraphReader graphReader, string fileName, int burningLength, int sampleSize = 1000);
	vector<int> solve();
private:
    GraphAnalyzer graphAnalyzer;
    GA algorithm;

//    void runAnalyzer();
//    void runGA();
    // Any additional methods or attributes as needed for control flow
};

#endif /* CODE_GBPSOLVER_HPP_ */
