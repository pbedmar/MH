//
// Created by Pedro Bedmar on 16/3/22.
//

#include <unordered_set>
#include <iostream>
#include "GreedyAlgorithm.h"


GreedyAlgorithm::GreedyAlgorithm(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_, int seed_) {
    seed = seed_;
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    srand(seed);
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

        int current_element = randInt(0, numElements-1); // first element is chosen randomly
        solution.insert(current_element);
        unselected_elements.erase(current_element);
        do {
            solution.insert(randInt(0, numElements - 1));// TODO: second element too, as we need three to compute dispersion?
        } while (solution.size() < 2);
        unselected_elements.erase(current_element);

        for (auto u = unselected_elements.cbegin(); u != unselected_elements.cend(); u++) {
            for (auto v = solution.cbegin(); v != solution.cend(); v++) {
                sum[*u] += distanceMatrix[*u][*v];
            }
        }

        for (auto v = solution.cbegin(); v != solution.cend(); v++) {
            for (auto v2 = solution.cbegin(); v2 != solution.cend(); v2++) {
                sum[*v] += distanceMatrix[*v][*v2];
            }
        }

        while (solution.size() < numRequiredElements) {

            cout << "Hola" << endl;

            double min_g = 0;
            double element_min_g = -1;

            for (auto u = unselected_elements.cbegin(); u != unselected_elements.cend(); u++) {
                double del_v_max = 0;
                double del_v_min = 0;

                for (auto v = solution.cbegin(); v != solution.cend(); v++) {
                    double del_v = sum[*v] + distanceMatrix[*u][*v];

                    if (del_v_max < del_v) {
                        del_v_max = del_v;
                    }
                    if (del_v_min > del_v) {
                        del_v_min = del_v;
                    }
                }

                double delta_u_max = max(sum[*u], del_v_max);
                double delta_u_min = min(sum[*u], del_v_min);

                double g = delta_u_max - delta_u_min;
                if (min_g > g) {
                    min_g = g;
                    element_min_g = *u;
                }

            }

            unselected_elements.erase(element_min_g);
            solution.insert(element_min_g);

            for (auto u = unselected_elements.cbegin(); u != unselected_elements.cend(); u++) {
                sum[*u] += distanceMatrix[*u][element_min_g];
            }



            for (auto v = solution.cbegin(); v != solution.cend(); v++) {
                sum[*v] += distanceMatrix[*v][element_min_g];
            }
        }

        double time = (clock()- start_time) / CLOCKS_PER_SEC;
        cout << "Execution number " << exec << ". Time " << time << ". Solution: ";
        for (auto it = solution.cbegin(); it != solution.cend(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }

}

int GreedyAlgorithm::randInt(int min, int max) {
    return rand() % (( max + 1 ) - min);
}