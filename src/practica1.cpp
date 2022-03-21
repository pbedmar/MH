#include <iostream>
#include <fstream>
#include <list>
#include "functions.h"
#include "DataLoader.h"
#include "GreedyAlgorithm.h"
using namespace std;

int main() {
    DataLoader data_ = DataLoader("data/GKD-b_36_n125_m37.txt");

    list<string> paths;
    ifstream parser("data/data_index.txt");
    string path;

    while(getline(parser, path)) {
        paths.push_back(path);
    }

    list<double> costs;
    list<double> times;

    for (const auto entry : paths) {
        DataLoader data = DataLoader(entry);

        GreedyAlgorithm greedyAlgorithm(data.getDistanceMatrix(), data.getNumElements(), data.getNumRequiredElements());
        greedyAlgorithm.run(5);

        costs.push_back(greedyAlgorithm.getAvgCost());
        times.push_back(greedyAlgorithm.getAvgTime());
    }

    printResults(costs, times);

    return 0;
}
