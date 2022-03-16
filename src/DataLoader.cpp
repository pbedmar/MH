//
// Created by Pedro Bedmar on 16/3/22.
//

#include "DataLoader.h"
#include <fstream>

DataLoader::DataLoader(std::string path) {
    ifstream file(path);
    if (!file) {
        cout << "Error opening data file. Aborting...";
        exit(-1);
    }

    string data;
    file >> data;
    numElements = stoi(data);
    file >> data;
    numRequiredElements = stoi(data);

    distanceMatrix = vector<vector<double> > (numElements, vector<double>(numElements, 0));

    while(file >> data) {
        int i = stoi(data);
        file >> data;
        int j = stoi(data);
        file >> data;
        cout << i << endl;
        distanceMatrix[i][j] = stod(data);
        distanceMatrix[j][i] = stod(data);
    }
}

int DataLoader::getNumElements() {
    return numElements;
}

int DataLoader::getNumRequiredElements() {
    return numRequiredElements;
}

vector<vector<double> > DataLoader::getDistanceMatrix() {
    return distanceMatrix;
}