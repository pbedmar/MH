//
// Created by Pedro Bedmar on 2/5/22.
//

#include <iostream>
#include <fstream>
#include "functions.h"
#include "DataLoader.h"
#include "TrajectoryAlgorithms.h"
using namespace std;

int main(int argc, char *argv[]) {
    int seed = stoi(argv[1]);
    string algorithm = argv[2];
    string datafile = argv[3];

    DataLoader data_ = DataLoader(datafile);

    if (algorithm == "ES") {
        TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        trajectoryAlgorithms_.run(1, algorithm);
    } else if (algorithm == "BMB") {
        TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        trajectoryAlgorithms_.run(1, algorithm);
    } else if (algorithm == "ILS") {
        TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        trajectoryAlgorithms_.run(1, algorithm);
    } else if (algorithm == "ILS-ES") {
        TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
        trajectoryAlgorithms_.run(1, algorithm);
    } else {
        cout << "Error selecting the algorithm. Try again with a valid code." << endl;
    }

//    DataLoader data_ = DataLoader("data/GKD-b_46_n150_m45.txt");
//
//    TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), 1);
//    trajectoryAlgorithms_.run(1, "ES");
//
//
//    list<string> paths;
//    ifstream parser("data/data_index.txt");
//    string path;
//
//    while(getline(parser, path)) {
//        paths.push_back(path);
//    }
//
//    // TRAJECTORY BASED ALGORITHMS
//    cout << "------ TRAJECTORY BASED ALGORITHMS ------" << endl;
//    vector<string> models;
//    models.emplace_back("ES");
//    models.emplace_back("BMB");
//    models.emplace_back("ILS");
//    models.emplace_back("ILS-ES");
//
//    for (auto model: models) {
//        list<double> trajectoryCosts;
//        list<double> trajectoryTimes;
//        list<double> trajectoryLowest;
//        list<double> trajectoryHighest;
//        cout << endl << endl << "--- " << model << " ---" << endl;
//
//        for (const auto entry: paths) {
//            DataLoader data = DataLoader(entry);
//
//            cout << endl << "Dataset: " << entry << endl;
//            TrajectoryAlgorithms trajectoryAlgorithms(data.getDistanceMatrix(), data.getNumElements(),
//                                              data.getNumRequiredElements(), 1);
//            trajectoryAlgorithms.run(1,model);
//
//            trajectoryCosts.push_back(trajectoryAlgorithms.getAvgCost());
//            trajectoryTimes.push_back(trajectoryAlgorithms.getAvgTime());
//            trajectoryLowest.push_back(trajectoryAlgorithms.getLowestCost());
//            trajectoryHighest.push_back(trajectoryAlgorithms.getHighestCost());
//        }
//
//        printResults(trajectoryCosts, trajectoryTimes, trajectoryLowest, trajectoryHighest);
//    }
//
//    cout << endl << endl << endl;

}
