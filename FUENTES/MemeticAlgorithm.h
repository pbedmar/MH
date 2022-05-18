//
// Created by Pedro Bedmar on 9/5/22.
//

#ifndef PRACTICAS_MEMETICALGORITHM_H
#define PRACTICAS_MEMETICALGORITHM_H

#include <vector>
#include <unordered_set>
#include <random>
using namespace std;

class MemeticAlgorithm {
private:
    int seed;
    mt19937 rng_gen;

    int numElements;
    int numRequiredElements;
    const int MAX_EVAL = 100000;
    const int MAX_EVAL_BL = 400;
    const int POPULATION_SIZE = 50;
    const double PROB_AGE_CROSSOVER = 1;
    const double PROB_MUTATION = 0.1;
    vector<vector<double> > distanceMatrix;

    double avg_time = 0;
    double avg_cost = 0;
    double lowest_cost = numeric_limits<double>::max();
    double highest_cost = -numeric_limits<double>::max();

    vector<bool> stationaryModel(string memeticType);
    pair<int,int> stationarySelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion);
    vector<bool> uniformCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2);
    vector<bool> mutationOperator(vector<bool>& individual);
    void localSearch(vector<int>& unselected_items, vector<int>& solution, double& solutionDispersion, int& eval);
    void localSearchExecution(vector<bool>& individual, double& dispersion, int& numEvaluations);

public:
    MemeticAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    double getLowestCost();
    double getHighestCost();
    void run(int n_times, string memeticType);
};


#endif //PRACTICAS_MEMETICALGORITHM_H
