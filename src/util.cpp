#include "util.h"
#include <memory>
#include <vector>
#include <random>
#include <EvolutionalStrategy.h>

namespace Util {

    // random utility

    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<float> uniformFloatDistribution(0.0f, 1.0f);
    std::normal_distribution<float> normalDistribution(0.0f, 1.0f);

    int randomIntUniform(int min, int max) {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(randomGenerator);
    }

    float randomFloatUniform() {
        return uniformFloatDistribution(randomGenerator);
    }

    float randomNromal() {
        return normalDistribution(randomGenerator);
    }

    // math utility

    float clamp(float val, float ra, float rb) {
        return val < ra?ra:val>rb?rb:val;
    }

    int greatestCommonDivisor(int a, int b) {
        return b == 0 ? a : greatestCommonDivisor(b, a % b);
    }

}
