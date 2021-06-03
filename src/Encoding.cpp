#include <cassert>
#include <BicyclePlan.h>
#include "Encoding.h"

using namespace std;

Encoding::Encoding(int N, bool randomShuffle) {
    m_powerList.reserve(N);
    if (randomShuffle) {
        for (int i = 0; i < N; ++i) {
            m_powerList[i] = Util::randomBetween0To1();
        }
    } else {
        for (int i = 0; i < N; ++i) {
            m_powerList[i] = (i + 1) / (float) N;
        }
    }

    m_dirtyFlag = true;
    m_precalculateObjective = 0;
    // TODO initialize m_selfAdaption
}


Encoding &Encoding::operator=(const Encoding &encoding) {
    if (&encoding != this) {
        m_powerList.reserve(encoding.m_powerList.size());
        for (int i = 0; i < encoding.m_powerList.size(); ++i) {
            m_powerList[i] = encoding.m_powerList[i];
        }
        m_precalculateObjective = encoding.m_precalculateObjective;
        m_selfAdaption = encoding.m_selfAdaption;

        m_dirtyFlag = encoding.m_dirtyFlag;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Encoding &encoding) {
    for (int i = 0; i < encoding.m_n; ++i) {
        os << encoding.m_powerList[i] << " ";
    }
    return os;
}

float &Encoding::operator[](int idx) {
    assert(idx >= 0 && idx < m_n);
    m_dirtyFlag = true;
    return m_powerList[idx];
}

float Encoding::operator[](int idx) const {
    assert(idx >= 0 && idx < m_n);
    return m_powerList[idx];
}

Encoding &Encoding::swapElement(int aPos, int bPos) {
    int tmp = m_powerList[aPos];
    m_powerList[aPos] = m_powerList[bPos];
    m_powerList[bPos] = tmp;
    m_dirtyFlag = true;
    return *this;
}

Encoding &Encoding::shiftElement(int shiftAmount) {
    int i, j, k, tmp;
    if (m_n <= 1 || shiftAmount == 0) return *this;
    shiftAmount = shiftAmount % m_n; // make sure shift isn't >n
    int gcd = Util::greatestCommonDivisor(m_n, shiftAmount);

    for (i = 0; i < gcd; i++) {
        // start cycle at i
        tmp = m_powerList[i];
        for (j = i; true; j = k) {
            k = j + shiftAmount;
            if (k >= m_n) k -= m_n; // wrap around if we go outside array
            if (k == i) break; // end of cycle
            m_powerList[j] = m_powerList[k];
        }
        m_powerList[j] = tmp;
    }
    return *this;
}

float Encoding::getPrecalculateObjective() {
    if(m_dirtyFlag) {
        const shared_ptr<BicyclePlan> bicyclePlan = BicyclePlan::getInstance();
        m_precalculateObjective = bicyclePlan->evaluate(std::unique_ptr<Encoding>(this));
        m_dirtyFlag = false;
    }
    return m_precalculateObjective;
}

void Encoding::averageOverAllParent(const std::vector<Person> &parentPopulation) {
    float parentPopulationSize = parentPopulation.size();
    for (int i = 0;i < m_n;++i) {
        m_powerList[i] = 0.0f;
        for(auto &parent: parentPopulation) {
            m_powerList[i] += (*parent)[i];
        }
        m_powerList[i] /= parentPopulationSize;
    }
}