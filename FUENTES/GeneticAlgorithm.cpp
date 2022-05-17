//
// Created by Pedro Bedmar on 30/4/22.
//

#include <iostream>
#include "functions.h"
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_,
                                   int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    rng_gen.seed(seed);
}

double GeneticAlgorithm::getAvgCost() {
    return avg_cost;
}

double GeneticAlgorithm::getAvgTime() {
    return avg_time;
}

double GeneticAlgorithm::getLowestCost(){
    return lowest_cost;
}

double GeneticAlgorithm::getHighestCost(){
    return highest_cost;
}

void GeneticAlgorithm::run(int n_times, string model, string crossoverOperator){

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        vector<vector<bool> > population;
        vector<double> populationDispersion;
        for (int i=0; i<POPULATION_SIZE; i++) {
            vector<bool> individual;

            for (int j=0; j<numRequiredElements; j++) {
                individual.push_back(true);
            }
            for (int j=0; j<(numElements-numRequiredElements); j++) {
                individual.push_back(false);
            }

            shuffle(individual.begin(), individual.end(), rng_gen);
            population.push_back(individual);
            populationDispersion.push_back(dispersion(distanceMatrix, individual));
        }

        vector<bool> solution;
        if (model == "gen") {
            solution = generationalModel(population, populationDispersion, crossoverOperator);
        } else if (model == "est") {
            solution = stationaryModel(population, populationDispersion, crossoverOperator);
        }

        double solutionDispersion = dispersion(distanceMatrix, solution);

        double elapsed = (clock() - start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += solutionDispersion;

        if (solutionDispersion < lowest_cost) {
            lowest_cost = solutionDispersion;
        }
        if (solutionDispersion > highest_cost) {
            highest_cost = solutionDispersion;
        }


        //print iteration results
        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << solutionDispersion << ". Solution: ";
        for (auto it: solution) {
            cout << it << ",";
        }
        cout << endl;
//        cout << "Solution not binary: " ;
//        for (int i=0; i<numElements; i++) {
//            if (solution[i])
//                cout << i << ",";
//        }
//        cout << endl;
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}

vector<bool> GeneticAlgorithm::generationalModel(vector<vector<bool> >& population, vector<double>& populationDispersion, string crossoverOperator) {

    double lastBestSolutionDispersion = numeric_limits<double>::max();
    vector<bool> lastBestSolution;
    for (int i = 0; i<POPULATION_SIZE; i++) {
        if (lastBestSolutionDispersion > populationDispersion[i]) {
            lastBestSolution = population[i];
            lastBestSolutionDispersion = populationDispersion[i];
        }
    }

    int numEvaluations = 50;
    while (numEvaluations < MAX_EVAL) {
        // generate parents
        population = generationalSelectionOperator(population, populationDispersion);

        // crossover (only between the first POPULATION_SIZE*PROB_AGG_CROSSOVER individuals)
        for (int i=0; i<POPULATION_SIZE*PROB_AGG_CROSSOVER; i+=2) {
            vector<bool> parent1 = population[i];
            vector<bool> parent2 = population[i + 1];

            vector<bool> child1;
            vector<bool> child2;
            if (crossoverOperator == "uni") {
                child1 = uniformCrossoverOperator(parent1, parent2);
                child2 = uniformCrossoverOperator(parent1, parent2);
            } else if (crossoverOperator == "pos") {
                child1 = positionBasedCrossoverOperator(parent1, parent2);
                child2 = positionBasedCrossoverOperator(parent1, parent2);
            }

            population[i] = child1;
            population[i + 1] = child2;
        }

        // perform mutations
        for (int i=0; i<POPULATION_SIZE*PROB_MUTATION; i++) { //TODO: Is this correct? Should I directly mutate the first individuals or shuffle right before?
            population[i] = mutationOperator(population[i]);
        }

        for (int i = 0; i < POPULATION_SIZE; i++) {
            populationDispersion[i] = dispersion(distanceMatrix, population[i]);
            numEvaluations++;
        }

        double lowerDispersion = numeric_limits<double>::max();
        double higherDispersion = -numeric_limits<double>::max();
        int bestSolutionIndex = -1;
        int worstSolutionIndex = -1;
        for (int i = 0; i<POPULATION_SIZE; i++) {
            if (lowerDispersion > populationDispersion[i]) {
                bestSolutionIndex = i;
                lowerDispersion = populationDispersion[i];
            }
            if (higherDispersion < populationDispersion[i]) {
                worstSolutionIndex = i;
                higherDispersion = populationDispersion[i];
            }
        }

        population[worstSolutionIndex] = lastBestSolution;
        populationDispersion[worstSolutionIndex] = lastBestSolutionDispersion;

        lastBestSolution = population[bestSolutionIndex];
        lastBestSolutionDispersion = lowerDispersion;

//        cout << "Evaluation " << numEvaluations << ": " << dispersion(distanceMatrix, lastBestSolution) << endl;
    }

//    for (const auto i: lastPopulation) {
//        vector<int> numericSolution = binaryToNumeric(i);
//        for (auto c: numericSolution) {
//            cout << c << ",";
//        }
//        cout << endl;
//    }

    return lastBestSolution;
}

vector<bool> GeneticAlgorithm::stationaryModel(vector<vector<bool> >& population, vector<double>& populationDispersion, string crossoverOperator) {
    int numEvaluations = 50;
    vector <bool> bestIndividual;

    while (numEvaluations < MAX_EVAL) {
        // generate parents
        pair<int,int> parentsIndex = stationarySelectionOperator(population, populationDispersion);

        // crossover (only between the two parents already selected)
        vector<bool> child1;
        vector<bool> child2;
        if (crossoverOperator == "uni") {
            child1 = uniformCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);
            child2 = uniformCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);
        } else if (crossoverOperator == "pos") {
            child1 = positionBasedCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);
            child2 = positionBasedCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);
        }

        uniform_int_distribution<mt19937::result_type> dist(0,100*PROB_MUTATION-1);
        if (dist(rng_gen) == 0) {
            child1 = mutationOperator(child1);
        }
        if (dist(rng_gen) == 0) {
            child2 = mutationOperator(child2);
        }

        // compute two worst individuals in the population
        double higherDispersion = -numeric_limits<double>::max();
        double secondHigherDispersion = -numeric_limits<double>::max();
        int worstIndividualIndex = -1;
        int secondWorstIndividualIndex = -1;
        for (int i=0; i<POPULATION_SIZE; i++) {
            double disp = populationDispersion[i];
            if (higherDispersion < disp) {
                higherDispersion = disp;
                worstIndividualIndex = i;
            }
        }
        for (int i=0; i<POPULATION_SIZE; i++) {
            double disp = populationDispersion[i];
            if (secondHigherDispersion < disp && i!=worstIndividualIndex) {
                secondHigherDispersion = disp;
                secondWorstIndividualIndex = i;
            }
        }

        // replace the old population's worst individuals with the new children (only if they are better)
        double dispChild1 = dispersion(distanceMatrix, child1);
        double dispChild2 = dispersion(distanceMatrix, child2);
        numEvaluations = numEvaluations+2;
        if (dispChild1 < dispChild2) {
            if (dispChild2 < higherDispersion) {
                population[worstIndividualIndex] = child2;
                populationDispersion[worstIndividualIndex] = dispChild2;

                population[secondWorstIndividualIndex] = child1;
                populationDispersion[secondWorstIndividualIndex] = dispChild1;

            } else if (dispChild1 < higherDispersion) {
                population[worstIndividualIndex] = child1;
                populationDispersion[worstIndividualIndex] = dispChild1;
            }
        } else {
            if (dispChild1 < higherDispersion) {
                population[worstIndividualIndex] = child1;
                populationDispersion[worstIndividualIndex] = dispChild1;

                population[secondWorstIndividualIndex] = child2;
                populationDispersion[secondWorstIndividualIndex] = dispChild2;

            } else if (dispChild2 < higherDispersion) {
                population[worstIndividualIndex] = child2;
                populationDispersion[worstIndividualIndex] = dispChild2;
            }
        }

        double lowerDispersion = numeric_limits<double>::max();
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double disp = populationDispersion[i];
            if (disp < lowerDispersion) {
                lowerDispersion = disp;
                bestIndividual = population[i];
            }
        }
//        cout << "Evaluation " << numEvaluations << ": " << lowerDispersion << endl;
    }

    return bestIndividual;
}

vector<vector<bool> > GeneticAlgorithm::generationalSelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion) {
    uniform_int_distribution<mt19937::result_type> dist(0,POPULATION_SIZE-1);
    vector<vector<bool> > parentsPopulation;
    vector<double> parentsPopulationDispersion;

    // as many binary tournaments as individuals in the population
    for (int i=0; i<POPULATION_SIZE; i++) {
        // get two individuals randomly
        int firstRandElem = dist(rng_gen);
        int secondRandElem = dist(rng_gen);
        while (firstRandElem == secondRandElem) {
            secondRandElem = dist(rng_gen);
        }

        // choose the best between the two
        int bestRandElem = firstRandElem;
        if (populationDispersion[bestRandElem] > populationDispersion[secondRandElem]) {
            bestRandElem = secondRandElem;
        }

        // add it to parents' population
        parentsPopulation.push_back(population[bestRandElem]);
        parentsPopulationDispersion.push_back(populationDispersion[bestRandElem]);

    }

    populationDispersion = parentsPopulationDispersion;
    return parentsPopulation;
}

pair<int,int> GeneticAlgorithm::stationarySelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion) {
    uniform_int_distribution<mt19937::result_type> dist(0,POPULATION_SIZE-1);
    pair<int,int> parents;

    // only two binary tournaments

    // get two individuals randomly
    int firstRandElem = dist(rng_gen);
    int secondRandElem = dist(rng_gen);
    while (firstRandElem == secondRandElem) {
        secondRandElem = dist(rng_gen);
    }

    // choose the best between the two
    int bestRandElem = firstRandElem;
    if (populationDispersion[bestRandElem] >
        populationDispersion[secondRandElem]) {
        bestRandElem = secondRandElem;
    }

    parents.first = bestRandElem;

    // get two individuals randomly
    firstRandElem = dist(rng_gen);
    secondRandElem = dist(rng_gen);
    while (firstRandElem == secondRandElem) {
        secondRandElem = dist(rng_gen);
    }

    // choose the best between the two
    bestRandElem = firstRandElem;
    if (populationDispersion[bestRandElem] >
        populationDispersion[secondRandElem]) {
        bestRandElem = secondRandElem;
    }

    // add it to parents' population
    parents.second = bestRandElem;

    return parents;
}

vector<bool> GeneticAlgorithm::uniformCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2) {
    uniform_int_distribution<mt19937::result_type> dist(0,1); //TODO: how should I generate random numbers to decide? like this?
    vector<bool> child(parent1);

    for (int i = 0; i<numElements; i++) {
        if (parent1[i] != parent2[i]) {
            child[i] = dist(rng_gen);
        }
    }

    // repairment process
    // compute the number of trues in the vector and the average distance
    int countNbTrues = 0;
    for (int i=0; i<numElements; i++) {
        if (child[i]) {
            countNbTrues++;
        }
    }

    // if there are more trues than required
    while (countNbTrues > numRequiredElements) {
        // compute accumulated distances and average
        vector<double> accDistance;
        for (int i=0; i<numElements; i++) {
            double sum = 0;
            for(int j=0; j<numElements; j++) {
                if (child[i] && child[j]) {
                    sum += distanceMatrix[i][j];
                }
                accDistance.push_back(sum);
            }
        }

        double avg = 0;
        for (auto i: accDistance) {
            avg += i;
        }
        avg = avg / accDistance.size();

        // get true more distant to the average
        double max = -numeric_limits<double>::max();
        int maxPosition = -1;
        for (int i=0; i<numElements; i++) {
            if (child[i]) {
                double sum = 0;

                for (int j = 0; j < numElements; j++) {
                    sum += distanceMatrix[i][j];
                }

                if (fabs(sum-avg) > max) {
                    max = fabs(sum-avg);
                    maxPosition = i;
                }
            }
        }

        // convert that true to false. TODO: Here or below?
        child[maxPosition] = false;

        // decrease number of trues
        countNbTrues--;
    }

    //if there are fewer trues than required
    while (countNbTrues < numRequiredElements) {
        // compute accumulated distances and average
        vector<double> accDistance;
        for (int i=0; i<numElements; i++) {
            double sum = 0;
            for(int j=0; j<numElements; j++) {
                if (child[i] && child[j]) {
                    sum += distanceMatrix[i][j];
                }
                accDistance.push_back(sum);
            }
        }

        double avg = 0;
        for (auto i: accDistance) {
            avg += i;
        }
        avg = avg / accDistance.size();

        // get false closer to the average
        double min = numeric_limits<double>::max();
        int minPosition = -1;
        for (int i=0; i<numElements; i++) {
            if (!child[i]) {
                double sum = 0;

                for (int j = 0; j < numElements; j++) {
                    sum += distanceMatrix[i][j];
                }

                if (fabs(sum-avg) < min) {
                    min = fabs(sum-avg);
                    minPosition = i;
                }
            }
        }

        // convert that false to true. TODO: Here or down?
        child[minPosition] = true;

        // increase number of trues
        countNbTrues++;
    }

    return child;
}

vector<bool> GeneticAlgorithm::positionBasedCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2) {
    vector<bool> child(parent1);

    vector <int> notEqualIndexes;
    for (int i=0; i<numElements; i++) {
        if (parent1[i] != parent2[i]) {
            notEqualIndexes.push_back(i);
        }
    }

    shuffle(notEqualIndexes.begin(), notEqualIndexes.end(), rng_gen);
    int pos = 0;
    for (int i=0; i<numElements; i++) {
        if (parent1[i] != parent2[i]) {
            child[i] = parent1[notEqualIndexes[pos]];
            pos++;
        }
    }

    return child;
}

vector<bool> GeneticAlgorithm::mutationOperator(vector<bool>& individual) {
    uniform_int_distribution<mt19937::result_type> dist(0, numElements-1);

    int position = dist(rng_gen);
    int position2 = dist(rng_gen);
    bool value = individual[position];
    bool value2 = individual[position2];

    while (position==position2 || value==value2) {
        position2 = dist(rng_gen);
        value2 = individual[position2];
    }

    individual[position] = !value;
    individual[position2] = !value2;

    return individual;
}
