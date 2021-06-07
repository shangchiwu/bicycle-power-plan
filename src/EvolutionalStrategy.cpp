#include <random>
#include <algorithm>
#include <timer.h>
#include <fstream>
#include <Config.h>
#include "EvolutionalStrategy.h"
#include "BicyclePlan.h"

using namespace std;

void EvolutionStrategy::initialize() {
    auto config = Config::getInstance();
    m_tauFactor = config->m_tauFactor;
    m_tauPrimeFactor = config->m_tauPrimeFactor;
    m_epsilonFactor = config->m_epsilonFactor;
    m_iteration = config->m_iteration;
}

void EvolutionStrategy::statistic(const std::string &filepath) const {
    fstream fs;
    fs.open(filepath, std::ios::out);
    fs << "Iteration, Evaluation time(ms), Execution time" << endl;
    for(int i = 0; i < m_executionTimeList.size(); ++i) {
        fs << i+1 << ", " << m_evaluationTimeList[i] << ", " << m_executionTimeList[i] << endl;
    }
    fs.close();
}

Person EvolutionStrategy::evaluate(int parentPopulationSize, int selectParentSize, int offspringPopulationSize) {
    // Reference from page 6 of http://www.cmap.polytechnique.fr/~nikolaus.hansen/es-overview-2015.pdf
    // mu is parentPopulationSize
    // rho is selectParentSize
    // lambda is offspringPopulationSize

    const int ITERATION = m_iteration;

    Population parentPopulations;
    initializePopulation(parentPopulations, parentPopulationSize);
//    for(auto person: parentPopulations) {
//        cout << "Encoding: " << *person << ", objective is " << person->getPrecalculateObjective() << endl;
//    }
//    getchar();

    Person bestPerson = getBestPersonFromPopulation(parentPopulations);
    Population offspringPopulations;
    offspringPopulations.reserve(offspringPopulationSize);

    for (int i = 0; i < ITERATION; ++i) {
//        cout << "Iteration: " << i << endl;
        Timer timer;
        timer.start();
        refreshChanceTable(parentPopulations);
        for (int j = 0; j < offspringPopulationSize; ++j) {
            Person newOffspring = generateOffspringFromPopulation(parentPopulations, selectParentSize);
            updateSelfAdaptionMutation(newOffspring);
            mutation(newOffspring);
            offspringPopulations.emplace_back(newOffspring);
        }
        parentPopulations = survivorSelection(parentPopulations, offspringPopulations);
        Person potentialBestPerson = getBestPersonFromPopulation(parentPopulations);
        if (potentialBestPerson->getPrecalculateObjective() < bestPerson->getPrecalculateObjective()) {
            bestPerson = potentialBestPerson;
        }
//        for(auto person: parentPopulations) {
//            cout << "Encoding: " << *person << ", objective is " << person->getPrecalculateObjective() << endl;
//            cout << "self-adaption: ";
//            for(int i = 0; i < person->m_n; ++i) {
//                cout << person->m_selfAdaptionList[i] << " ";
//            }
//            cout << endl;
//            getchar();
//        }
//        getchar();
        offspringPopulations.clear();
        timer.stop();
        const double executionTime = (float) timer.getDuration() / 1e6f;
        m_executionTimeList.emplace_back(executionTime);
        m_evaluationTimeList.emplace_back(bestPerson->getPrecalculateObjective());
    }
    return bestPerson;
}

Person getBestPersonFromPopulation(Population &population) {
    float bestQuality = 1e9;
    Person bestPerson;
    for (auto &person: population) {
        const float quality = person->getPrecalculateObjective();
//        cout << "bestQuality vs quality: " << bestQuality << " " << quality << endl;
        if (quality < bestQuality) {
            bestQuality = quality;
            bestPerson = person;
        }
    }
    return bestPerson;
}

void initializePopulation(Population &parentPopulations, int parentPopulationSize) {
    // TODO Project initial encoding to feasible encoding
    auto bicyclePlan = BicyclePlan::getInstance();
    parentPopulations.reserve(parentPopulationSize);
    for (int i = 0; i < parentPopulationSize; ++i) {
        parentPopulations.emplace_back(make_shared<Encoding>(bicyclePlan->m_track.m_numSegments));
    }
}

void EvolutionStrategy::refreshChanceTable(const Population &parentPopulations) {
    m_chanceTableList.clear();
    float totalObjective = 0.0f;
    float maxObjective = 0.0f;
    for(auto &parent: parentPopulations) {
        float objective = parent->getPrecalculateObjective();
        maxObjective = max(maxObjective, objective);
    }
    for(auto &parent: parentPopulations) {
        float objective = parent->getPrecalculateObjective();
        totalObjective += maxObjective - objective;
    }
    float endRange = 0.0f;
    for(auto &parent: parentPopulations) {
        float objective = parent->getPrecalculateObjective();
        endRange += (maxObjective - objective)/totalObjective;
        m_chanceTableList.emplace_back(endRange);
    }
}

Person EvolutionStrategy::generateOffspringFromPopulation(const Population &parentPopulations, int selectParentSize) {
    // In page 7 "Recombination Operators" of http://www.cmap.polytechnique.fr/~nikolaus.hansen/es-overview-2015.pdf
    // Use intermediate recombination
    Population selectedParentPopulation;
    selectedParentPopulation.reserve(selectParentSize);
    int chooseIdx;
    for (int i = 0; i < selectParentSize; ++i) {
        float choose = Util::randomFloatUniform();
        chooseIdx = lower_bound(m_chanceTableList.begin(), m_chanceTableList.end(), choose) - m_chanceTableList.begin();
        if (chooseIdx >= m_chanceTableList.size()) {
            chooseIdx = m_chanceTableList.size()-1;
        }
        selectedParentPopulation.emplace_back(parentPopulations[chooseIdx]);
    }
    const int roadAmount = parentPopulations[0]->m_n;
    Person offspring = make_shared<Encoding>(roadAmount);
    offspring->weightVoteOverAllParent(selectedParentPopulation);
    offspring->m_debug = chooseIdx;
    return offspring;
}

void EvolutionStrategy::updateSelfAdaptionMutation(Person &offspring) {
    // multi value learning rate
    // NOTE: dividend can be replace to any other value
    const float tau = m_tauFactor / static_cast<float>(sqrt(2 * offspring->m_n));
    const float tauPrime = m_tauPrimeFactor / static_cast<float>(sqrt(2 * sqrt(offspring->m_n)));
    const float EPSILON = m_epsilonFactor;

    for (int i = 0; i < offspring->m_n; ++i) {
        offspring->m_selfAdaptionList[i] = max(offspring->m_selfAdaptionList[i] *
                                               exp(tau * Util::randomNormal() +
                                                   tauPrime * Util::randomNormal()), EPSILON);
    }
}

void mutation(Person &offspring) {
    for (int i = 0; i < offspring->m_n; ++i) {
        // Project it to non-zero
        (*offspring)[i] = Util::clamp(
                (*offspring)[i] + offspring->m_selfAdaptionList[i] * Util::randomNormal(),
                .01f, 1.f);
    }
}

Population survivorSelection(const Population &parentPopulations, const Population &offspringPopulations) {
    // (mu, lambda) selection
    std::vector<PersonQuality> personQualityList;
    personQualityList.reserve(offspringPopulations.size());
    for (const auto &offspringPopulation : offspringPopulations) {
        personQualityList.emplace_back(offspringPopulation->getPrecalculateObjective(), offspringPopulation);
    }
    // (mu + lambda) selection - uncomment this to convert to (mu + lambda) selection
    for (const auto &parentPopulation : parentPopulations) {
        personQualityList.emplace_back(parentPopulation->getPrecalculateObjective(), parentPopulation);
    }
    std::sort(personQualityList.begin(), personQualityList.end());
//    for(auto i: personQualityList) {
//        cout << i.first << " " << i.second->m_debug << endl;
//    }
//    getchar();

    Population newPopulation;
    newPopulation.reserve(parentPopulations.size());
    for (int i = 0; i < parentPopulations.size(); ++i) {
        newPopulation.emplace_back(personQualityList[i].second);
    }
    return newPopulation;
}