//
// Created by Pedro Bedmar on 21/3/22.
//

#include <iostream>
#include "functions.h"

double avg(vector<double> v) {
    double sum = 0;

    for (auto it = v.cbegin(); it != v.cend(); it++) {
        sum += *it;
    }

    return sum/v.size();
}

double dispersion(vector<vector<double> > distanceMatrix, unordered_set<int> solution) {
    double max = -numeric_limits<double>::max();
    double min = numeric_limits<double>::max();

    for (auto v: solution) {
        double d = 0;
        for (auto v2: solution) {
            d += distanceMatrix[v][v2];
        }
        if (d > max) {
            max = d;
        }
        if (d < min) {
            min = d;
        }
    }

    return max - min;
}

double dispersion(vector<vector<double> > distanceMatrix, vector<int> solution) {
    double max = -numeric_limits<double>::max();
    double min = numeric_limits<double>::max();

    for (auto v: solution) {
        double d = 0;
        for (auto v2: solution) {
            d += distanceMatrix[v][v2];
        }
        if (d > max) {
            max = d;
        }
        if (d < min) {
            min = d;
        }
    }

    return max - min;
}

double dispersion(vector<vector<double> > distanceMatrix, vector<bool> individual) {
    double max = -numeric_limits<double>::max();
    double min = numeric_limits<double>::max();

    for (int i = 0; i<individual.size(); i++) {
        if (individual[i]) {
            double d = 0;

            for (int j = 0; j < individual.size(); j++) {
                if (individual[j]) {
                    d += distanceMatrix[i][j];
                }
            }

            if (d > max) {
                max = d;
            }
            if(d < min) {
                min = d;
            }
        }
    }

    return max - min;
}

vector<int> binaryToNumeric(vector<bool> solution) {
    vector<int> result;
    for (int i=0; i<solution.size(); i++) {
        if (solution[i])
            result.push_back(i);
    }
    cout << endl;

    return result;
}

void printResults(list<double> cost, list<double> time, list<double> lowest, list<double> highest) {
    cout << "Number of cases: " << cost.size() << endl << endl;

    cout << "Cost:" << endl;
    for (auto it = cost.cbegin(); it != cost.cend(); it++) {
        cout << fixed << *it << endl;
    }

    cout << endl;

    cout << "Time:" << endl;
    for (auto it = time.cbegin(); it != time.cend(); it++) {
        cout << fixed << *it << endl;
    }

    cout << endl;

    cout << "Min. cost:" << endl;
    for (auto it = lowest.cbegin(); it != lowest.cend(); it++) {
        cout << fixed << *it << endl;
    }

    cout << endl;

    cout << "Max. cost:" << endl;
    for (auto it = highest.cbegin(); it != highest.cend(); it++) {
        cout << fixed << *it << endl;
    }
}