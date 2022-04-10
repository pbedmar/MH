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
        vector<int> best_solution;
        vector<int> unselected_items;

        double current_cost = 0;
        double best_cost = 0;

        vector<double> sum(numElements, 0);

        for (int i = 0; i < numElements; i++) {
            unselected_items.push_back(i);
        }

        shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);

        for (int i = 0; i < numRequiredElements; i++) {
            int element = unselected_items[unselected_items.size()-1];
            unselected_items.pop_back();
            solution.push_back(element);
        }

        best_solution = solution;
        current_cost = dispersion(distanceMatrix, solution);
        best_cost = current_cost;

        for (auto w: solution) {
            for (auto w2: solution) {
                sum[w] += distanceMatrix[w][w2];
            }
        }

        int eval = 0;
        bool better_solution = true;

        while (eval < MAX_EVAL && better_solution) {

            better_solution = false;

            auto u = solution.begin();
            while (u != solution.end() && !better_solution && eval < MAX_EVAL) {
                auto v = unselected_items.begin();
                while (v != unselected_items.end() && !better_solution && eval < MAX_EVAL) {

                    eval++;

                    vector<double> delta(numElements, 0);
                    double delta_max_w = -numeric_limits<double>::max();
                    double delta_min_w = numeric_limits<double>::max();

                    for (auto w: solution) {
                        if (w != *u) {
                            delta[w] = sum[w] - distanceMatrix[w][*u] + distanceMatrix[w][*v];
                            delta[*v] += distanceMatrix[w][*v];

                            if (delta[w] > delta_max_w) {
                                delta_max_w = delta[w];
                            }

                            if (delta[w] < delta_min_w) {
                                delta_min_w = delta[w];
                            }
                        }
                    }

                    double delta_max = max(delta[*v], delta_max_w);
                    double delta_min = min(delta[*v], delta_min_w);
                    double new_cost = delta_max - delta_min;

//                    //print iteration results
//                    cout << "u" << *u << "v" << *v << ". Cost " << new_cost << ". Solution: ";
//                    for (auto it: solution) {
//                        cout << it << ",";
//                    }
//                    cout << endl;

                    if (new_cost < current_cost) {
                        best_cost = new_cost;
                        current_cost = new_cost;
                        sum = delta;

                        int u_value = *u;
                        int v_value = *v;
                        *u = v_value;
                        *v = u_value;

                        better_solution = true;
                        best_solution = solution;
                    }
                    v++;
                }
                u++;
            }

            shuffle(solution.begin(), solution.end(), rng_gen);
            shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);
        }

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += best_cost;

        //print iteration results
        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost funcion aux " << dispersion(distanceMatrix, best_solution) << ". Cost " << best_cost << ". Solution: ";
        for (auto it: best_solution) {
            cout << it << ",";
        }
        cout << endl;
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}