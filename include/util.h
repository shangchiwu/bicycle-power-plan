#include <vector>
#include <memory>
#include <

namespace Util {

    // random utility
    int randomIntUniform();
    float randomFloatUniform();
    float randomNromal();

    // math utility
    float clamp(float val, float ra, float rb);
    int greatestCommonDivisor(int a, int b);
}
