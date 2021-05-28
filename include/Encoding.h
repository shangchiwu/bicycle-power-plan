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
#include "util.h"
#include "EvolutionalStrategy.h"
#include <iostream>
#include <memory>

typedef std::shared_ptr<Encoding > Person;

class Encoding {
public:
    Encoding(int N, bool randomShuffle = true);

    Encoding &operator=(const Encoding &encoding);

    friend std::ostream &operator<<(std::ostream &os, const Encoding &encoding);

    Encoding &swapElement(int aPos, int bPos);

    Encoding &shiftElement(int shiftAmount);

    float &operator[](int idx);

    float operator[](int idx) const;

    float getPrecalculateObjective();

    void averageOverAllParent(const std::vector<Person> &parentPopulation);

    int m_n;
    std::vector<float> m_powerList;
    float m_precalculateObjective;
    float m_selfAdaption;

private:
    bool m_dirtyFlag;
};


#endif //BICYCLEPOWERPLAN_ENCODING_H