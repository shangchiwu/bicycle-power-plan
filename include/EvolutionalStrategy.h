//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H
#define BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H

#include <vector>
#include <memory>
#include "Encoding.h"
#include "BicyclePlan.h"

std::shared_ptr<Encoding> evolutionalStrategy(float mu, float rho, shared_ptr<BicyclePlan> bicyclePlan);

void initializePopulation(std::vector <std::shared_ptr<Encoding >> &parentPopulations);

std::shared_ptr<Encoding> generateOffspringFromPopulation(const std::vector <std::shared_ptr<Encoding >> &parentPopulations);

void updateSelfAdaptionRecombination(std::shared_ptr<Encoding> offspring);

void recombination(std::shared_ptr<Encoding> offspring);

void updateSelfAdaptionMutation(std::shared_ptr<Encoding> offspring);

void mutation(std::shared_ptr<Encoding> offspring);

#endif //BICYCLEPOWERPLAN_EVOLUTIONSTRATEGY_H
