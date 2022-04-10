//
// Created by Pedro Bedmar on 16/3/22.
//

#include <unordered_set>
#include <iostream>
#include "GreedyAlgorithm.h"
#include "functions.h"


GreedyAlgorithm::GreedyAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_) {
    seed = seed_;
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    rng_gen.seed(seed);
}

double GreedyAlgorithm::getAvgCost() {
    return avg_cost;
}

double GreedyAlgorithm::getAvgTime() {
 return avg_time;
}

void GreedyAlgorithm::run(int n_times) {

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        unordered_set<int> solution;
        unordered_set<int> unselected_elements;
        vector<double> sum(numElements, 0);

        for(int i = 0; i < numElements; i++) {
            unselected_elements.insert(i);
        }

        uniform_int_distribution<mt19937::result_type> dist(0,numElements-1);

        int first_element = dist(rng_gen); // first element is chosen randomly
        solution.insert(first_element);
        unselected_elements.erase(first_element);

        int second_element;
        do {
            second_element = dist(rng_gen);
        } while (first_element == second_element);
        solution.insert(second_element);
        unselected_elements.erase(second_element);

        for (auto u: unselected_elements) {
            for (auto v: solution) {
                sum[u] += distanceMatrix[u][v];
            }
        }

        for (auto v: solution) {
            for (auto v2: solution) {
                sum[v] += distanceMatrix[v][v2];
            }
        }

        double min_g = 0;
        double element_min_g = -1;

        while (solution.size() < numRequiredElements) {

            min_g = numeric_limits<double>::max();
            element_min_g = -1;

            for (auto u: unselected_elements) {
                double del_v_max = -numeric_limits<double>::max();
                double del_v_min = numeric_limits<double>::max();

                for (auto v: solution) {
                    double del_v = sum[v] + distanceMatrix[u][v];

                    if (del_v_max < del_v) {
                        del_v_max = del_v;
                    }
                    if (del_v_min > del_v) {
                        del_v_min = del_v;
                    }
                }

                double delta_u_max = max(sum[u], del_v_max);
                double delta_u_min = min(sum[u], del_v_min);

                double g = delta_u_max - delta_u_min;
                if (min_g > g) {
                    min_g = g;
                    element_min_g = u;
                }
            }

            unselected_elements.erase(element_min_g);
            solution.insert(element_min_g);

            for (int i = 0; i < numElements; i++) {
                sum[i] += distanceMatrix[i][element_min_g];
            }
        }

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += min_g;

        //print iteration results
        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << min_g << ". Solution: ";
        for (auto it = solution.cbegin(); it != solution.cend(); it++) {
            cout << *it << ", ";
        }
        cout << endl;
    }

    avg_time = avg_time/n_times; //TODO: Accumulated or mean time?
    avg_cost = avg_cost/n_times;
}

