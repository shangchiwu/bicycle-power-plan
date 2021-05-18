#include "EvolutionalStrategy.h"
#include "BicyclePlan.h"

using namespace std;

shared_ptr<Encoding> evolutionalStrategy(float mu, float rho, shared_ptr<BicyclePlan> bicyclePlan) {
    const int ITERATION = 10000;
    const int OFFSPRING_PER_ITERATION = 32;

    vector <shared_ptr<Encoding >> parentPopulations;
    shared_ptr<Encoding> bestEncoding;
    initializePopulation(parentPopulations);
    for (int i = 0; i < ITERATION; ++i) {
        vector <shared_ptr<Encoding >> offspringPopulations;
        for (int j = 0; j < OFFSPRING_PER_ITERATION; ++j) {
            // TODO Generate offspring from any parent
            shared_ptr <Encoding> newOffspring = generateOffspringFromPopulation(parentPopulations);
            // TODO Recombination self-adaption
            updateSelfAdaptionRecombination(newOffspring);
            // TODO Recombination
            recombination(newOffspring);
            // TODO Mutation self-adaption
            updateSelfAdaptionMutation(newOffspring);
            // TODO Mutation
            mutation(newOffspring);
            // TODO Calculate F function of this encoding
            bicyclePlan->evaluate(newOffspring);
            // TODO Add to offspring population
            offspringPopulations.push_back(newOffspring);
        }
        // TODO Select new populations from both parent population and offspring population
        // TODO Record best population
    }
    return bestEncoding;
}

void initializePopulation(vector <shared_ptr<Encoding >> &parentPopulations) {
    // TODO Project initial encoding to feasible encoding
}

shared_ptr<Encoding> generateOffspringFromPopulation(const vector <shared_ptr<Encoding >> &parentPopulations) {

}

void updateSelfAdaptionRecombination(shared_ptr<Encoding> offspring) {

}

void recombination(shared_ptr<Encoding> offspring) {

}

void updateSelfAdaptionMutation(shared_ptr<Encoding> offspring) {

}

void mutation(shared_ptr<Encoding> offspring) {

}