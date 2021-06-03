#ifndef BICYCLEPOWERPLAN_CYCLIST_H
#define BICYCLEPOWERPLAN_CYCLIST_H

#include <string>

class Cyclist {
public:
    Cyclist();
    Cyclist(const Cyclist &other) = default;

    bool readConfig(const std::string &configFilePath);

    std::string m_cyclistName;

    // cyclist info
    float m_maxPower;               // unit in (Watt)
    float m_totalEnergy;            // unit in (kJ)
    float m_cyclistWeight;          // unit in (kg)
    float m_frontalAreaCoef;        // range in 0~1

    // bicycle info
    float m_bicycleWeight;          // unit in (kg)
    float m_rollingResistanceCoef;  // range in 0~1
    float m_transmissionEfficiency; // range in 0~1
};

#endif // BICYCLEPOWERPLAN_CYCLIST_H
