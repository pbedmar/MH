//
// Created by Pedro Bedmar on 30/5/22.
//

#include <iostream>
#include <math.h>
#include "functions.h"
#include "TrajectoryAlgorithms.h"

TrajectoryAlgorithms::TrajectoryAlgorithms(vector<vector<double> > distanceMatrix_, int numElements_, int numRequiredElements_,
                                   int seed_) {
    distanceMatrix = distanceMatrix_;
    numElements = numElements_;
    numRequiredElements = numRequiredElements_;
    seed = seed_;
    rng_gen.seed(seed);

    max_neighbours = 5*numElements;
    max_successes = 0.1*max_neighbours;

}

double TrajectoryAlgorithms::getAvgCost() {
    return avg_cost;
}

double TrajectoryAlgorithms::getAvgTime() {
    return avg_time;
}

double TrajectoryAlgorithms::getLowestCost(){
    return lowest_cost;
}

double TrajectoryAlgorithms::getHighestCost(){
    return highest_cost;
}

void TrajectoryAlgorithms::run(int n_times, string type) {

    avg_time = 0;
    avg_cost = 0;

    for (int exec = 0; exec<n_times; exec++) {
        clock_t start_time = clock();

        vector<int> solution;
        if (type == "ES") {
            solution = simulatedAnnealingExecution();
        } else if (type == "BMB") {
            solution = BMB();
        } else if (type == "ILS") {
            solution = ILS();
        } else {
            solution = ILS_ES();
        }

        double solutionDispersion = dispersion(distanceMatrix, solution);

        double elapsed = (clock()- start_time);
        double elapsed_in_seconds = elapsed / CLOCKS_PER_SEC;

        avg_time += elapsed_in_seconds;
        avg_cost += solutionDispersion;

        if (solutionDispersion < lowest_cost) {
            lowest_cost = solutionDispersion;
        }
        if (solutionDispersion > highest_cost) {
            highest_cost = solutionDispersion;
        }

        //print iteration results
        cout << "Execution number " << exec << ". Time " << elapsed_in_seconds << ". Cost " << solutionDispersion << ". Solution: ";
        for (auto it: solution) {
            cout << it << ",";
        }
        cout << endl;
    }

    avg_time = avg_time/n_times;
    avg_cost = avg_cost/n_times;
}

void TrajectoryAlgorithms::computeRandomSolution(vector<int>& unselected_items, vector<int>& solution) {
    for (int i = 0; i < numElements; i++) {
        unselected_items.push_back(i);
    }

    shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);

    for (int i = 0; i < numRequiredElements; i++) {
        int element = unselected_items[unselected_items.size()-1];
        unselected_items.pop_back();
        solution.push_back(element);
    }
}

vector<int> TrajectoryAlgorithms::simulatedAnnealingExecution() {
    vector<int> solution;
    vector<int> unselected_items;
    computeRandomSolution(unselected_items, solution);

    double current_cost = dispersion(distanceMatrix, solution);

    simulatedAnnealing(unselected_items, solution, current_cost, MAX_EVAL);

    return solution;
}

void TrajectoryAlgorithms::simulatedAnnealing(vector<int>& unselected_items, vector<int>& solution, double& current_cost, int MAX_EVAL) {
    uniform_int_distribution<mt19937::result_type> distS(0,numRequiredElements-1);
    uniform_int_distribution<mt19937::result_type> distU(0,numElements-numRequiredElements-1);
    uniform_real_distribution<float> dist01(0,1);
    int M = MAX_EVAL/max_neighbours;

    vector<int> best_solution = solution;
    vector<int> best_unselected_items = unselected_items;
    double best_cost = current_cost;

    vector<double> sum(numElements, 0);
    for (auto w: solution) {
        for (auto w2: solution) {
            sum[w] += distanceMatrix[w][w2];
        }
    }

    int eval = 0;

    double t_0 = (MU*current_cost) / (-log(PHI));
    double t_f = 0.001;
    while (t_0 <= t_f && current_cost!=0) {
        t_f = t_f/10;
    }

    double beta = (t_0 - t_f) / (M*t_0*t_f);
    double t = t_0;

    int count_neighbours = 1;
    int count_successes = 1;
    while (eval < MAX_EVAL && count_successes > 0) {
        count_neighbours = 0;
        count_successes = 0;

        while (count_neighbours < max_neighbours && count_successes < max_successes) {
            // compute neighbour
            int indexS = distS(rng_gen);
            int indexU = distU(rng_gen);
            int swapS = solution[indexS];
            int swapU = unselected_items[indexU];

            vector<double> delta(numElements, 0);
            double delta_max_w = -numeric_limits<double>::max();
            double delta_min_w = numeric_limits<double>::max();

            for (auto w: solution) {
                if (w != swapS) {
                    delta[w] = sum[w] - distanceMatrix[w][swapS] + distanceMatrix[w][swapU];
                    delta[swapU] += distanceMatrix[w][swapU];

                    if (delta[w] > delta_max_w) {
                        delta_max_w = delta[w];
                    }

                    if (delta[w] < delta_min_w) {
                        delta_min_w = delta[w];
                    }
                }
            }

            double delta_max = max(delta[swapU], delta_max_w);
            double delta_min = min(delta[swapU], delta_min_w);
            double neighbour_cost = delta_max - delta_min;

            double delta_f = fabs(neighbour_cost - current_cost);
            count_neighbours++;
            eval++;

            double prob01 = dist01(rng_gen);
            double exp_formula = exp(-delta_f/t);
            if (neighbour_cost < current_cost || prob01 <= exp_formula) {
                solution[indexS] = swapU;
                unselected_items[indexU] = swapS;
                sum = delta;
                current_cost = neighbour_cost;
                count_successes++;

                if (current_cost < best_cost) {
                    best_solution = solution;
                    best_unselected_items = unselected_items;
                    best_cost = current_cost;
                }
            }
        }

        t = t / (1+beta*t);
    }
    solution = best_solution;
    unselected_items = best_unselected_items;
    current_cost = best_cost;
}

void TrajectoryAlgorithms::localSearch(vector<int>& unselected_items, vector<int>& solution, double& solutionDispersion, int MAX_EVAL) {
    vector<int> best_solution = solution;
    vector<int> best_unselected_items = unselected_items;
    double current_cost = solutionDispersion;
    double best_cost = current_cost;

    bool better_solution = true;

    vector<double> sum(numElements, 0);
    for (auto w: solution) {
        for (auto w2: solution) {
            sum[w] += distanceMatrix[w][w2];
        }
    }

    int eval = 0;

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
                    best_unselected_items = unselected_items;
                }
                v++;
            }
            u++;
        }

        shuffle(solution.begin(), solution.end(), rng_gen);
        shuffle(unselected_items.begin(), unselected_items.end(), rng_gen);
    }

    solution = best_solution;
    solutionDispersion = best_cost;
    unselected_items = best_unselected_items;
}

vector<int> TrajectoryAlgorithms::BMB() {
    int T = 10;
    int maxEvalsLS = MAX_EVAL/T;

    vector<int> best_solution;
    double best_cost = numeric_limits<double>::max();

    for (int i=0; i<T; i++) {
        vector<int> solution;
        vector<int> unselected_items;
        computeRandomSolution(unselected_items, solution);

        double current_cost = dispersion(distanceMatrix, solution);
        localSearch(unselected_items, solution, current_cost, maxEvalsLS);

        if (current_cost < best_cost) {
            best_solution = solution;
            best_cost = current_cost;
        }
    }

    return best_solution;
}

vector<int> TrajectoryAlgorithms::ILS() {
    int T = 10;
    int maxEvalsLS = MAX_EVAL/T;
    int num_mutations = 0.1*numRequiredElements;
    if (num_mutations < 1) {
        num_mutations = 1;
    }

    vector<int> best_solution;
    vector<int> best_unselected_items;
    computeRandomSolution(best_unselected_items, best_solution);

    double best_cost = dispersion(distanceMatrix, best_solution);
    localSearch(best_unselected_items, best_solution, best_cost, maxEvalsLS);
    T--;

    for (int i=0; i<T; i++) {
        vector<int> new_solution = best_solution;
        vector<int> new_unselected_items = best_unselected_items;

        shuffle(new_solution.begin(), new_solution.end(), rng_gen);
        shuffle(new_unselected_items.begin(), new_unselected_items.end(), rng_gen);

        for (int j=0; j<num_mutations; j++) {
            int swap = new_solution[j];
            new_solution[j] = new_unselected_items[j];
            new_unselected_items[j] = swap;
        }

        double new_cost = dispersion(distanceMatrix, new_solution);
        localSearch(new_unselected_items, new_solution, new_cost, maxEvalsLS);

        if (new_cost < best_cost) {
            best_solution = new_solution;
            best_unselected_items = new_unselected_items;
            best_cost = new_cost;
        }
    }

    return best_solution;
}

vector<int> TrajectoryAlgorithms::ILS_ES() {
    int T = 10;
    int maxEvalsES = MAX_EVAL/T;
    int num_mutations = 0.1*numRequiredElements;
    if (num_mutations < 1) {
        num_mutations = 1;
    }

    vector<int> best_solution;
    vector<int> best_unselected_items;
    computeRandomSolution(best_unselected_items, best_solution);

    double best_cost = dispersion(distanceMatrix, best_solution);
    simulatedAnnealing(best_unselected_items, best_solution, best_cost, maxEvalsES);
    T--;

    for (int i=0; i<T; i++) {
        vector<int> new_solution = best_solution;
        vector<int> new_unselected_items = best_unselected_items;

        shuffle(new_solution.begin(), new_solution.end(), rng_gen);
        shuffle(new_unselected_items.begin(), new_unselected_items.end(), rng_gen);

        for (int j=0; j<num_mutations; j++) {
            int swap = new_solution[j];
            new_solution[j] = new_unselected_items[j];
            new_unselected_items[j] = swap;
        }

        double new_cost = dispersion(distanceMatrix, new_solution);
        simulatedAnnealing(new_unselected_items, new_solution, new_cost, maxEvalsES);

        if (new_cost < best_cost) {
            best_solution = new_solution;
            best_unselected_items = new_unselected_items;
            best_cost = new_cost;
        }
    }

    return best_solution;
}