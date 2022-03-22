//
// Created by Pedro Bedmar on 18/3/22.
//

#include <iostream>
#include <unordered_set>
#include "LocalSearchAlgorithm.h"
#include "functions.h"

LocalSearchAlgorithm::LocalSearchAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_,
                                           int numRequiredElements_, int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    srand(seed);
}

double LocalSearchAlgorithm::getAvgCost() {
    return avg_cost;
}

double LocalSearchAlgorithm::getAvgTime() {
    return avg_time;
}

void LocalSearchAlgorithm::run(int n_times) {

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        unordered_set<int> solution;
        unordered_set<int> unselected_items;
        vector<double> sum(numElements, 0);
        double current_cost = numeric_limits<double>::max(); //TODO

        for (int i = 0; i < numElements; i++) {
            unselected_items.insert(i);
        }

        for (int i = 0; i < numRequiredElements; i++) {
            int next_val = -1;

            do {
                next_val = randInt(0, numElements);
            } while (solution.find(next_val) != solution.end());

            solution.insert(next_val);
            unselected_items.erase(next_val);
        }

        for (auto w: solution) {
            for (auto w2: solution) {
                sum[w] += distanceMatrix[w][w2]; //TODO: What is anterior(w)? Am I initializing this correctly for the base case?
            }
        }

        bool better_solution = true;
        int eval = 0;

        while (eval < MAX_EVAL && better_solution) {

            eval = 0;
            better_solution = false;

            cout << "(";
            for (auto i: solution)
                cout << i << " ";
            cout << ")" << endl << "New solution evals: ";

            for (auto u = solution.begin(); u != solution.end() && !better_solution; u++) {
                for (auto v = unselected_items.begin(); v != unselected_items.end() && !better_solution; v++) {

                    vector<double> delta(numElements, 0);

                    double delta_w_max = -numeric_limits<double>::max();
                    double delta_w_min = numeric_limits<double>::max();

                    for (auto w = solution.begin(); w != solution.end(); w++) {
                        delta[*w] = sum[*w] - distanceMatrix[*w][*u] + distanceMatrix[*w][*v]; //TODO: Anterior
                        delta[*v] += distanceMatrix[*v][*w];

                        if (delta[*w] > delta_w_max) {
                            delta_w_max = delta[*w];
                        }

                        if (delta[*w] < delta_w_min) {
                            delta_w_min = delta[*w];
                        }
                    }

                    double delta_max = max(delta[*v], delta_w_max);
                    double delta_min = min(delta[*v], delta_w_min);
                    double new_cost = (delta_max - delta_min) - current_cost;

                    eval++;

                    cout << eval << " ";

                    if (new_cost < 0) {
                        sum = delta;
                        current_cost = new_cost;
                        better_solution = true;

                        int last_u = *u;
                        int last_v = *v;
                        solution.erase(last_u);
                        solution.insert(last_v);
                        unselected_items.erase(last_v);
                        unselected_items.insert(last_u);
                    }
                }
            }
        }

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += current_cost;

        //print iteration results
        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << current_cost << ". Solution: ";
        for (auto it = solution.cbegin(); it != solution.cend(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}

double inter(vector<double> solution, int i, int j) {

}