//
// Created by Pedro Bedmar on 9/5/22.
//

#ifndef PRACTICAS_MEMETICALGORITHM_H
#define PRACTICAS_MEMETICALGORITHM_H

#include <vector>
#include <random>
using namespace std;

class MemeticAlgorithm {
private:
    int seed;
    mt19937 rng_gen;

    int numElements;
    int numRequiredElements;
    const int MAX_EVAL = 10000;
    const int POPULATION_SIZE = 50;
    vector<vector<double> > distanceMatrix;

    double avg_time = 0;
    double avg_cost = 0;
    double lowest_cost = numeric_limits<double>::max();
    double highest_cost = -numeric_limits<double>::max();

public:
    MemeticAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    double getLowestCost();
    double getHighestCost();
    void run(int n_times);
};


#endif //PRACTICAS_MEMETICALGORITHM_H
