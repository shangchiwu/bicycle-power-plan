//
// Created by Edge on 2021/5/18.
//

#include "BicyclePlan.h"
#include "Track.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "nlohmann/json.hpp"
#include "Encoding.h"

std::shared_ptr<BicyclePlan> BicyclePlan::m_instance = nullptr;

BicyclePlan::BicyclePlan() {}
BicyclePlan::BicyclePlan(const BicyclePlan &other) : m_cyclist(other.m_cyclist), m_track(other.m_track) {}

bool BicyclePlan::readConfig(const std::string &configFilePath) {
    if (m_instance == nullptr) {
        m_instance = std::make_shared<BicyclePlan>();
    }
    std::ifstream ifs(configFilePath);

    if (!ifs.good()) {
        std::cerr << "Error: BicyclePlan::readConfig(): failed to read config file \"" <<
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
    size_t pos = configFilePath.find_last_of("\\/");
    auto pathPrefix = (std::string::npos == pos)
                      ? configFilePath
                      : configFilePath.substr(0, pos+1);
    result &= m_instance->m_cyclist.readConfig(pathPrefix+cyclistFile);
    result &= m_instance->m_track.readConfig(pathPrefix+trackFile);

    return result;
}

std::shared_ptr<BicyclePlan> BicyclePlan::getInstance() {
    return m_instance;
}

float BicyclePlan::evaluate(const std::shared_ptr<Encoding> &offspring) {

    // constant params
    constexpr int newtonMaxIterations = 10;  // for newton's method
    constexpr float newtonMaxTolerance = 0.001f;
    constexpr float failureVelocity = 0.05f;

    // accumulative params
    float leftEnergy = m_cyclist.m_totalEnergy;
    float totalTime = 0.0f;

    // iterate all segments
    for (int i = 0; i < m_track.m_numSegments; ++i) {

        // compute penalty of low energy
        const Segment &segment = m_track[i];
        const float leftEnergyRatio = leftEnergy / m_cyclist.m_totalEnergy;
        const float maxOutputPowerRatio = (1.0f - std::pow(1.0f - leftEnergyRatio, 3.0f));
        // const float outputPower = std::min((*offspring)[i], maxOutputPowerRatio) * m_cyclist.m_maxPower;
        const float outputPower = (*offspring)[i] * m_cyclist.m_maxPower;

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
            velocityIter -= f / fp;
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
        leftEnergy -= consumedEnergy;
        // const float weightLoss = consumedEnergy / 32318.0f;
        
        // compute time with break time
        velocity *= maxOutputPowerRatio;

        // test whether the cyclist is dead
        if (leftEnergy <= 0.0f) {
            velocity = failureVelocity;
        }

        time = segment.m_distance / velocity;
        totalTime += time;
    }

    return totalTime;
}
