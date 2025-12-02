/*
 * GraphAnalyzer.hpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#ifndef CODE_GRAPHANALYZER_HPP_
#define CODE_GRAPHANALYZER_HPP_

#include "BetweennessApprox/bc.hpp"
#include "BetweennessApprox/graph_t.hpp"
#include "BetweennessApprox/utility.hpp"

#include "GraphReader.hpp"
#include <networkit/graph/Graph.hpp>

class GraphAnalyzer {
public:
	GraphAnalyzer(GraphReader graphReader, int sample_size);

    void calculateMiddleNodes();
    void findComponents();
    void betweennessCentrality(GraphReader graphReader, int sampleSize);
    vector<double> run_RAND1(GraphReader graphReader, int sample_size);
    int getNodes() const { return nodes; }
        int getEdges() const { return edges; }
    vector<vector<unsigned short int>> getMiddleMatrix() { return middleMatrix; }
    vector<vector<unsigned short int>> getDistanceMatrix() { return distanceMatrix; }
    string getPath() const { return path; }
        const vector<vector<int>>& getAdjList() const { return adjList; }
        const vector<pair<double, int>>& getCentrality() const { return centrality; }

private:
    int nodes, edges;
    string path;
    vector<vector<int>> adjList;
    double sigmoidCentralitySum;
    vector<int> nodeMark;
    vector<int> nodeFillMark;

    void BFSDistance(int start);
    void BFSMiddle(int start);

    vector<vector<unsigned short int>> middleMatrix;
    vector<vector<unsigned short int>> distanceMatrix;
    vector<vector<int>> components;
    vector<int> componentsSize;
    vector<pair<double, int>> centrality;

    // Add other necessary members
};

#endif /* CODE_GRAPHANALYZER_HPP_ */

 //   private:
//        vector<int> notBurnt;
        //vector<pair<int, vector<int>>> population[MAX_GA_ITERATIONS];
        //random_device randomDevice;
        //vector<int> bestBurningSequence;



//        vector<int> generateChromosome(int chromosomeSize, int minimumDistance = 1);
//        void findNotBurnt(vector<int> chromosome);
//        vector<int> number2sequence(long long number, int sequencelength, int sequenceBase);
//        long long costFunction(vector<int> chromosome, int skipValue);
//        int rouletteWheelSelection(int generation, int topPopulation, double fitnessSum);
//        vector<int> crossover(vector<int> parentChromosome1, vector<int> parentChromosome2);
//        vector<int> mutate(vector<int> chromosome, double mutateProbabilty);
//    public:
////        Solver(GraphReader graphReader, int burninglength, int sampleSize = 1000);
//Solver(GraphReader graphReader, string graphFileName, int burninglength, int sampleSize = 1000);
//        vector<int> solve(int chromosomeSize = -1, int minimumDistance = -1,
//                          int skipValue = 20, int maxGenerations = 50, int topPopulation = 300,
//                          int crossoverPopulation = 500, double mutateProbabilty = 0.1,
//                          double alpha = 0.05, double beta = 200);
//};
