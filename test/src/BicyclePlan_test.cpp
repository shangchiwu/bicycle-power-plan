#include "BicyclePlan.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Encoding.h"

int main() {

    // construct plan

    std::cout << "Input plan file path (plan_*.json): " << std::endl;
    std::string planFilePath;
    std::getline(std::cin, planFilePath);

    std::cout << "Construct plan..." << std::endl;

    std::cout << "Reading config..." << std::endl;
    bool success = BicyclePlan::readConfig(planFilePath);
    if (!success) {
        std::cerr << "Error loading config" << std::endl;
        exit(1);
    }
    std::shared_ptr<BicyclePlan> plan = BicyclePlan::getInstance();


    std::cout << "plainName: " << plan->m_planName << std::endl;
    std::cout << "cyclistName: " << plan->m_cyclist.m_cyclistName << std::endl;
    std::cout << "trackName: " << plan->m_track.m_trackName << std::endl;
    std::cout << std::endl;

    // evaluate vector

    std::shared_ptr<Encoding> encoding=std::make_shared<Encoding>(plan->m_track.m_numSegments);
    std::cout << plan->m_track.m_numSegments << std::endl;
    std::cout << "segmentOutputPowerRatio: " << std::endl;
    for (int i = 0;i < plan->m_track.m_numSegments; ++i) {
        (*encoding)[i] = 0.5f;
        std::cout << " " << (*encoding)[i] << ",";
    }

    std::cout << std::endl;

    std::cout << "evaluating..." << std::endl;

    const float totalTime = plan->evaluate(encoding);

    std::cout << "totalTime: " << totalTime << std::endl;
    std::cout << std::endl;

    // end test

    std::cout << "END" << std::endl;

    return 0;
}
