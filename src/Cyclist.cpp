#include "Cyclist.h"
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

Cyclist::Cyclist() :
        m_maxPower(0), m_totalEnergy(0), m_cyclistWeight(0), m_frontalAreaCoef(0),
        m_bicycleWeight(0), m_rollingResistanceCoef(0), m_transmissionEfficiency(0) {}

bool Cyclist::readConfig(const std::string &configFilePath) {
    std::ifstream ifs(configFilePath);

    if (!ifs.good()) {
        std::cerr << "Error: Cyclist::readConfig(): faild to read config file \"" <<
                     configFilePath << "\"!" << std::endl;
        return false;
    }

    nlohmann::json config;
    ifs >> config;
    ifs.close();

    m_cyclistName            = config["cyclistName"];
    m_maxPower               = config["maxPower"];
    m_totalEnergy            = config["totalEnergy"];
    m_cyclistWeight          = config["cyclistWeight"];
    m_frontalAreaCoef        = config["frontalAreaCoef"];
    m_bicycleWeight          = config["bicycleWeight"];
    m_rollingResistanceCoef  = config["rollingResistanceCoef"];
    m_transmissionEfficiency = config["transmissionEfficiency"];

    return true;
}
