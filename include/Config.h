//
// Created by Edge on 2021/6/5.
//

#ifndef BICYCLEPOWERPLAN_CONFIG_H
#define BICYCLEPOWERPLAN_CONFIG_H

#include <string>
#include <memory>

class Config {
public:
    static bool readConfig(const std::string &configFilePath);

    static std::shared_ptr<Config> getInstance();


    int m_parentPopulationSize;
    int m_selectPopulationSize;
    int m_offspringPopulationSize;

    std::string m_bicyclePowerPlanLocation;
    std::string m_statisticLocation;
    std::string m_encodingLocation;

    int m_seed;

    float m_tauFactor = 1.0f;
    float m_tauPrimeFactor = 1.0f;
    float m_epsilonFactor = 1.0f;

private:
    static std::shared_ptr<Config> m_instance;

};


#endif //BICYCLEPOWERPLAN_CONFIG_H
