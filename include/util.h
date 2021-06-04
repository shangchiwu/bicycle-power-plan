#ifndef BICYCLEPOWERPLAN_UTIL_H
#define BICYCLEPOWERPLAN_UTIL_H

namespace Util {

    // random utility
    int randomIntUniform(int min, int max);

    float randomFloatUniform();

    float randomNormal();

    // math utility
    float clamp(float val, float ra, float rb);

    int greatestCommonDivisor(int a, int b);
}

#endif // BICYCLEPOWERPLAN_UTIL_H
