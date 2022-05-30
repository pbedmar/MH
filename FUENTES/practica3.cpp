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
//    int seed = stoi(argv[1]);
//    string algorithm = argv[2];
//    string datafile = argv[3];
//
//    DataLoader data_ = DataLoader(datafile);
//
//    if (algorithm == "g") {
//        GeneticAlgorithm geneticAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
//        geneticAlgorithm_.run(1, "gen", "uni");
//    } else if (algorithm == "m") {
//        MemeticAlgorithm memeticAlgorithm_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), seed);
//        memeticAlgorithm_.run(1, "AM0.1");
//    }

    DataLoader data_ = DataLoader("data/GKD-b_43_n150_m15.txt");

    TrajectoryAlgorithms trajectoryAlgorithms_(data_.getDistanceMatrix(), data_.getNumElements(), data_.getNumRequiredElements(), 1);
    trajectoryAlgorithms_.run(1, "gen");


//    list<string> paths;
//    ifstream parser("data/data_index.txt");
//    string path;
//
//    while(getline(parser, path)) {
//        paths.push_back(path);
//    }

//    // GENETIC
//    cout << "------ GENETIC ALGORITHM ------" << endl;
//    vector<string> models;
//    models.emplace_back("gen");
//    models.emplace_back("est");
//    vector<string> crossoverOperators;
//    crossoverOperators.emplace_back("uni");
//    crossoverOperators.emplace_back("pos");
//
//    for (auto model: models) {
//        for(auto crossoverOperator: crossoverOperators) {
//
//            list<double> geneticCosts;
//            list<double> geneticTimes;
//            list<double> geneticLowest;
//            list<double> geneticHighest;
//            cout << endl << endl << "-- " << model << " -- " << crossoverOperator << " --" << endl;
//
//            for (const auto entry: paths) {
//                DataLoader data = DataLoader(entry);
//
//                cout << endl << "Dataset: " << entry << endl;
//                GeneticAlgorithm geneticAlgorithm(data.getDistanceMatrix(), data.getNumElements(),
//                                                  data.getNumRequiredElements());
//                geneticAlgorithm.run(1,model, crossoverOperator);
//
//                geneticCosts.push_back(geneticAlgorithm.getAvgCost());
//                geneticTimes.push_back(geneticAlgorithm.getAvgTime());
//                geneticLowest.push_back(geneticAlgorithm.getLowestCost());
//                geneticHighest.push_back(geneticAlgorithm.getHighestCost());
//            }
//
//            printResults(geneticCosts, geneticTimes, geneticLowest, geneticHighest);
//        }
//    }
//
//    cout << endl << endl << endl;
//
//
//    // MEMEMETIC
//    cout << "------ MEMETIC ALGORITHM ------" << endl;
//    vector<string> memeticVersions;
//    memeticVersions.emplace_back("AM1.0");
//    memeticVersions.emplace_back("AM0.1");
//    memeticVersions.emplace_back("AM0.1mej");
//
//    for (auto version: memeticVersions) {
//        list<double> memeticCosts;
//        list<double> memeticTimes;
//        list<double> memeticLowest;
//        list<double> memeticHighest;
//        cout << endl << endl << "-- " << version << " --" << endl;
//
//        for (const auto entry: paths) {
//            DataLoader data = DataLoader(entry);
//
//            cout << endl << "Dataset: " << entry << endl;
//            MemeticAlgorithm memeticAlgorithm(data.getDistanceMatrix(), data.getNumElements(),
//                                              data.getNumRequiredElements());
//            memeticAlgorithm.run(1,version);
//
//            memeticCosts.push_back(memeticAlgorithm.getAvgCost());
//            memeticTimes.push_back(memeticAlgorithm.getAvgTime());
//            memeticLowest.push_back(memeticAlgorithm.getLowestCost());
//            memeticHighest.push_back(memeticAlgorithm.getHighestCost());
//        }
//
//        printResults(memeticCosts, memeticTimes, memeticLowest, memeticHighest);
//    }
//
//    cout << endl << endl;
}
