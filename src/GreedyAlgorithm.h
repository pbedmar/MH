//
// Created by Pedro Bedmar on 16/3/22.
//

#ifndef PRACTICAS_GREEDYALGORITHM_H
#define PRACTICAS_GREEDYALGORITHM_H

#include <vector>
using namespace std;

class GreedyAlgorithm {
private:
    int seed;
    int numElements;
    int numRequiredElements;
    vector<vector<double> > distanceMatrix;

    double avg_time = 0;
    double avg_cost = 0;

public:
    GreedyAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    void run(int n_times);
};


#endif //PRACTICAS_GREEDYALGORITHM_H