//
// Created by Pedro Bedmar on 19/6/22.
//

extern "C" {
    #include "cec17.h"
}
#include <iostream>
#include <vector>
#include <random>

using namespace std;

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

Individual computeMassCenter(vector<Individual> &population, int &eval) {
    sort(population.begin(), population.end());

    vector<double> mass_center(population[0].individual.size(), 0);

    vector<double> inverted_fitness;
    double sum_inverted_fitness = 0;
    for (Individual individual: population) {
        inverted_fitness.push_back(1/individual.fitness);
        sum_inverted_fitness += 1/individual.fitness;
    }

    for (int i=0; i<population.size(); i++) {
        for (int j=0; j<population[i].individual.size(); j++) {
            mass_center[j] += inverted_fitness[i]*population[i].individual[j];
        }
    }

    for (int i=0; i<mass_center.size(); i++) {
        mass_center[i] = mass_center[i]/sum_inverted_fitness;
    }

    eval++;
    return Individual(mass_center);
}

// in the publication, num_candidates=30 and dim=10
void bigbang_bigcrunch(int num_candidates, int dim, mt19937 &gen, int function_id) {
    cec17_init("original", function_id, dim);

    int upper_limit = 100;
    int lower_limit = -100;
    uniform_real_distribution<> dist_u(lower_limit, upper_limit);
    normal_distribution<> dist_n(0,1);

    int iter = 1;
    int eval = 0;
    const int MAX_EVAL = 10000*dim;

    // during the big bang phase, percentage of regenerated individuals
    double new_individual_probability = 0.3;

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
        Individual mass_center = computeMassCenter(population, eval);

        // replace the worst individuals of the population
        //for (int i=num_candidates-1; i>num_candidates*(1-new_individual_probability); i--) {
        normal_distribution<> dist_n2(0,upper_limit/log2(iter+1));
        population[0] = mass_center;
        for (int i=1; i<num_candidates; i++) {
            for (int j=0; j<dim; j++) {
//                double distance = upper_limit*dist_n(gen)/iter;
//                double new_gen = mass_center.individual[j] + distance;
                double new_gen = mass_center.individual[j] + dist_n2(gen);

                if (new_gen > upper_limit) {
                    new_gen = upper_limit;
//                    cout << "enter" << endl;
                }
                if (new_gen < lower_limit) {
                    new_gen = lower_limit;
//                    cout << "enter2" << endl;
                }

                population[i].individual[j] = new_gen;
            }
            population[i].fitness = cec17_fitness(&population[i].individual[0]);
            eval++;
        }

        iter++;
    }
}

int main() {
    int num_candidates = 50;
    int seed = 0;

    vector<int> dimensions;
    dimensions.push_back(10);
    dimensions.push_back(30);
    dimensions.push_back(50);
    dimensions.push_back(100);

    std::mt19937 gen(seed);

    for (int dim: dimensions) {
        for (int i = 1; i <= 30; i++) {
            cout << "Execute dimension " << dim << " function " << i << endl;
            bigbang_bigcrunch(num_candidates, dim, gen, i);
        }
    }
}