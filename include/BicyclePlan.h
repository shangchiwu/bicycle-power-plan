//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_BICYCLEPLAN_H
#define BICYCLEPOWERPLAN_BICYCLEPLAN_H

#include <memory>
#include <string>
#include <vector>
#include "Cyclist.h"
#include "Track.h"

class Encoding;

class BicyclePlan  {
public:
    BicyclePlan();

    BicyclePlan(const BicyclePlan &other);

    static bool readConfig(const std::string &configFilePath);

    static std::shared_ptr<BicyclePlan> getInstance();

    float evaluate(const std::shared_ptr<Encoding> &offspring);

    std::string m_planName;
    Cyclist m_cyclist;
    Track m_track;
private:
    static std::shared_ptr<BicyclePlan> m_instance;
};

#endif // BICYCLEPOWERPLAN_BICYCLEPLAN_H
