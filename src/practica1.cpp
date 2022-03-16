#include <iostream>
#include "DataLoader.h"

using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;

    DataLoader experiment = DataLoader("data/GKD-b_1_n25_m2.txt");

    cout << experiment.getDistanceMatrix()[0][5] << endl;
    cout << experiment.getDistanceMatrix()[5][0] << endl;

    return 0;
}
