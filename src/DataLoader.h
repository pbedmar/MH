//
// Created by Pedro Bedmar on 16/3/22.
//

#ifndef PRACTICAS_DATALOADER_H
#define PRACTICAS_DATALOADER_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class DataLoader {
    int numElements;
    int numRequiredElements;
    vector<vector<double> > distanceMatrix;

public:
    DataLoader(string path);
    int getNumElements();
    int getNumRequiredElements();
    vector<vector<double> > getDistanceMatrix();
};


#endif //PRACTICAS_DATALOADER_H
