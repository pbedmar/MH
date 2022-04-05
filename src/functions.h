//
// Created by Pedro Bedmar on 21/3/22.
//

#ifndef PRACTICAS_FUNCTIONS_H
#define PRACTICAS_FUNCTIONS_H

#include <vector>
#include <list>
#include <unordered_set>
using namespace std;

double avg(vector<double> v);

double dispersion(vector<vector<double> > distanceMatrix, unordered_set<int> solution);

double dispersion(vector<vector<double> > distanceMatrix, vector<int> solution);

void printResults(list<double> cost, list<double> time);

#endif //PRACTICAS_FUNCTIONS_H
