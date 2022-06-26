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

Individual bigbang_bigcrunch_bestindiv(int num_candidates, int dim, mt19937 &gen, int function_id, char* name, const int MAX_EVAL) {
    int upper_limit = 100;
    int lower_limit = -100;
    uniform_real_distribution<> dist_u(lower_limit, upper_limit);
    normal_distribution<> dist_n(0,1);

    int iter = 1;
    int eval = 0;

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

void BMB(int num_candidates, int dim, mt19937 &gen, int function_id, char* name) {
    cec17_init(name, function_id, dim);

    const int MAX_EVAL_TOTAL = 10000*dim;
    const int T=10;

    for (int i=0; i<T; i++) {
        bigbang_bigcrunch_bestindiv(num_candidates, dim, gen, function_id, name, MAX_EVAL_TOTAL/T);
    }
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

    string name_ = "bmb";
    char name[CHAR_MAX_SIZE];
    strcpy(name, name_.c_str());

    fs::create_directory("results_" + name_ + "/");

    for (int i=0; i<dimensions.size(); i++) {
        for (int j = 1; j <= 30; j++) {
            cout << "Execute dimension " << dimensions[i] << " function " << j << endl;
            for (int exec = 0; exec < NUM_EXECS; exec++) {
                cout << "Exec num. " << exec << endl;
                BMB(num_candidates[i], dimensions[i], gen, j, name);
            }
        }
    }
}