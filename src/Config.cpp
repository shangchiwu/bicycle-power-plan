//
// Created by Edge on 2021/6/5.
//

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Config.h"

using namespace std;

std::shared_ptr<Config> Config::m_instance;

bool Config::readConfig(const string &configFilePath) {
    if (m_instance == nullptr) {
        m_instance = std::make_shared<Config>();
    }
    std::ifstream ifs(configFilePath);

    if (!ifs.good()) {
        std::cerr << "Error: Config::readConfig(): failed to read config file \"" <<
                  configFilePath << "\"!" << std::endl;
        return false;
    }

    nlohmann::json config;
    ifs >> config;
    ifs.close();

    m_instance->m_bicyclePowerPlanLocation = config["bicyclePowerPlanLocation"];

    m_instance->m_parentPopulationSize = config["parentPopulationSize"];
    m_instance->m_selectPopulationSize = config["selectPopulationSize"];
    m_instance->m_offspringPopulationSize = config["offspringPopulationSize"];

    m_instance->m_statisticLocation = config["statistic_filename"];
    m_instance->m_encodingLocation = config["output_encoding_filename"];

    m_instance->m_iteration = config["iteration"];

    m_instance->m_tauFactor = config["tauFactor"];
    m_instance->m_tauPrimeFactor = config["tauPrimeFactor"];
    m_instance->m_epsilonFactor = config["epsilonFactor"];

    m_instance->m_seed = config["seed"];

    return true;
}

shared_ptr<Config> Config::getInstance() {
    return m_instance;
}