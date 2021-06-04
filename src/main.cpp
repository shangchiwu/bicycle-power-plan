#include <iostream>
#include <cassert>
#include <EvolutionalStrategy.h>
#include "BicyclePlan.h"
#include "timer.h"

using namespace std;

int main(int argc, char **argv) {
    assert(argc == 2);
    std::string filepath(argv[1]);

    // initialize

    bool initializeResult = BicyclePlan::readConfig(filepath);
    if (!initializeResult) {
        cerr << "Bicycle plan initialize failed" << endl;
        exit(1);
    } else {
        cout << "Bicycle plan initialize success, the plan name is " << BicyclePlan::getInstance()->m_planName << endl;
    }

    // parameter settings

    constexpr int parentPopulationSize = 64;
    constexpr int selectParentSize = 4;
    constexpr int offspringPopulationSize = 64;
    
    // compute

    cout << "[Start] Evolution Strategy..." << endl;

    Timer timer;
    timer.start();

    const Person bestPerson = evolutionStrategy(parentPopulationSize, selectParentSize, offspringPopulationSize);

    timer.stop();
    const double executionTime = (double) timer.getDuration() / 1e9f;
    cout << "[End] Execution time: " << executionTime << " seconds" << endl;

    // output result

    cout << "Self adaption value: ";
    for (auto i: bestPerson->m_selfAdaptionList) {
        cout << i << " ";
    }
    cout << endl;
    cout << "Calculate objective: " << bestPerson->m_precalculateObjective << endl;
    cout << "Best encoding: " << *bestPerson << endl;

    return 0;
}
