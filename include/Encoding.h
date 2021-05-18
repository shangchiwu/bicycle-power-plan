//
// Created by Edge on 2021/5/18.
//

// Encoding should store
// 1. real-value encoding
// 2. calculated F value of current real-value encoding
// 3. self-adaptive recombination parameter
// 4. self-adaptive mutation parameter
// (They are encoding-wise, which means every encoding should have its own self-adaption parameter
// to self-adapt it on two operation)

#ifndef BICYCLEPOWERPLAN_ENCODING_H
#define BICYCLEPOWERPLAN_ENCODING_H

#include <vector>
#include "Util.h"

class Encoding {
public:
    Encoding(int N, bool randomShuffle = true);

    Encoding &operator=(const Encoding &encoding);

    std::vector<float> m_powerList;
    float m_precalculateObjective;
    float m_selfAdaptRecombination;
    float m_selfAdaptMutation;

private:
    bool m_dirtyFlag;
};

#endif //BICYCLEPOWERPLAN_ENCODING_H