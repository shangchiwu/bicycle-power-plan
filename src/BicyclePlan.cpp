//
// Created by Edge on 2021/5/18.
//

#include "BicyclePlan.h"
#include "Track.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

BicyclePlan::BicyclePlan() {}
BicyclePlan::BicyclePlan(const BicyclePlan &other) : m_cyclist(other.m_cyclist), m_track(other.m_track) {}

std::shared_ptr<BicyclePlan> BicyclePlan::instance = nullptr;

bool BicyclePlan::initialize(const std::string &filepath) {
    std::shared_ptr<BicyclePlan> plan;
    const bool result = plan->readConfig(filepath);

    if (!result)  // failed to read config file
        return false;

    // set static instance
    instance = plan;

    return true;
}

std::shared_ptr<BicyclePlan> BicyclePlan::getInstance() {
    return instance;
}

float BicyclePlan::evaluate(std::shared_ptr<Encoding> offspring) {
    // TODO: evaluate encoding by output power vector
    return 0;
}

bool BicyclePlan::readConfig(const std::string &configFilePath) {
    std::ifstream ifs(configFilePath);
    
    if (!ifs.good()) {
        std::cerr << "Error: BicyclePlan::readConfig(): faild to read config file \"" <<
                     configFilePath << "\"!" << std::endl;
        return false;
    }

    nlohmann::json config;
    ifs >> config;
    ifs.close();

    m_planName = config["planName"];
    const std::string cyclistFile = config["cyclistFile"];
    const std::string trackFile = config["trackFile"];

    bool result = true;
    result &= m_cyclist.readConfig(cyclistFile);
    result &= m_track.readConfig(trackFile);

    return result;
}

float BicyclePlan::evaluate(const std::vector<float> &segmentOutputPowerRatio) {
    
    // constant params
    constexpr int newtonMaxIterations = 10;  // for newton's method
    constexpr float newtonMaxTolerance = 0.001f;
    constexpr float failureTimePenalty = 86400.0f;

    // accumulative params
    float leftEnergy = m_cyclist.m_totalEnergy;
    float totalTime = 0.0f;

    // iterate all segments
    for (int i = 0; i < m_track.m_numSegments; ++i) {
        
        // compute penalty of low energy
        const Segment &segment = m_track[i];
        const float leftEnergyRatio = leftEnergy / m_cyclist.m_totalEnergy;
        const float maxOutputPowerRatio = (1.0f - std::pow(1.0f - leftEnergyRatio, 3.0f));
        const float outputPower = std::min(segmentOutputPowerRatio[i], maxOutputPowerRatio) * m_cyclist.m_maxPower;

        // prepare params
        const float airDensity = (1.293f - 0.00426f * segment.m_temperature) * std::exp(segment.m_elevation / 7000.0f);
        const float airResistanceCoef = 0.5f * m_cyclist.m_frontalAreaCoef * airDensity;
        const float totalWeight = 9.8f * (m_cyclist.m_cyclistWeight + m_cyclist.m_bicycleWeight);
        const float totalResistance = totalWeight * (segment.m_grade + m_cyclist.m_rollingResistanceCoef);

        // approximate velocity by newton's method
        float velocity = 0.0f;
        float velocityIter = 20.0f;  // initial guess

        for (int iteration = 0; iteration < newtonMaxIterations; ++iteration) {
            const float totalWindSpeed = velocityIter + segment.m_windSpeed;
            const float airCoef = (totalWindSpeed > 0.0f) ? airResistanceCoef : -airResistanceCoef;
            const float f = velocityIter * (airCoef * totalWindSpeed * totalWindSpeed + totalResistance) - m_cyclist.m_transmissionEfficiency * outputPower;
            const float fp = airCoef * (3.0f * velocityIter + segment.m_windSpeed) * totalWindSpeed + totalResistance;
            const float velocityOld = velocityIter;
            velocityIter = velocityIter - f / fp;
            if (std::fabs(velocityIter - velocityOld) < newtonMaxTolerance) {
                velocity = velocityIter;  // converged
                break;
            }
            // velocity = 0 if failed to converge after all iterations
        }

        // compute time
        float time = 0.0f;
        if (velocity > 0.0f)
            time = segment.m_distance / velocity;
        
        // compute other params
        const float consumedEnergy = time * outputPower / 0.25f / 1000.0f;
        // const float weightLoss = consumedEnergy / 32318.0f;

        // record the result of this segment
        totalTime += time;
        leftEnergy -= consumedEnergy;
    }

    // test whether the cyclist is dead
    if (leftEnergy <= 0.0f)
        totalTime += failureTimePenalty;

    return totalTime;
}
