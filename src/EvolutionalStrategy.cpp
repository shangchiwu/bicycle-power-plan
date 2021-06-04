#include <random>
#include <algorithm>
#include "EvolutionalStrategy.h"
#include "BicyclePlan.h"

using namespace std;

Person evolutionStrategy(int parentPopulationSize, int selectParentSize, int offspringPopulationSize) {
    // Reference from page 6 of http://www.cmap.polytechnique.fr/~nikolaus.hansen/es-overview-2015.pdf
    // mu is parentPopulationSize
    // rho is selectParentSize
    // lambda is offspringPopulationSize
    constexpr int ITERATION = 10000;

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
        for (int j = 0; j < offspringPopulationSize; ++j) {
            Person newOffspring = generateOffspringFromPopulation(parentPopulations, selectParentSize);
            updateSelfAdaptionMutation(newOffspring);
            mutation(newOffspring);
            offspringPopulations.emplace_back(newOffspring);
        }
        parentPopulations = survivorSelection(parentPopulations, offspringPopulations);
        bestPerson = getBestPersonFromPopulation(parentPopulations);
//        for(auto person: parentPopulations) {
//            cout << "Encoding: " << *person << ", objective is " << person->getPrecalculateObjective() << endl;
//        }
//        getchar();
        offspringPopulations.clear();
    }
    return bestPerson;
}

Person getBestPersonFromPopulation(Population &population) {
    float bestQuality = 1e9;
    Person bestPerson;
    // FIXME I assume the better quality have less value
    for (auto &person: population) {
        const float quality = person->getPrecalculateObjective();
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

Person generateOffspringFromPopulation(const Population &parentPopulations, int selectParentSize) {
    // In page 7 "Recombination Operators" of http://www.cmap.polytechnique.fr/~nikolaus.hansen/es-overview-2015.pdf
    // Use intermediate recombination
    Population selectedParentPopulation;
    selectedParentPopulation.reserve(selectParentSize);
    for (int i = 0; i < selectParentSize; ++i) {
        // FIXME currently use random select parent
        selectedParentPopulation.emplace_back(parentPopulations[Util::randomIntUniform(0, static_cast<int>(parentPopulations.size())-1)]);
    }
    const int roadAmount = parentPopulations[0]->m_n;
    Person offspring = make_shared<Encoding>(roadAmount);
    offspring->averageOverAllParent(selectedParentPopulation);
    return offspring;
}

void updateSelfAdaptionMutation(Person &offspring) {
    // multi value learning rate
    // NOTE: dividend can be replace to any other value
    const float tau = 1.f / static_cast<float>(sqrt(2 * offspring->m_n));
    const float tauPrime = 1.f / static_cast<float>(sqrt(2 * sqrt(offspring->m_n)));
    constexpr float EPSILON = 1.0f;

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
        (*offspring)[i] += offspring->m_selfAdaptionList[i] * Util::randomNormal();
        (*offspring)[i] = Util::clamp((*offspring)[i], .01f, 1.f);
    }
}

Population survivorSelection(const Population &parentPopulations, const Population &offspringPopulations) {
    // (mu, lambda) selection
    // TODO should assert parent population and offspring population
    std::vector<PersonQuality> personQualityList;
    personQualityList.reserve(offspringPopulations.size());
    for (const auto &offspringPopulation : offspringPopulations) {
        personQualityList.emplace_back(offspringPopulation->getPrecalculateObjective(), offspringPopulation);
    }
    // assume lower is better
    std::sort(personQualityList.begin(), personQualityList.end());

    Population newPopulation;
    newPopulation.reserve(parentPopulations.size());
    for (int i = 0; i < parentPopulations.size(); ++i) {
        newPopulation.emplace_back(personQualityList[i].second);
    }
    return newPopulation;
}