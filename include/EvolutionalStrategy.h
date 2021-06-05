//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H
#define BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H

#include <vector>
#include <memory>
#include "Encoding.h"
#include "BicyclePlan.h"


typedef std::shared_ptr<Encoding> Person;

typedef std::vector<Person> Population;

typedef std::pair<float, Person> PersonQuality;

class EvolutionStrategy {
public:
    void initialize();

    Person evaluate(int parentPopulationSize, int selectParentSize, int offspringPopulationSize);

    void updateSelfAdaptionMutation(Person &offspring);

    void statistic(const std::string &filepath) const;

    std::vector<float> m_executionTimeList;
    std::vector<float> m_evaluationTimeList;

    float m_tauFactor = 1.0f;
    float m_tauPrimeFactor = 1.0f;
    float m_epsilonFactor= 1.0f;
};

Person getBestPersonFromPopulation(Population &population);

void initializePopulation(Population &parentPopulations, int parentPopulationSize);

Person generateOffspringFromPopulation(const Population &parentPopulations, int selectParentSize);

void mutation(Person &offspring);

Population survivorSelection(const Population &parentPopulations, const Population &offspringPopulations);

#endif //BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H
