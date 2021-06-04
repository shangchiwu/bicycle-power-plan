#include <cstdlib>
#include <vector>
#include <memory>

class Encoding;

namespace Util {

    float randomBetween0To1();

    int greatestCommonDivisor(int a, int b);

    int generateRandomParentIdx(const std::vector <std::shared_ptr<Encoding >> &parentPopulations);

    float clamp(float val, float ra, float rb);

}
