//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_BICYCLEPLAN_H
#define BICYCLEPOWERPLAN_BICYCLEPLAN_H

#include <memory>

class Encoding;

class BicyclePlan {
public:
    BicyclePlan(const std::string &filepath);

    float evaluate(std::shared_ptr<Encoding> offspring);

private:

};


#endif //BICYCLEPOWERPLAN_BICYCLEPLAN_H
