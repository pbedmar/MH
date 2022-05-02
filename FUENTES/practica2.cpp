//
// Created by Pedro Bedmar on 2/5/22.
//

#include <iostream>
#include "functions.h"
#include "DataLoader.h"
#include "GeneticAlgorithm.h"
using namespace std;

int main(int argc, char *argv[]) {
    DataLoader data_ = DataLoader("data/GKD-b_43_n150_m15.txt");

    GeneticAlgorithm geneticAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), 1);
    geneticAlgorithm_.run(1);

    return 0;
}
