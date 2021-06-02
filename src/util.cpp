#include "util.h"
#include <memory>
#include <vector>
#include <EvolutionalStrategy.h>

namespace Util {
    float randomBetween0To1() {
        return rand() / RAND_MAX;
    }

    int greatestCommonDivisor(int a, int b) {
        return b == 0 ? a : greatestCommonDivisor(b, a % b);
    }

    int generateRandomParentIdx(const Population &parentPopulations) {
        int idx = static_cast<int>(parentPopulations.size() * randomBetween0To1());
        if (idx >= parentPopulations.size()) {
            idx--;
        }
        return idx;
    }

    float clamp(float val, float ra, float rb) {
        return val < ra?ra:val>rb?rb:val;
    }

}
