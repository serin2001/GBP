/*
 * GA.cpp
 *
 *  Created on: Dec 2, 2025
 *      Author: andreas
 */

#include "GA.hpp"

GA::GA(GraphAnalyzer graphAnalyzer, int burningLength) {
	this->burningLength = burningLength;
	this->nodes = graphAnalyzer.getNodes();
	this->adjList = graphAnalyzer.getAdjList();
	this->centrality = graphAnalyzer.getCentrality();
	this->middleMatrix = graphAnalyzer.getMiddleMatrix();
	this->distanceMatrix = graphAnalyzer.getDistanceMatrix();
	//this->solve(chromosomeSize, minimumDistance, skipValue, maxGenerations, topPopulation, crossoverPopulation, mutateProbabilty, alpha, beta)
}

vector<int> GA::generateChromosome(int chromosomeSize, int minimumDistance) {
    vector<int> chromosome(chromosomeSize, 0);
    mt19937 generator(randomDevice());
    for (int found = 0; found < chromosomeSize; found++) {
        vector<pair<double, int>> tempSigmoidCentrality(nodes, {0, 0});
        double weightSum = 0;
        for (int i = 0; i < nodes; i++) {
            tempSigmoidCentrality[i] = sigmoidCentrality[i];
            for (int j = 0; j < found; j++)
                if (distanceMatrix[centrality[i].second][centrality[chromosome[j]].second] < minimumDistance)
                    tempSigmoidCentrality[i].first = 0;
            weightSum += tempSigmoidCentrality[i].first;
        }
        if (weightSum == 0) {
            minimumDistance--;
            found--;
            continue;
        }
        uniform_real_distribution<> nodeSelector(0, weightSum);
        double sampleNode = nodeSelector(generator);
        for (int i = 0; i < nodes; i++) {
            sampleNode -= tempSigmoidCentrality[i].first;
            if (sampleNode < 0) {
                chromosome[found] = i;
                break;
            }
        }
    }
    vector<int> resultChromosome;
    for (auto node : chromosome)
        resultChromosome.push_back(centrality[node].second);

    //if (verbose)
    //    for (auto node : resultChromosome)
      //      cout << node << " ";

    return resultChromosome;
}

void GA::findNotBurnt(vector<int> chromosome) {
    notBurnt.clear();
    nodeMark = vector<int>(nodes, nodes);
    nodeFillMark = vector<int>(nodes, nodes);

    for (int i = 0; i < chromosome.size(); i++)
        for (int node = 0; node < nodes; node++)
            nodeMark[node] = min(nodeMark[node], max(0, ((int)distanceMatrix[chromosome[i]][node] - (burningLength - 1 - i))));

    for (int node = 0; node < nodes; node++)
        if (nodeMark[node] != 0)
            notBurnt.push_back(node);

//        cout << "Unverbrannt: " << notBurnt.size() << endl;
}

vector<int> GA::number2sequence(long long number, int sequencelength, int sequenceBase) {
    vector<int> sequence(sequencelength, 0);
    for (int i = 0; number > 0; i++) {
        sequence[i] = number % sequenceBase;
        number /= sequenceBase;
    }
    return sequence;
}

long long GA::costFunction(vector<int> chromosome, int skipValue) {
    findNotBurnt(chromosome);

    if ((int)notBurnt.size() > skipValue)
        return INF;

    int notBurntSize = (int)notBurnt.size();
    int remaininglength = burningLength - (int)chromosome.size();

    if (notBurntSize == 0) {
        bestBurningSequence.clear();
        for (auto node : chromosome)
            bestBurningSequence.push_back(node);
        for (int i = 0; i < remaininglength; i++)
            bestBurningSequence.push_back(0);
        return 0;
    }

    long long statesNumber = 1;
    for (int i = 0; i < remaininglength; i++)
        statesNumber *= notBurntSize;

    long long minimumCost = INF;
    for (long long number = 0; number < statesNumber; number++) {
        vector<int> sequence = number2sequence(number, remaininglength, notBurntSize);

        long long cost = 0;
        for (auto node : notBurnt)
            nodeFillMark[node] = nodes;

        for (int i = 0; i < remaininglength; i++)
            for (auto node : notBurnt)
                nodeFillMark[node] = min(nodeFillMark[node], max(0, (int)distanceMatrix[node][notBurnt[sequence[i]]] - (remaininglength - 1 - i)));

        for (auto node : notBurnt)
            if (nodeFillMark[node])
            // Das ist die Penalty-Funktion
                cost += min(nodeMark[node], nodeFillMark[node]) * min(nodeMark[node], nodeFillMark[node]);

        if (cost == 0) {
            for (auto nodeIndex : sequence)
                cout << notBurnt[nodeIndex] << ";";
            cout << endl;
      }
        minimumCost = min(minimumCost, cost);
        if (minimumCost == 0) {
            bestBurningSequence.clear();
            for (auto node : chromosome)
                bestBurningSequence.push_back(node);
            for (auto nodeIndex : sequence)
                bestBurningSequence.push_back(notBurnt[nodeIndex]);
            break;
        }
    }
    return minimumCost;
}

vector<int> GA::crossover(vector<int> parentChromosome1, vector<int> parentChromosome2) {
    mt19937 generator(randomDevice());
    uniform_int_distribution<> methodSelector(0, 2);
    vector<int> childChromosome;

    for (int i = 0; i < (int)parentChromosome1.size(); i++) {
        switch (methodSelector(generator)) {
            case 0:
                if (middleMatrix[parentChromosome1[i]][parentChromosome2[i]] < nodes) {
                    childChromosome.push_back(middleMatrix[parentChromosome1[i]][parentChromosome2[i]]);
                    break;
                }
            case 1:
                childChromosome.push_back(parentChromosome1[i]);
                break;
            case 2:
                childChromosome.push_back(parentChromosome2[i]);
                break;
        }
    }
    return childChromosome;
}

vector<int> GA::mutate(vector<int> chromosome, double mutateProbability) {
    mt19937 generator(randomDevice());
    uniform_real_distribution<> mutateSelector(0, 1);
    uniform_int_distribution<>  methodSelector(0, 1);

    for (int i = 0; i < (int)chromosome.size(); i++) {
        if (mutateSelector(generator) <= mutateProbability) {
            switch (methodSelector(generator)) {
                case 0: {
                    uniform_real_distribution<> nodeSelector(0, sigmoidCentralitySum);
                    double sampleNode = nodeSelector(generator);
                    for (int j = 0; j < nodes; j++) {
                        sampleNode -= sigmoidCentrality[j].first;
                        if (sampleNode < 0) {
                            chromosome[i] = centrality[j].second;
                            break;
                        }
                    }
                }
                break;
                case 1: {
                    int neighborSize = adjList[chromosome[i]].size();
                    uniform_int_distribution<> neighborSelector(0, neighborSize - 1);
                    chromosome[i] = adjList[chromosome[i]][neighborSelector(generator)];
                }
                break;
            }
        }
    }
    return chromosome;
}

int GA::rouletteWheelSelection(int generation, int topPopulation, double fitnessSum) {
    double maxWeight = 1.0 / ((double) population[generation][0].first + 1);
    mt19937 generator(randomDevice());
    uniform_int_distribution<> chromosomeSelector(0, population[generation].size() - 1);
    uniform_real_distribution<> chromosomeAcceptor(0, maxWeight);
    while (true) {
        int index = chromosomeSelector(generator);
        double weight = 1.0 / ((double) population[generation][index].first + 1);
        if (chromosomeAcceptor(generator) <= weight) {
            return index;
        }
    }
}

vector<int> GA::solve(int chromosomeSize, int minimumDistance,
                          int skipValue, int maxGenerations, int topPopulation,
                          int crossoverPopulation, double mutateProbability,
                          double alpha, double beta) {
    if (chromosomeSize == -1)
        chromosomeSize = max(burningLength - 3, 2);

    if (minimumDistance == -1)
        minimumDistance = max(burningLength - 3, 2);

    sigmoidCentrality = vector<pair<double, int>>(nodes, {0, 0});
    sigmoidCentralitySum = 0;
    for (int i = 0; i < nodes; i++) {
        sigmoidCentrality[i].first = 1.0 / (exp(-1 * beta * (centrality[i].first - alpha)) + 1);
        sigmoidCentrality[i].second = centrality[i].second;
        sigmoidCentralitySum += sigmoidCentrality[i].first;
    }

    for (int i = 0; i < topPopulation; i++) {
        cout << '*'; fflush(stdout);
        vector<int> chromosome = generateChromosome(chromosomeSize, minimumDistance);
        int fitness = costFunction(chromosome, skipValue);
        population[0].push_back({fitness, chromosome});
        if (fitness == 0)
            break;
    }
    cout << endl;

    for (int generation = 0; generation < maxGenerations; generation++) {
        sort(population[generation].begin(), population[generation].end());
        population[generation].erase(unique(population[generation].begin(), population[generation].end()), population[generation].end());

        if (population[generation][0].first == 0)
            return bestBurningSequence;

        if (population[generation][0].first == INF)
            skipValue += 10;

        while ((int)population[generation].size() < topPopulation) {
            vector<int> chromosome = generateChromosome(chromosomeSize, minimumDistance);
            int fitness = costFunction(chromosome, skipValue);
            population[generation].push_back({fitness, chromosome});
        }
        sort(population[generation].begin(), population[generation].end());

        double fitnessSum = 0;
        for (int i = 0; i < topPopulation; i++)
            fitnessSum += population[generation][i].first;

        cout << "#" << generation + 1 << ": " << endl;
        for (int i = 0; i < 10; i++)
            cout << population[generation][i].first << " ";
        cout << endl;

        for (int i = 0; i < topPopulation; i++)
            population[generation + 1].push_back(population[generation][i]);

        for (int i = 0; i < crossoverPopulation; i++) {
            int chromosomeParent1 = 0, chromosomeParent2 = 0;
            while (chromosomeParent1 == chromosomeParent2) {
                chromosomeParent1 = rouletteWheelSelection(generation, topPopulation, fitnessSum);
                chromosomeParent2 = rouletteWheelSelection(generation, topPopulation, fitnessSum);
            }
            vector<int> chromosome = crossover(population[generation][chromosomeParent1].second,
                                               population[generation][chromosomeParent2].second);
            int fitness = costFunction(chromosome, skipValue);
            population[generation + 1].push_back({fitness, chromosome});
        }
        for (int i = 0; i < topPopulation + crossoverPopulation; i++) {
            vector<int> chromosome = mutate(population[generation + 1][i].second, mutateProbability);
            int fitness = costFunction(chromosome, skipValue);
            population[generation + 1].push_back({fitness, chromosome});
        }
    }
    return bestBurningSequence;
}
