//
// Created by Pedro Bedmar on 19/6/22.
//

extern "C" {
    #include "cec17.h"
}
#include <iostream>
#include <filesystem>
#include <vector>
#include <random>

using namespace std;
namespace fs = std::filesystem;

void clip(vector<double> &sol, int lower, int upper) {
    for (auto &val : sol) {
        if (val < lower) {
            val = lower;
        }
        else if (val > upper) {
            val = upper;
        }
    }
}

void increm_bias(vector<double> &bias, vector<double> dif) {
    for (unsigned i = 0; i < bias.size(); i++) {
        bias[i] = 0.2*bias[i]+0.4*(dif[i]+bias[i]);
    }
}

void decrement_bias(vector<double> &bias, vector<double> dif) {
    for (unsigned i = 0; i < bias.size(); i++) {
        bias[i] = bias[i]-0.4*(dif[i]+bias[i]);
    }
}

/**
 * Aplica el Solis Wets (Búsqueda Local)
 *
 * @param  sol solucion a mejorar.
 * @param fitness fitness de la solución.
 */
template <class Random>
void soliswets(vector<double> &sol, double &fitness, double delta, int maxevals, int &evals_end, int lower, int upper, Random &random) {
    const size_t dim = sol.size();
    vector<double> bias (dim), dif (dim), newsol (dim);
    double newfit;
    size_t i;

    int evals = 0;
    int num_success = 0;
    int num_failed = 0;

    while (evals < maxevals) {
        std::uniform_real_distribution<double> distribution(0.0, delta);

        for (i = 0; i < dim; i++) {
            dif[i] = distribution(random);
            newsol[i] = sol[i] + dif[i] + bias[i];
        }

        clip(newsol, lower, upper);
        newfit = cec17_fitness(&newsol[0]);
        evals += 1;

        if (newfit < fitness) {
            sol = newsol;
            fitness = newfit;
            increm_bias(bias, dif);
            num_success += 1;
            num_failed = 0;
        }
        else if (evals < maxevals) {

            for (i = 0; i < dim; i++) {
                newsol[i] = sol[i] - dif[i] - bias[i];
            }

            clip(newsol, lower, upper);
            newfit = cec17_fitness(&newsol[0]);
            evals += 1;

            if (newfit < fitness) {
                sol = newsol;
                fitness = newfit;
                decrement_bias(bias, dif);
                num_success += 1;
                num_failed = 0;
            }
            else {
                for (i = 0; i < dim; i++) {
                    bias[i] /= 2;
                }

                num_success = 0;
                num_failed += 1;
            }
        }

        if (num_success >= 5) {
            num_success = 0;
            delta *= 2;
        }
        else if (num_failed >= 3) {
            num_failed = 0;
            delta /= 2;
        }
    }

    evals_end += evals;
}

// chromosome data structure, containing its genes and its fitness.
struct Individual {
    vector<double> individual;
    double fitness;

    Individual(vector<double> c) {
        individual = c;
        fitness = cec17_fitness(&individual[0]);
    }

    bool operator < (const Individual& c) const
    {
        return (fitness < c.fitness);
    }
};


// in the publication, num_candidates=30 and dim=10
Individual bigbang_bigcrunch_bestindiv(int num_candidates, int dim, mt19937 &gen, int function_id, char* name) {
    cec17_init(name, function_id, dim);

    int upper_limit = 100;
    int lower_limit = -100;
    uniform_real_distribution<> dist_u(lower_limit, upper_limit);
    normal_distribution<> dist_n(0,1);

    int iter = 1;
    int eval = 0;
    const int MAX_EVAL = 10000*dim;

    // compute the initial population - initial big bang
    vector<Individual> population;
    for (int i=0; i<num_candidates; i++) {
        // generate random individual and add it to the population
        vector<double> individual_;
        for (int j=0; j<dim; j++) {
            individual_.push_back(dist_u(gen));
        }
        Individual individual(individual_);
        population.push_back(individual);
        eval++;
    }

    while (eval < MAX_EVAL) {
        // select the best individual - big crunch phase
        sort(population.begin(), population.end());
        Individual mass_center = population[0];

        if (iter % 10 == 0) {
            soliswets(mass_center.individual, mass_center.fitness, 0.2, 70*dim, eval, -100, 100, gen);
        }

        // replace the worst individuals of the population
        for (int i=0; i<num_candidates; i++) {
            for (int j=0; j<dim; j++) {
                double distance = upper_limit*dist_n(gen)/iter;
                double new_gen = mass_center.individual[j] + distance;

                if (new_gen > upper_limit) {
                    new_gen = upper_limit;
                }
                if (new_gen < lower_limit) {
                    new_gen = lower_limit;
                }

                population[i].individual[j] = new_gen;
            }
            population[i].fitness = cec17_fitness(&population[i].individual[0]);
            eval++;
        }

        iter++;
    }

    sort(population.begin(), population.end());
    return population[0];
}


int main() {
    const int CHAR_MAX_SIZE = 100;
    const int NUM_EXECS = 10;

    vector<int> dimensions;
    dimensions.push_back(10);
    dimensions.push_back(30);
    dimensions.push_back(50);
    dimensions.push_back(100);

    vector<int> num_candidates;
    num_candidates.push_back(30);
    num_candidates.push_back(50);
    num_candidates.push_back(100);
    num_candidates.push_back(150);

    std::mt19937 gen(0);

    string name_ = "ls2";
    char name[CHAR_MAX_SIZE];
    strcpy(name, name_.c_str());

    fs::create_directory("results_" + name_ + "/");

    for (int i=0; i<dimensions.size(); i++) {
        for (int j = 1; j <= 30; j++) {
            cout << "Execute dimension " << dimensions[i] << " function " << j << endl;
            for (int exec = 0; exec < NUM_EXECS; exec++) {
                cout << "Exec num. " << exec << endl;
                Individual best = bigbang_bigcrunch_bestindiv(num_candidates[i], dimensions[i], gen, j, name);
                cout << "Error: " << cec17_error(best.fitness) << endl << endl;
            }
        }
    }
}