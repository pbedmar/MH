#include <iostream>
#include "DataLoader.h"
#include "GreedyAlgorithm.h"
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;

    DataLoader data = DataLoader("data/GKD-b_36_n125_m37.txt");

    cout << "Number of elements: " << data.getNumElements() << endl;
    cout << "Number of solution elements: " << data.getNumRequiredElements() << endl << endl;

    cout << "Testing distances:" << endl;
    cout << data.getDistanceMatrix()[0][5] << endl;
    cout << data.getDistanceMatrix()[5][0] << endl << endl;

    GreedyAlgorithm greedyAlgorithm(data.getDistanceMatrix(), data.getNumElements(), data.getNumRequiredElements());
    greedyAlgorithm.run(10);

    return 0;
}
