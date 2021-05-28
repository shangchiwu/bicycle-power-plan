//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_BICYCLEPLAN_H
#define BICYCLEPOWERPLAN_BICYCLEPLAN_H

#include <memory>

class Encoding;

class BicyclePlan {
public:

    static bool initialize(const std::string &filepath);

    static std::shared_ptr<BicyclePlan> getInstance();

    float evaluate(std::shared_ptr<Encoding> offspring);

private:
    static std::shared_ptr<BicyclePlan> instance;
};


#endif //BICYCLEPOWERPLAN_BICYCLEPLAN_H
