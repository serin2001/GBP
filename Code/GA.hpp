/*
 * GA.hpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#ifndef CODE_GA_HPP_
#define CODE_GA_HPP_

#define BOOST_ALLOW_DEPRECATED_HEADERS
#define INF                             1000
#define MAX_NODES                       100000
#define MAX_GA_ITERATIONS               10000


#include <bits/stdc++.h>

#include <networkit/graph/Graph.hpp>
#include "GraphReader.hpp"
#include "GraphAnalyzer.hpp"

#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#include "BetweennessApprox/bc.hpp"
#include "BetweennessApprox/graph_t.hpp"
#include "BetweennessApprox/utility.hpp"

using namespace std;

class GA {
public:
	GA(GraphAnalyzer graphAnalyzer, int burningLength);
  //  vector<int> solve(int maxGenerations, int topPopulation, int crossoverPopulation, double mutateProbability, double alpha, double beta);
    vector<int> generateChromosome(int chromosomeSize, int minimumDistance = 1);
    vector<int> solve(int chromosomeSize = -1, int minimumDistance = -1,
                              int skipValue = 20, int maxGenerations = 50, int topPopulation = 300,
                              int crossoverPopulation = 500, double mutateProbability = 0.1,
                              double alpha = 0.05, double beta = 200);
private:
    int nodes, edges;
    vector<int> notBurnt;
    vector<int> crossover(vector<int> parent1, vector<int> parent2);
    vector<int> mutate(vector<int> chromosome, double mutateProbability);
    int rouletteWheelSelection(int generation, int topPopulation, double fitnessSum);
    vector<int> number2sequence(long long number, int sequencelength, int sequenceBase);
    long long costFunction(vector<int> chromosome, int skipValue);
    vector<int> bestBurningSequence;
    int burningLength;
    vector<pair<int, vector<int>>> population[MAX_GA_ITERATIONS];
    void findNotBurnt(vector<int> chromosome);
    double sigmoidCentralitySum;
            vector<int> nodeMark;
            vector<int> nodeFillMark;
            vector<vector<int>> adjList;
            vector<pair<double, int>> centrality;
            vector<pair<double, int>> sigmoidCentrality;
            vector<vector<unsigned short int>> middleMatrix;
            vector<vector<unsigned short int>> distanceMatrix;
        //    vector<vector<int>> components;
       //     vector<int> componentsSize;
            random_device randomDevice;
    // Add other necessary members
};

#endif /* CODE_GA_HPP_ */
