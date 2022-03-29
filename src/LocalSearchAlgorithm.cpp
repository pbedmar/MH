//
// Created by Pedro Bedmar on 18/3/22.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include "LocalSearchAlgorithm.h"
#include "functions.h"

LocalSearchAlgorithm::LocalSearchAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_,
                                           int numRequiredElements_, int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    rng_gen.seed(seed);
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

        vector<int> solution;
        vector<int> unselected_items;
        vector<double> sum(numElements, 0);

        for (int i = 0; i < numElements; i++) {
            unselected_items.push_back(i);
        }

        shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);

        for (int i = 0; i < numRequiredElements; i++) {
            solution.push_back(unselected_items[unselected_items.size()-1]);
            unselected_items.pop_back();
        }


        double current_cost = 0;
        for (auto w: solution) {
            for (auto w2: solution) {
                sum[w] += distanceMatrix[w][w2]; //TODO: What is anterior(w)? Am I initializing this correctly for the base case?
                current_cost += distanceMatrix[w][w2];
            }
        }

        int eval = 0;

        while (eval < MAX_EVAL) {

            bool better_solution = false;

//            for (auto u: solution) {
//                cout << u << " ";
//            }
//
//            cout << endl;
//            for (auto u: unselected_items) {
//                cout << u << " ";
//            }
//            cout << endl << endl;

            int swap = solution[solution.size() - 1];
            solution.pop_back();
            solution.push_back(unselected_items[unselected_items.size() - 1]);
            unselected_items.pop_back();
            unselected_items.push_back(swap);

            shuffle(solution.begin(), solution.end(), rng_gen);
            shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);

            auto u = solution.begin();
            auto v = unselected_items.begin();

            while (u != solution.end() && !better_solution && eval < MAX_EVAL) {
                while (v != unselected_items.end() && !better_solution && eval < MAX_EVAL) {

                    vector<double> delta(numElements, 0);

                    double delta_w_max = -numeric_limits<double>::max();
                    double delta_w_min = numeric_limits<double>::max();

                    for (auto w = solution.begin(); w != solution.end(); w++) {
                        if (*w != *u) {
                            delta[*w] = sum[*w] - distanceMatrix[*w][*u] + distanceMatrix[*w][*v]; //TODO: Anterior
                            delta[*v] += distanceMatrix[*v][*w];

                            if (delta[*w] > delta_w_max) {
                                delta_w_max = delta[*w];
                            }

                            if (delta[*w] < delta_w_min) {
                                delta_w_min = delta[*w];
                            }
                        }
                    }

                    double delta_max = max(delta[*v], delta_w_max);
                    double delta_min = min(delta[*v], delta_w_min);
                    double new_cost = delta_max - delta_min;

                    if (new_cost < current_cost) {
                        sum = delta;
                        current_cost = new_cost;
                        better_solution = true;
                        eval = 0;

                        int last_u = *u;
                        int last_v = *v;
                        solution.erase(u);
                        solution.push_back(last_v);
                        unselected_items.erase(v);
                        unselected_items.push_back(last_u);
                    }
                    eval++;
                    v++;
                }
                u++;
            }
        }

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += current_cost;

        //print iteration results
//        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << current_cost << ". Solution: ";
//        for (auto it = solution.cbegin(); it != solution.cend(); it++) {
//            cout << *it << " ";
//        }
//        cout << endl;
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}