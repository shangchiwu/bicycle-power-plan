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

class BicyclePlan {
public:
    BicyclePlan();
    BicyclePlan(const BicyclePlan &other);

    static bool initialize(const std::string &filepath);
    static std::shared_ptr<BicyclePlan> getInstance();
    float evaluate(std::shared_ptr<Encoding> offspring);

    bool readConfig(const std::string &configFilePath);
    float evaluate(const std::vector<float> &segmentOutputPowerRatio);

    std::string m_planName;
    Cyclist m_cyclist;
    Track m_track;

private:
    static std::shared_ptr<BicyclePlan> instance;
};

#endif // BICYCLEPOWERPLAN_BICYCLEPLAN_H
