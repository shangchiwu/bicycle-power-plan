//
// Created by Edge on 2021/5/18.
//

#include "BicyclePlan.h"

using namespace std;

shared_ptr<BicyclePlan> BicyclePlan::instance = nullptr;

bool BicyclePlan::initialize(const std::string &filepath) {
    if (0) {
        // initialize success
        return true;
    }
    return false;
}

std::shared_ptr<BicyclePlan> BicyclePlan::getInstance() {
    return instance;
}

float BicyclePlan::evaluate(shared_ptr<Encoding> offspring) {

}