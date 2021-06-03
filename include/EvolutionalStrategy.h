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


Person evolutionStrategy(int parentPopulationSize, int selectParentSize, int offspringPopulationSize);

Person getBestPersonFromPopulation(Population &population);

void initializePopulation(Population &parentPopulations, int parentPopulationSize);

Person generateOffspringFromPopulation(const Population &parentPopulations, int selectParentSize);

void updateSelfAdaptionMutation(Person &offspring);

void mutation(Person &offspring);

Population survivorSelection(const Population &parentPopulations, const Population &offspringPopulations);

#endif //BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H
