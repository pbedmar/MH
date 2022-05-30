//
// Created by Pedro Bedmar on 30/5/22.
//

#include <iostream>
#include <math.h>
#include "functions.h"
#include "TrajectoryAlgorithms.h"

TrajectoryAlgorithms::TrajectoryAlgorithms(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_,
                                   int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    rng_gen.seed(seed);

    max_neighbours = 5*numElements;
    max_successes = 0.1*max_neighbours;
    M = MAX_EVAL/max_neighbours;
}

double TrajectoryAlgorithms::getAvgCost() {
    return avg_cost;
}

double TrajectoryAlgorithms::getAvgTime() {
    return avg_time;
}

double TrajectoryAlgorithms::getLowestCost(){
    return lowest_cost;
}

double TrajectoryAlgorithms::getHighestCost(){
    return highest_cost;
}

void TrajectoryAlgorithms::run(int n_times, string memeticType) {

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        vector<int> solution = simulatedAnnealing();

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
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}

void TrajectoryAlgorithms::computeRandomSolution(vector<int>& solution, vector<int>& unselected_items) {
    for (int i = 0; i < numElements; i++) {
        unselected_items.push_back(i);
    }

    shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);

    for (int i = 0; i < numRequiredElements; i++) {
        int element = unselected_items[unselected_items.size()-1];
        unselected_items.pop_back();
        solution.push_back(element);
    }
}

vector<int> TrajectoryAlgorithms::simulatedAnnealing() {
    vector<int> solution;
    vector<int> unselected_items;

    computeRandomSolution(solution, unselected_items);
    vector<int> best_solution = solution;
    double current_cost = dispersion(distanceMatrix, solution);
    double best_cost = current_cost;

    vector<double> sum(numElements, 0);
    for (auto w: solution) {
        for (auto w2: solution) {
            sum[w] += distanceMatrix[w][w2];
        }
    }

    cout << "bestCost: " << best_cost << endl;

    double t_0 = (MU*current_cost) / (-log(PHI));
    double t_f = 0.001;

    double t = t_0;
    double k = 0;
    cout << t << "  " << t_f;
    while (t > t_f && k < M) { //TODO: Es esto así?
        double count_neighbours = 0;
        double count_successes = 0;

        while (count_neighbours < max_neighbours && count_successes < max_successes) {
            // compute neighbour
            uniform_int_distribution<mt19937::result_type> distS(0,numRequiredElements-1);
            uniform_int_distribution<mt19937::result_type> distU(0,numElements-numRequiredElements-1);

            int indexS = distS(rng_gen);
            int indexU = distU(rng_gen);
            int swapS = solution[indexS];
            int swapU = unselected_items[indexU];

            vector<double> delta(numElements, 0);
            double delta_max_w = -numeric_limits<double>::max();
            double delta_min_w = numeric_limits<double>::max();
            for (auto w: solution) {
                delta[w] = sum[w] - distanceMatrix[w][swapS] + distanceMatrix[w][swapU];
                delta[swapU] += distanceMatrix[w][swapU];

                if (delta[w] > delta_max_w) {
                    delta_max_w = delta[w];
                }

                if (delta[w] < delta_min_w) {
                    delta_min_w = delta[w];
                }
            }

            double delta_max = max(delta[swapU], delta_max_w);
            double delta_min = min(delta[swapU], delta_min_w);
            double neighbour_cost = delta_max - delta_min;

            double delta_f = neighbour_cost - current_cost;
            count_neighbours++;  //TODO: here or inside the if below?

            uniform_real_distribution<mt19937::result_type> dist01(0,1);
            if (delta_f<0 || dist01(rng_gen) <= exp(-delta_f/k*t)) {
                solution[indexS] = swapU;
                unselected_items[indexU] = swapS;
                current_cost = neighbour_cost;
                // cout << "Holaaa" << endl;

                if (current_cost < best_cost) {
                    best_solution = solution;
                    best_cost = current_cost;
                    cout << "Caracolaa" << endl;

                    count_successes++;
                }
            }
        }

        double beta = (t_0 - t_f) / (M*t_0*t_f);
        t = t / (1+beta*t);

        k++;
    }
    cout << k << endl;
    cout << "factorized cost: " << best_cost << endl;
    return best_solution;
}

