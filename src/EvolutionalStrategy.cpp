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
    const int ITERATION = 10000;

    Population parentPopulations;
    Person bestPerson;
    initializePopulation(parentPopulations, parentPopulationSize);
//    for(auto person: parentPopulations) {
//        cout << "Encoding: " << *person << ", objective is " << person->getPrecalculateObjective() << endl;
//    }
//    getchar();
    bestPerson = getBestPersonFromPopulation(parentPopulations);
    for (int i = 0; i < ITERATION; ++i) {
//        cout << "Iteration: " << i << endl;
        Population offspringPopulations;
        for (int j = 0; j < offspringPopulationSize; ++j) {
            Person newOffspring = generateOffspringFromPopulation(parentPopulations, selectParentSize);
            updateSelfAdaptionMutation(newOffspring);
            mutation(newOffspring);
            offspringPopulations.push_back(newOffspring);
        }
        parentPopulations = survivorSelection(parentPopulations, offspringPopulations);
        bestPerson = getBestPersonFromPopulation(parentPopulations);
//        for(auto person: parentPopulations) {
//            cout << "Encoding: " << *person << ", objective is " << person->getPrecalculateObjective() << endl;
//        }
//        getchar();
    }
    return bestPerson;
}

Person getBestPersonFromPopulation(Population &population) {
    float bestQuality = 1e9;
    Person bestPerson;
    // FIXME I assume the better quality have less value
    for (auto &person: population) {
        float quality = person->getPrecalculateObjective();
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
    for(int i = 0;i < parentPopulationSize;++i) {
        parentPopulations.push_back(make_shared<Encoding>(bicyclePlan->m_track.m_numSegments));
    }
}

Person generateOffspringFromPopulation(const Population &parentPopulations, int selectParentSize) {
    // In page 7 "Recombination Operators" of http://www.cmap.polytechnique.fr/~nikolaus.hansen/es-overview-2015.pdf
    // Use intermediate recombination
    Population selectedParentPopulation;
    for (int i = 0; i < selectParentSize; ++i) {
        // FIXME currently use random select parent
        selectedParentPopulation.push_back(parentPopulations[Util::generateRandomParentIdx(parentPopulations)]);
    }
    int roadAmount = parentPopulations[0]->m_n;
    Person offspring = make_shared<Encoding>(roadAmount);
    offspring->averageOverAllParent(selectedParentPopulation);
    return offspring;
}

void updateSelfAdaptionMutation(Person &offspring) {
    // first order learning rate
    float tau = 1.f / static_cast<float>(sqrt(2 * offspring->m_n));
    float EPSILON = 0.3f;

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    normal_distribution<float> normalDistribution(0.0, 1.0);

    offspring->m_selfAdaption = max(offspring->m_selfAdaption * exp(tau * normalDistribution(generator)), EPSILON);
}

void mutation(Person &offspring) {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    normal_distribution<float> normalDistribution(0.0, 1.0);
    for (auto &gene: offspring->m_powerList) {
        gene += offspring->m_selfAdaption * normalDistribution(generator);
        // Project it to non-zero
        if (gene < 0.01f) {
            gene = 0.01f;
        } else if (gene > 1.0f) {
            gene = 1.0f;
        }
    }

}

Population survivorSelection(const Population &parentPopulations, const Population &offspringPopulations) {
    // (mu, lambda) selection
    // TODO should assert parent population and offspring population
    Population newPopulation;
    std::vector<PersonQuality> personQualityList;
    for (const auto &offspringPopulation : offspringPopulations) {
        PersonQuality personQuality = PersonQuality(offspringPopulation->getPrecalculateObjective(),
                                                    offspringPopulation);
        personQualityList.push_back(personQuality);
    }
    // assume lower is better
    std::sort(personQualityList.begin(), personQualityList.end());
    for (int i = 0; i < parentPopulations.size(); ++i) {
        newPopulation.push_back(personQualityList[i].second);
    }
    return newPopulation;
}