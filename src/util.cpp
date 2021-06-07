#include "util.h"
#include <random>
#include <ctime>
#include <Config.h>

namespace Util {

    // random utility

    static std::mt19937 randomGenerator((unsigned int) std::time(nullptr));
    static std::uniform_real_distribution<float> uniformFloatDistribution(0.0f, 1.0f);
    static std::normal_distribution<float> normalDistribution(0.0f, 1.0f);

    void initializeUtil() {
        auto config = Config::getInstance();
        randomGenerator = std::mt19937(config->m_seed);
    }

    int randomIntUniform(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(randomGenerator);
    }

    float randomFloatUniform() {
        return uniformFloatDistribution(randomGenerator);
    }

    float randomNormal() {
        return normalDistribution(randomGenerator);
    }

    // math utility

    float clamp(float val, float ra, float rb) {
        return val < ra ? ra : val > rb ? rb : val;
    }

    int greatestCommonDivisor(int a, int b) {
        return b == 0 ? a : greatestCommonDivisor(b, a % b);
    }

}
