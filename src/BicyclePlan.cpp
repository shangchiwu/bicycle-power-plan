//
// Created by Edge on 2021/5/18.
//

#include "BicyclePlan.h"
#include "Track.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "nlohmann/json.hpp"

std::shared_ptr<BicyclePlan> BicyclePlan::m_instance = nullptr;


BicyclePlan::BicyclePlan() {}
BicyclePlan::BicyclePlan(const BicyclePlan &other) : m_cyclist(other.m_cyclist), m_track(other.m_track) {}

bool BicyclePlan::readConfig(const std::string &configFilePath) {
    if (m_instance == nullptr) {
        m_instance = std::make_shared<BicyclePlan>();
    }
    std::ifstream ifs(configFilePath);

    if (!ifs.good()) {
        std::cerr << "Error: BicyclePlan::readConfig(): faild to read config file \"" <<
                     configFilePath << "\"!" << std::endl;
        return false;
    }

    nlohmann::json config;
    ifs >> config;
    ifs.close();

    m_instance->m_planName = config["planName"];
    const std::string cyclistFile = config["cyclistFile"];
    const std::string trackFile = config["trackFile"];

    bool result = true;
    result &= m_instance->m_cyclist.readConfig(cyclistFile);
    result &= m_instance->m_track.readConfig(trackFile);

    return result;
}

std::shared_ptr<BicyclePlan> BicyclePlan::getInstance() {
    return m_instance;
}


float BicyclePlan::evaluate(std::shared_ptr<Encoding> offspring) {
    // TODO
}
