#include <iostream>
#include <cassert>
#include <EvolutionalStrategy.h>
#include <Config.h>
#include "BicyclePlan.h"
#include "timer.h"
#include <fstream>


using namespace std;


#ifdef __linux__    // Linux only
#include <sched.h>  // sched_setaffinity
void pinCPU (int cpu_number)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);

    CPU_SET(cpu_number, &mask);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
    {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
}
#elif _WIN32
#include <Windows.h>

void pinCPU (int cpu_number)
{
    HANDLE process = GetCurrentProcess();
    DWORD_PTR processAffinityMask = 1UL << cpu_number;

    BOOL success = SetProcessAffinityMask(process, processAffinityMask);
}
#endif

static void outputStatisticAndEncodingFile(const Person &bestPerson, const Timer &timer, const EvolutionStrategy &es) {
    auto config = Config::getInstance();

    // Output statistic file
    es.statistic(config->m_statisticLocation);

    // Output encoding file
    fstream fs;
    fs.open(config->m_encodingLocation, std::ios::out);
    fs << *bestPerson;
    fs.close();

}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "BicyclePlan.exe <config path> <pin at cpu id>";
        exit(1);
    }
    std::string configFilepath(argv[1]);
    pinCPU(strtol(argv[2], nullptr, 10));

    // read config

    bool configResult = Config::readConfig(configFilepath);
    if (!configResult) {
        cerr << "Config initialize failed" << endl;
        exit(1);
    } else {
        cout << "Config initialize success" << endl;
    }

    auto config = Config::getInstance();
    cout << "Seed is " << config->m_seed << endl;

    // initialize bicycle plan

    bool initializeResult = BicyclePlan::readConfig(config->m_bicyclePowerPlanLocation);
    if (!initializeResult) {
        cerr << "Bicycle plan initialize failed" << endl;
        exit(1);
    } else {
        cout << "Bicycle plan initialize success, the plan name is " << BicyclePlan::getInstance()->m_planName << endl;
    }

    // parameter settings
    const int parentPopulationSize = config->m_parentPopulationSize;
    const int selectParentSize = config->m_selectPopulationSize;
    const int offspringPopulationSize = config->m_offspringPopulationSize;

    Util::initializeUtil();

    // compute

    cout << "[Start] Evolution Strategy..." << endl;

    Timer timer;
    timer.start();

    EvolutionStrategy evolutionStrategy;
    evolutionStrategy.initialize();
    const Person bestPerson = evolutionStrategy.evaluate(parentPopulationSize, selectParentSize, offspringPopulationSize);

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

    outputStatisticAndEncodingFile(bestPerson, timer, evolutionStrategy);

    return 0;
}
