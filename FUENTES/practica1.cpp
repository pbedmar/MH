#include <iostream>
#include <fstream>
#include <list>
#include "functions.h"
#include "DataLoader.h"
#include "GreedyAlgorithm.h"
#include "LocalSearchAlgorithm.h"
using namespace std;

int main(int argc, char *argv[]) {
    int seed = stoi(argv[1]);
    string algorithm = argv[2];
    string datafile = argv[3];

    DataLoader data_ = DataLoader(datafile);

    if (algorithm == "g") {
        GreedyAlgorithm greedyAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        greedyAlgorithm_.run(1);
    } else if (algorithm == "b") {
        LocalSearchAlgorithm localSearchAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        localSearchAlgorithm_.run(1);
    }

    //DataLoader data_ = DataLoader("data/GKD-b_1_n25_m2.txt");
//    DataLoader data_ = DataLoader("data/GKD-b_43_n150_m15.txt");
//
//    LocalSearchAlgorithm localSearchAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements());
//
//    localSearchAlgorithm_.run(1);
//
//    return 0;

//    list<string> paths;
//    ifstream parser("data/data_index.txt");
//    string path;
//
//    while(getline(parser, path)) {
//        paths.push_back(path);
//    }
//
//    // GREEDY
//    cout << "------ GREEDY ALGORITHM ------" << endl;
//    list<double> greedy_costs;
//    list<double> greedy_times;
//    list<double> greedy_lowest;
//    list<double> greedy_highest;
//
//    for (const auto entry : paths) {
//        DataLoader data = DataLoader(entry);
//
//        cout << endl << "Dataset: " << entry << endl;
//        GreedyAlgorithm greedyAlgorithm(data.getDistanceMatrix(), data.getNumElements(), data.getNumRequiredElements());
//        greedyAlgorithm.run(5);
//
//        greedy_costs.push_back(greedyAlgorithm.getAvgCost());
//        greedy_times.push_back(greedyAlgorithm.getAvgTime());
//        greedy_lowest.push_back(greedyAlgorithm.getLowestCost());
//        greedy_highest.push_back(greedyAlgorithm.getHighestCost());
//    }
//
//    printResults(greedy_costs, greedy_times, greedy_lowest, greedy_highest);
//
//
//    //LOCAL SEARCH
//    cout << "------ LOCAL SEARCH ALGORITHM ------" << endl;
//    list<double> ls_costs;
//    list<double> ls_times;
//    list<double> ls_lowest;
//    list<double> ls_highest;
//
//    for (const auto entry : paths) {
//        DataLoader data = DataLoader(entry);
//
//        cout << endl << "Dataset: " << entry << endl;
//        LocalSearchAlgorithm localSearchAlgorithm(data.getDistanceMatrix(), data.getNumElements(), data.getNumRequiredElements());
//        localSearchAlgorithm.run(5);
//
//        ls_costs.push_back(localSearchAlgorithm.getAvgCost());
//        ls_times.push_back(localSearchAlgorithm.getAvgTime());
//        ls_lowest.push_back(localSearchAlgorithm.getLowestCost());
//        ls_highest.push_back(localSearchAlgorithm.getHighestCost());
//    }
//
//    printResults(ls_costs, ls_times, ls_lowest, ls_highest);
//    cout << endl << endl;

    return 0;
}
