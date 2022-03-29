//
// Created by Pedro Bedmar on 21/3/22.
//

#include <iostream>
#include <random>
#include "functions.h"

double avg(vector<double> v) {
    double sum = 0;

    for (auto it = v.cbegin(); it != v.cend(); it++) {
        sum += *it;
    }

    return sum/v.size();
}

void printResults(list<double> cost, list<double> time) {
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
}