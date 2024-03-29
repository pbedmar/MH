//
// Created by Pedro Bedmar on 30/4/22.
//

#ifndef PRACTICAS_GENETICALGORITHM_H
#define PRACTICAS_GENETICALGORITHM_H

#include <vector>
#include <random>
using namespace std;

class GeneticAlgorithm {
private:
    int seed;
    mt19937 rng_gen;

    int numElements;
    int numRequiredElements;
    const int MAX_EVAL = 100000;
    const int POPULATION_SIZE = 50;
    const double PROB_AGG_CROSSOVER = 0.7;
    const double PROB_MUTATION = 0.1;
    vector<vector<double> > distanceMatrix;

    double avg_time = 0;
    double avg_cost = 0;
    double lowest_cost = numeric_limits<double>::max();
    double highest_cost = -numeric_limits<double>::max();
    vector<bool> generationalModel(string crossoverOperator);
    vector<bool> stationaryModel(string crossoverOperator);
    void generationalSelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion);
    pair<int,int> stationarySelectionOperator(vector<vector<bool> >& population, vector<double>& populationDispersion);
    vector<bool> uniformCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2);
    vector<bool> positionBasedCrossoverOperator(vector<bool>& parent1, vector<bool>& parent2);
    vector<bool> mutationOperator(vector<bool>& individual);

public:
    GeneticAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_ = 1);
    double getAvgTime();
    double getAvgCost();
    double getLowestCost();
    double getHighestCost();
    void run(int n_times, string model, string crossoverOperator);
};


#endif //PRACTICAS_GENETICALGORITHM_H
