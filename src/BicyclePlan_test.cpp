#include "BicyclePlan.h"
#include <iostream>
#include <string>
#include <vector>
#include "Encoding.h"

int main() {

    // construct plan

    std::cout << "Input plan file path (plan_*.json): " << std::endl;
    std::string planFilePath;
    std::getline(std::cin, planFilePath);

    std::cout << "Construct plan..." << std::endl;
    BicyclePlan plan;

    std::cout << "Reading config..." << std::endl;
    plan.readConfig(planFilePath);

    std::cout << "plainName: " << plan.m_planName << std::endl;
    std::cout << "cyclistName: " << plan.m_cyclist.m_cyclistName << std::endl;
    std::cout << "trackName: " << plan.m_track.m_trackName << std::endl;
    std::cout << std::endl;

    // evaluate vector

    std::vector<float> segmentOutputPowerRatio(plan.m_track.m_numSegments, 0.5);

    std::cout << "segmentOutputPowerRatio: " << std::endl;
    for (const float &x : segmentOutputPowerRatio) {
        std::cout << " " << x << ",";
    }
    std::cout << std::endl;

    std::cout << "evaluating..." << std::endl;

    const float totalTime = plan.evaluate(segmentOutputPowerRatio);

    std::cout << "totalTime: " << totalTime << std::endl;
    std::cout << std::endl;

    // end test

    std::cout << "END" << std::endl;

    return 0;
}
