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

Individual massCenterEquation(vector<Individual> &population, int &eval) {
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

    sort(population.begin(), population.end());

    eval++;
    return Individual(mass_center);
}


// in the publication, num_candidates=30 and dim=10
void bigbang_bigcrunch_equation(int num_candidates, int dim, mt19937 &gen, int function_id, char* name) {
    cec17_init(name, function_id, dim);

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
        normal_distribution<> dist_n2(0,upper_limit/log2(iter+1));

        // select the best individual - big crunch phase
        Individual mass_center = massCenterEquation(population, eval);
        population[num_candidates*(1-new_individual_probability)] = mass_center;

        // replace the worst individuals of the population
        for (int i=num_candidates-1; i>num_candidates*(1-new_individual_probability); i--) {
            for (int j=0; j<dim; j++) {
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

// in the publication, num_candidates=30 and dim=10
void bigbang_bigcrunch_bestindiv(int num_candidates, int dim, mt19937 &gen, int function_id, char* name) {
    cec17_init(name, function_id, dim);

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
        sort(population.begin(), population.end());
        Individual mass_center = population[0];

        // replace the worst individuals of the population
        for (int i=num_candidates-1; i>num_candidates*(1-new_individual_probability); i--) {
            for (int j=0; j<dim; j++) {
                double distance = upper_limit*dist_n(gen)/iter;
                double new_gen = mass_center.individual[j] + distance;

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

void exec_version(int num_candidates, int dim, std::mt19937 gen, int function, string version, char* name) {
    if (version == "best_individual") {
        bigbang_bigcrunch_bestindiv(num_candidates, dim, gen, function, name);
    } else if (version == "equation") {
        bigbang_bigcrunch_equation(num_candidates, dim, gen, function, name);
    }

}

int main() {
    const int CHAR_MAX_SIZE = 100;
    const int NUM_EXECS = 10;
    int num_candidates = 50;

    vector<int> dimensions;
    dimensions.push_back(10);
    dimensions.push_back(30);
    dimensions.push_back(50);
    dimensions.push_back(100);

    vector<string> versions;
    versions.emplace_back("best_individual");
    versions.emplace_back("equation");


    for (int i=0; i<versions.size(); i++) {
        std::mt19937 gen(i);

        for (int exec = 0; exec < NUM_EXECS; exec++) {
            string name_ = versions[i] + "_exec" + to_string(exec);
            char name[CHAR_MAX_SIZE];
            strcpy(name, name_.c_str());

            fs::create_directory(name_ + "/");
            cout << endl << "Ejecutando " << name_ << endl;

            for (int dim: dimensions) {
                for (int j = 1; j <= 30; j++) {
                    cout << "Execute dimension " << dim << " function " << j << endl;
                    exec_version(num_candidates, dim, gen, j, versions[i], name);
                }
            }
        }
    }
}