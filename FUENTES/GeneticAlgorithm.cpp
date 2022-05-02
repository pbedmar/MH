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

void GeneticAlgorithm::run(int n_times){

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();


        vector<vector<bool> > population;
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
        }

        vector<bool> solution = generationalModel(population);
        double solutionDispersion = dispersion(distanceMatrix, solution);

        double elapsed = (clock()- start_time);
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
//        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << solutionDispersion << ". Solution: ";
//        for (auto it: solution) {
//            cout << it << ",";
//        }
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

vector<bool> GeneticAlgorithm::generationalModel(vector<vector<bool> > lastPopulation) {

    double lastLowerDispersion = numeric_limits<double>::max();
    int lastBestSolutionIndex = -1;
    for (int i = 0; i<POPULATION_SIZE; i++) {
        if (lastLowerDispersion > dispersion(distanceMatrix, lastPopulation[i])) {
            lastBestSolutionIndex = i;
            lastLowerDispersion = dispersion(distanceMatrix, lastPopulation[i]);
        }
    }

    int numEvaluations = 0;
    while (numEvaluations < MAX_EVAL) {
        // generate parents
        vector<vector<bool> > population = generationalSelectionOperator(lastPopulation);

        // crossover (only between the first POPULATION_SIZE*PROB_AGG_CROSSOVER individuals)
        for (int i=0; i<POPULATION_SIZE*PROB_AGG_CROSSOVER; i+=2) {
            vector<bool> parent1 = population[i];
            vector<bool> parent2 = population[i + 1];

            vector<bool> child1 = uniformCrossoverOperator(parent1, parent2);
            vector<bool> child2 = uniformCrossoverOperator(parent1, parent2);

            population[i] = child1;
            population[i + 1] = child2;
        }

        // perform mutations
        for (int i=0; i<POPULATION_SIZE*PROB_MUTATION; i++) { //TODO: Is this correct? Should I mutate the first individuals or shuffle right before?
            population[i] = mutationOperator(population[i]);
        }

        double lowerDispersion = numeric_limits<double>::max();
        double higherDispersion = -numeric_limits<double>::max();
        int bestSolutionIndex = -1;
        int worstSolutionIndex = -1;
        for (int i = 0; i<POPULATION_SIZE; i++) {
            if (lowerDispersion > dispersion(distanceMatrix, population[i])) {
                bestSolutionIndex = i;
                lowerDispersion = dispersion(distanceMatrix, population[i]);
            }
            if (higherDispersion < dispersion(distanceMatrix, population[i])) {
                worstSolutionIndex = i;
                higherDispersion = dispersion(distanceMatrix, population[i]);
            }
        }

        if (lowerDispersion > lastLowerDispersion) { //TODO: why my implementation increases dispersion in some iterations? Should that happen?
            population[worstSolutionIndex] = lastPopulation[lastBestSolutionIndex];
            lastBestSolutionIndex = worstSolutionIndex;
        } else {
            lastLowerDispersion = lowerDispersion;
            lastBestSolutionIndex = bestSolutionIndex;
        }
        lastPopulation = population;

        cout << "Evaluation " << numEvaluations << ": " << dispersion(distanceMatrix, lastPopulation[lastBestSolutionIndex]) << endl;

        numEvaluations++;
    }

    for (const auto i: lastPopulation) {
        vector<int> numericSolution = binaryToNumeric(i);
        for (auto c: numericSolution) {
            cout << c << ",";
        }
        cout << endl;
    }

    return lastPopulation[lastBestSolutionIndex];
}

vector<vector<bool> > GeneticAlgorithm::generationalSelectionOperator(vector<vector<bool> > population) {
    uniform_int_distribution<mt19937::result_type> dist(0,POPULATION_SIZE-1);

    vector<vector<bool> > parentsPopulation;

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
        if (dispersion(distanceMatrix, population[bestRandElem]) >
            dispersion(distanceMatrix, population[secondRandElem])) {
            bestRandElem = secondRandElem;
        }

        // add it to parents' population
        parentsPopulation.push_back(population[bestRandElem]);
    }

    return parentsPopulation;
}

vector<bool> GeneticAlgorithm::uniformCrossoverOperator(vector<bool> parent1, vector<bool> parent2) {
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
    int countAvg = 0;
    double avg = 0;
    for (int i=0; i<numElements; i++) {
        if (child[i]) {
            countNbTrues++;
        }
        for (int j=0; j<numElements; j++) { // TODO: until numElements or i?
            if (child[i] && child[j]) {
                countAvg++;
                avg += distanceMatrix[i][j];
            }
        }
    }

    if (countAvg > 0) {
        avg /= countAvg;
    }

    // if there are more trues than required
    while (countNbTrues > numRequiredElements) {
        double max = -numeric_limits<double>::max();
        int maxPosition = -1;

        // get true more distant to the average
        for (int i=0; i<numElements; i++) {
            if (child[i]) {
                double sum = 0;

                for (int j = 0; j < numElements; j++) {
                    sum += distanceMatrix[i][j] - avg;
                }

                if (sum > max) {
                    max = sum;
                    maxPosition = i;
                }
            }
        }

        // convert that true to false. TODO: Here or down?
        child[maxPosition] = false;

        // update average
        avg *= countAvg;
        for (int i=0; i<numElements; i++) {
            if (child[i]) {
                countAvg--;
                avg -= distanceMatrix[maxPosition][i];
            }
        }
        if (countAvg > 0) {
            avg /= countAvg;
        }
        // decrease number of trues
        countNbTrues--;
    }

    //if there are less trues than required
    while (countNbTrues < numRequiredElements) {
        double min = numeric_limits<double>::max();
        int minPosition = -1;

        // get false closer to the average
        for (int i=0; i<numElements; i++) {
            if (!child[i]) {
                double sum = 0;

                for (int j = 0; j < numElements; j++) {
                    sum += distanceMatrix[i][j] - avg;
                }

                if (sum < min) {
                    min = sum;
                    minPosition = i;
                }
            }
        }

        // convert that false to true. TODO: Here or down?
        child[minPosition] = true;

        // update average
        avg *= countAvg;
        for (int i=0; i<numElements; i++) {
            if (!child[i]) { // TODO: Negation or as it is?
                countAvg++;
                avg += distanceMatrix[minPosition][i];
            }
        }
        if (countAvg > 0) {
            avg /= countAvg;
        }

        // increase number of trues
        countNbTrues++;
    }

    return child;
}

vector<bool> GeneticAlgorithm::mutationOperator(vector<bool> individual) {
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
