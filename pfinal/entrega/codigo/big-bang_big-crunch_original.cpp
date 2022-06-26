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

Individual bigbang_bigcrunch_equation(int num_candidates, int dim, mt19937 &gen, int function_id, char* name) {
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
        Individual mass_center = massCenterEquation(population, eval);

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

void exec_version(int num_candidates, int dim, std::mt19937 gen, int function, string version, char* name) {
    // center of mass is the best individual
    if (version == "bi") {
        Individual best = bigbang_bigcrunch_bestindiv(num_candidates, dim, gen, function, name);
        cout << "Error: " << cec17_error(best.fitness) << endl;
    // center of mass computed using the equation in the publication
    } else if (version == "eq") {
        Individual best = bigbang_bigcrunch_equation(num_candidates, dim, gen, function, name);
        cout << "Error: " << cec17_error(best.fitness) << endl;
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

    vector<string> versions;
    versions.emplace_back("bi");
    versions.emplace_back("eq");


    for (int v=0; v<versions.size(); v++) {
        std::mt19937 gen(v);

        string name_ = "original_" + versions[v];
        char name[CHAR_MAX_SIZE];
        strcpy(name, name_.c_str());

        fs::create_directory("results_" + name_ + "/");
        cout << endl << "Ejecutando " << name_ << endl;

        for (int i=0; i<dimensions.size(); i++) {
            for (int j = 1; j <= 30; j++) {
                cout << "Execute dimension " << dimensions[i] << " function " << j << endl;
                for (int exec = 0; exec < NUM_EXECS; exec++) {
                    cout << "Exec num. " << exec << endl;
                    exec_version(num_candidates[i], dimensions[i], gen, j, versions[v], name);
                    cout << endl;
                }
            }
        }
    }
}