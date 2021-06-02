#include <iostream>
#include <EvolutionalStrategy.h>
#include <cassert>
#include "BicyclePlan.h"

using namespace std;

int main(int argc, char **argv) {
    assert(argc == 2);
    std::string filepath(argv[1]);
    bool initializeResult = BicyclePlan::initialize(filepath);
    if (!initializeResult) {
        cerr << "Bicycle plan initialize failed" << endl;
        exit(1);
    } else {
        cout << "Bicycle plan initialize success" << endl;
    }

    int parentPopulationSize = 64, selectParentSize = 4, offspringPopulationSize = 64;
    Person bestPerson = evolutionStrategy(parentPopulationSize, selectParentSize, offspringPopulationSize);
    cout << "Self adaption value: " << bestPerson->m_selfAdaption << endl;
    cout << "Calculate objective: " << bestPerson->m_precalculateObjective << endl;
    cout << "Best encoding: " << *bestPerson << endl;
}