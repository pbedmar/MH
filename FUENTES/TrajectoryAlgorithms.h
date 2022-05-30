//
// Created by Pedro Bedmar on 30/5/22.
//

#ifndef PRACTICAS_TRAJECTORYALGORITHMS_H
#define PRACTICAS_TRAJECTORYALGORITHMS_H

#include <vector>
#include <random>
using namespace std;

class TrajectoryAlgorithms {
private:
    int seed;
    mt19937 rng_gen;

    int numElements;
    int numRequiredElements;
    vector<vector<double> > distanceMatrix;

    double mu = 0.3;
    double phi = 0.3;
    const int MAX_NEIGHBOURS = 5*numElements;
    const int MAX_SUCCESSES = 0.1*MAX_NEIGHBOURS;
    const int MAX_EVAL = 100000;
    const int M = MAX_EVAL/MAX_NEIGHBOURS;

    double avg_time = 0;
    double avg_cost = 0;
    double lowest_cost = numeric_limits<double>::max();
    double highest_cost = -numeric_limits<double>::max();

    void computeRandomSolution(vector<int>& solution, vector<int>& unselected_items);
    vector<int> simulatedAnnealing();

public:
    TrajectoryAlgorithms(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    double getLowestCost();
    double getHighestCost();
    void run(int n_times, string memeticType);
};


#endif //PRACTICAS_TRAJECTORYALGORITHMS_H
