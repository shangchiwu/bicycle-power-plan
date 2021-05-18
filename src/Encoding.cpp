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
    // TODO initialize m_selfAdaptRecombination, m_selfAdaptMutation
}


Encoding &Encoding::operator=(const Encoding &encoding) {
    m_powerList.reserve(encoding.m_powerList.size());
    for (int i = 0; i < encoding.m_powerList.size(); ++i) {
        m_powerList[i] = encoding.m_powerList[i];
    }
    m_precalculateObjective = encoding.m_precalculateObjective;
    m_selfAdaptRecombination = encoding.m_selfAdaptRecombination;
    m_selfAdaptMutation = encoding.m_selfAdaptMutation;

    m_dirtyFlag = encoding.m_dirtyFlag;
}