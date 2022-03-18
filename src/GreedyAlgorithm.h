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
    int executionTime = -1;
    int numElements;
    int numRequiredElements;
    vector<vector<double> > distanceMatrix;

public:
    GreedyAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    void run(int n_times);
    int randInt(int min, int max);
};


#endif //PRACTICAS_GREEDYALGORITHM_H
