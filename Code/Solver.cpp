#include "Solver.hpp"

#include <networkit/graph/Graph.hpp>
#include <networkit/centrality/Centrality.hpp>
#include <networkit/centrality/Betweenness.hpp>

Solver::Solver(GraphReader graphReader, string fileName, int burninglength, int sample_size) {
    this->burninglength = burninglength;
    this->nodes = graphReader.getNodes();
        this->edges = graphReader.getEdges();
            this->adjList = graphReader.getAdjList();

   // this->readGraph(fileName);
    this->calculateMiddleNodes();
    this->findComponents();
    this->betweennessCentrality(fileName, graphReader, sample_size);
}

void Solver::findComponents() {
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

vector<double> Solver::run_RAND1(string graph_path, GraphReader graphReader, int sample_size) {
  sgraph_t comp;
  BC bc(graphReader);
  bc.prepare_sgraph(graph_path, graphReader, comp);

  vector<double> RAND1_between_centrality_result;

  bc.RAND1(comp, RAND1_between_centrality_result, sample_size);

  return RAND1_between_centrality_result;
}

void Solver::betweennessCentrality(string graphFileName, GraphReader graphReader, int sampleSize) {
    vector<double> centralityValues(nodes, 0.0), componentsMaxCentrality(nodes, 0.0);

    graphFileName = "./Data/" + graphFileName;
//    sgraph_t comp;
//    prepare_sgraph(graphFileName, graphReader, comp);
    centralityValues = run_RAND1(graphFileName, graphReader, min(sampleSize, nodes));

    //RAND1 rand1;
    //rand1.RAND1(graphFileName, graphReader, std::min(sampleSize, nodes));
    //r.run();
    //centralityValues = rand1.getBetweennessCentrality();
        std::cout << "Centrality Values: ";
    for (size_t i = 0; i < min(20, static_cast<int>(centralityValues.size())); ++i) {
        // Access the pair at index i
        std::cout << "{" << centralityValues[i] << "}";

        // Add a comma if it's not the last element
        if (i < centralityValues.size() - 1) {
            std::cout << ", ";
        }
    }
 
    std::cout << "\nCalculating exact Betweenness Centrality using NetworKit...";

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

  //  if (verbose)
  //      for (auto nodeValues : centrality) {
  //          cout << nodeValues.first << " " << nodeValues.second << endl;
  //          if (nodeValues.first < 0.01)
  //              break;
  //      }
}

void Solver::BFSDistance(int start) {
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

void Solver::BFSMiddle(int start) {
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

void Solver::calculateMiddleNodes() {
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

vector<int> Solver::generateChromosome(int chromosomeSize, int minimumDistance) {
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

void Solver::findNotBurnt(vector<int> chromosome) {
    notBurnt.clear();
    nodeMark = vector<int>(nodes, nodes);
    nodeFillMark = vector<int>(nodes, nodes);
    
    for (int i = 0; i < chromosome.size(); i++)
        for (int node = 0; node < nodes; node++)
            nodeMark[node] = min(nodeMark[node], max(0, ((int)distanceMatrix[chromosome[i]][node] - (burninglength - 1 - i))));

    for (int node = 0; node < nodes; node++)
        if (nodeMark[node] != 0)
            notBurnt.push_back(node);
    
        cout << "Unverbrannt: " << notBurnt.size() << endl;
}

vector<int> Solver::number2sequence(long long number, int sequencelength, int sequenceBase) {
    vector<int> sequence(sequencelength, 0);
    for (int i = 0; number > 0; i++) {
        sequence[i] = number % sequenceBase;
        number /= sequenceBase;
    }
    return sequence;
}

long long Solver::costFunction(vector<int> chromosome, int skipValue) {
    findNotBurnt(chromosome);

    if ((int)notBurnt.size() > skipValue)
        return INF;

    int notBurntSize = (int)notBurnt.size();
    int remaininglength = burninglength - (int)chromosome.size();

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

vector<int> Solver::crossover(vector<int> parentChromosome1, vector<int> parentChromosome2) {
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

vector<int> Solver::mutate(vector<int> chromosome, double mutateProbabilty) {
    mt19937 generator(randomDevice());
    uniform_real_distribution<> mutateSelector(0, 1);
    uniform_int_distribution<>  methodSelector(0, 1);

    for (int i = 0; i < (int)chromosome.size(); i++) {
        if (mutateSelector(generator) <= mutateProbabilty) {
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

int Solver::rouletteWheelSelection(int generation, int topPopulation, double fitnessSum) {
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

vector<int> Solver::solve(int chromosomeSize, int minimumDistance, 
                          int skipValue, int maxGenerations, int topPopulation,
                          int crossoverPopulation, double mutateProbabilty,
                          double alpha, double beta) {
    if (chromosomeSize == -1)
        chromosomeSize = max(burninglength - 3, 2);

    if (minimumDistance == -1)
        minimumDistance = max(burninglength - 3, 2);

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
            vector<int> chromosome = mutate(population[generation + 1][i].second, mutateProbabilty);  
            int fitness = costFunction(chromosome, skipValue);
            population[generation + 1].push_back({fitness, chromosome});
        }
    }
    return bestBurningSequence;
}
