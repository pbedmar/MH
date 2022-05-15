//
// Created by Pedro Bedmar on 9/5/22.
//

#include <iostream>
#include "functions.h"
#include "MemeticAlgorithm.h"

MemeticAlgorithm::MemeticAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_,
                                   int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    rng_gen.seed(seed);
}

double MemeticAlgorithm::getAvgCost() {
    return avg_cost;
}

double MemeticAlgorithm::getAvgTime() {
    return avg_time;
}

double MemeticAlgorithm::getLowestCost(){
    return lowest_cost;
}

double MemeticAlgorithm::getHighestCost(){
    return highest_cost;
}

void MemeticAlgorithm::run(int n_times, string memeticType) {
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

        vector<bool> solution = stationaryModel(population, populationDispersion, memeticType);

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

vector<bool> MemeticAlgorithm::stationaryModel(vector<vector<bool> >& population, vector<double>& populationDispersion, string memeticType) {
    int numEvaluations = 0;
    vector <bool> bestIndividual;

    while (numEvaluations < MAX_EVAL) {
        // generate parents
        pair<int,int> parentsIndex = stationarySelectionOperator(population, populationDispersion);

        // crossover (only between the two parents already selected)
        vector<bool> child1;
        vector<bool> child2;

        child1 = uniformCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);
        child2 = uniformCrossoverOperator(population[parentsIndex.first], population[parentsIndex.second]);

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
        if (dispChild1 < dispChild2) {
            if (dispChild2 < secondHigherDispersion) {
                population[secondWorstIndividualIndex] = child2;
                populationDispersion[secondWorstIndividualIndex] = dispChild2;

                population[worstIndividualIndex] = child1;
                populationDispersion[worstIndividualIndex] = dispChild1;

            } else if (dispChild1 < secondHigherDispersion) {
                population[secondWorstIndividualIndex] = child1;
                populationDispersion[secondWorstIndividualIndex] = dispChild1;
            }
        }

//        cout << "Evaluation " << numEvaluations << ": " << lowerDispersion << endl;
        numEvaluations++;

        if (numEvaluations % 10 == 0) {
            if (memeticType == "AM1.0") {
                for (int i = 0; i < POPULATION_SIZE; i++) {
//                    cout << endl;
//                    for (auto i: binaryToNumeric(population[i])) {
//                        cout << i << ",";
//                    }
//                    cout << endl;
                    localSearchExecution(population[i], populationDispersion[i], numEvaluations);
//                    cout << endl;
//                    for (auto i: binaryToNumeric(population[i])) {
//                        cout << i << ",";
//                    }
//                    cout << endl;
//                    exit(0);
                }
            } else if (memeticType == "AM0.1") {
                uniform_int_distribution<mt19937::result_type> dist2(0,POPULATION_SIZE-1);
                unordered_set<int> chosenIndividuals;

                for (int i = 0; i < POPULATION_SIZE*0.1; i++) {
                    int individualIndex;
                    while (chosenIndividuals.size() <= i) {
                        individualIndex = dist2(rng_gen);
                        chosenIndividuals.insert(individualIndex);
                    }

                    localSearchExecution(population[individualIndex], populationDispersion[individualIndex], numEvaluations);
                }
            } else if (memeticType == "AM0.1mej") {
                unordered_set<int> chosenIndividuals;
                double lowerDispersion = numeric_limits<double>::max();
                int lowerDispersionIndex = -1;

                for (int i = 0; i < POPULATION_SIZE*0.1; i++) {
                    if (populationDispersion[i] < lowerDispersion && chosenIndividuals.find(i) == chosenIndividuals.end()) {
                        lowerDispersion = populationDispersion[i];
                        lowerDispersionIndex = i;
                    }
                }

                chosenIndividuals.insert(lowerDispersionIndex);
                localSearchExecution(population[lowerDispersionIndex], populationDispersion[lowerDispersionIndex], numEvaluations);
            }
        }
    }

    double lowerDispersion = numeric_limits<double>::max();
    for (auto i: population) {
        double disp = dispersion(distanceMatrix, i);
        if (disp < lowerDispersion) {
            lowerDispersion = disp;
            bestIndividual = i;
        }
    }

    return bestIndividual;
}

pair<int,int> MemeticAlgorithm::stationarySelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion) {
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

vector<bool> MemeticAlgorithm::uniformCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2) {
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

    //if there are fewer trues than required
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

vector<bool> MemeticAlgorithm::mutationOperator(vector<bool>& individual) {
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

void MemeticAlgorithm::localSearch(vector<int>& unselected_items, vector<int>& solution, double& solutionDispersion, int& eval) {
    vector<int> best_solution = solution;
    double current_cost = solutionDispersion;
    double best_cost = current_cost;

    bool better_solution = true;

    vector<double> sum(numElements, 0);
    for (auto w: solution) {
        for (auto w2: solution) {
            sum[w] += distanceMatrix[w][w2];
        }
    }

    int max_eval_local = eval + MAX_EVAL_BL;

    while (eval < MAX_EVAL && eval < max_eval_local && better_solution) {

        better_solution = false;

        auto u = solution.begin();
        while (u != solution.end() && !better_solution && eval < MAX_EVAL && eval < max_eval_local) {
            auto v = unselected_items.begin();
            while (v != unselected_items.end() && !better_solution && eval < MAX_EVAL && eval < max_eval_local) {

                eval++;

                vector<double> delta(numElements, 0);
                double delta_max_w = -numeric_limits<double>::max();
                double delta_min_w = numeric_limits<double>::max();

                for (auto w: solution) {
                    if (w != *u) {
                        delta[w] = sum[w] - distanceMatrix[w][*u] + distanceMatrix[w][*v];
                        delta[*v] += distanceMatrix[w][*v];

                        if (delta[w] > delta_max_w) {
                            delta_max_w = delta[w];
                        }

                        if (delta[w] < delta_min_w) {
                            delta_min_w = delta[w];
                        }
                    }
                }

                double delta_max = max(delta[*v], delta_max_w);
                double delta_min = min(delta[*v], delta_min_w);
                double new_cost = delta_max - delta_min;

                if (new_cost < current_cost) {
                    best_cost = new_cost;
                    current_cost = new_cost;
                    sum = delta;

                    int u_value = *u;
                    int v_value = *v;
                    *u = v_value;
                    *v = u_value;

                    better_solution = true;
                    best_solution = solution;
                }
                v++;
            }
            u++;
        }

        shuffle(solution.begin(), solution.end(), rng_gen);
        shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);
    }

    solution = best_solution;
    solutionDispersion = best_cost;
}

void MemeticAlgorithm::localSearchExecution(vector<bool>& individual, double& dispersion, int& numEvaluations) {
    vector<int> solution;
    vector<int> unselectedItems;

    for (int j=0; j<numElements; j++) {
        if (individual[j])
            solution.push_back(j);
        else
            unselectedItems.push_back(j);
    }

    localSearch(unselectedItems, solution, dispersion, numEvaluations);

    individual = vector<bool>(numElements, false);
    for (int j=0; j<numRequiredElements; j++) {
        individual[solution[j]] = true;
    }
}