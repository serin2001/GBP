#define BOOST_ALLOW_DEPRECATED_HEADERS
#define INF                             1000
#define MAX_NODES                       100000
#define MAX_GA_ITERATIONS               10000

#include <bits/stdc++.h>

#include <networkit/graph/Graph.hpp>
#include "GraphReader.hpp"

#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>

#include "BetweennessApprox/bc.hpp"
#include "BetweennessApprox/graph_t.hpp"
#include "BetweennessApprox/utility.hpp"

using namespace std;

class Solver {
    private:
        int nodes, edges;
        int burninglength;
        double sigmoidCentralitySum;
        vector<int> nodeMark;
        vector<int> nodeFillMark;
        vector<vector<int>> adjList;
        vector<pair<double, int>> centrality;
        vector<pair<double, int>> sigmoidCentrality;
        vector<vector<unsigned short int>> middleMatrix;
        vector<vector<unsigned short int>> distanceMatrix;
        vector<vector<int>> components;
        vector<int> componentsSize;
        vector<int> notBurnt;
        vector<pair<int, vector<int>>> population[MAX_GA_ITERATIONS];
        random_device randomDevice;
        vector<int> bestBurningSequence;
        vector<double> run_RAND1(string graph_path, GraphReader graphReader, int sample_size);
    private:
        void findComponents();
        void betweennessCentrality(string graphFileName, GraphReader graphReader, int sampleSize);
        void BFSDistance(int start);
        void BFSMiddle(int start);
        void calculateMiddleNodes();
        vector<int> generateChromosome(int chromosomeSize, int minimumDistance = 1);
        void findNotBurnt(vector<int> chromosome);
        vector<int> number2sequence(long long number, int sequencelength, int sequenceBase);
        long long costFunction(vector<int> chromosome, int skipValue);
        int rouletteWheelSelection(int generation, int topPopulation, double fitnessSum);
        vector<int> crossover(vector<int> parentChromosome1, vector<int> parentChromosome2);
        vector<int> mutate(vector<int> chromosome, double mutateProbabilty);
    public:
//        Solver(GraphReader graphReader, int burninglength, int sampleSize = 1000);
Solver(GraphReader graphReader, string graphFileName, int burninglength, int sampleSize = 1000);
        vector<int> solve(int chromosomeSize = -1, int minimumDistance = -1, 
                          int skipValue = 20, int maxGenerations = 50, int topPopulation = 300,
                          int crossoverPopulation = 500, double mutateProbabilty = 0.1, 
                          double alpha = 0.05, double beta = 200);
};
