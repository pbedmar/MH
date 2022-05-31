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

    const int MAX_EVAL = 100000;
    const double MU = 0.3;
    const double PHI = 0.3;
    int max_neighbours;
    int max_successes;
    int M;

    double avg_time = 0;
    double avg_cost = 0;
    double lowest_cost = numeric_limits<double>::max();
    double highest_cost = -numeric_limits<double>::max();

    void computeRandomSolution(vector<int>& unselected_items, vector<int>& solution);
    vector<int> simulatedAnnealingExecution();
    void simulatedAnnealing(vector<int>& unselected_items, vector<int>& solution, double& current_cost, int MAX_EVAL);
    void localSearch(vector<int>& unselected_items, vector<int>& solution, double& solutionDispersion, int MAX_EVAL);
    vector<int> BMB();
    vector<int> ILS();
    vector<int> ILS_ES();

public:
    TrajectoryAlgorithms(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    double getLowestCost();
    double getHighestCost();
    void run(int n_times, string memeticType);
};


#endif //PRACTICAS_TRAJECTORYALGORITHMS_H
