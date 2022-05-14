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

void MemeticAlgorithm::run(int n_times) {
    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;
//
//        avg_time += elapsed_in_seconds;
//        avg_cost += solutionDispersion;
//
//        if (solutionDispersion < lowest_cost) {
//            lowest_cost = solutionDispersion;
//        }
//        if (solutionDispersion > highest_cost) {
//            highest_cost = solutionDispersion;
//        }
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}