//
// Created by Edge on 2021/5/18.
//

#include "BicyclePlan.h"
#include <cmath>
#include <assert.h>
#include "Encoding.h"

using namespace std;

shared_ptr<BicyclePlan> BicyclePlan::m_instance = nullptr;

float SegmentFunction::fatigueMultiplier(float fatigue) {
    return fatigue * fatigue;
}

bool BicyclePlan::initialize(const std::string &filepath) {
    if (m_instance == nullptr) {
        m_instance = make_shared<BicyclePlan>();
    }
    m_instance->m_segmentFunctionList.push_back(make_shared<DoubleSegmentFunction>(2.1));
    m_instance->m_segmentFunctionList.push_back(make_shared<DoubleSegmentFunction>(0.7));
    // FIXNE adding below, something will blow and the answer will blow too
//    m_instance->m_segmentFunctionList.push_back(make_shared<OneDAckleySegmentFunction>());
//    m_instance->m_segmentFunctionList.push_back(
//            make_shared<CompositeSegmentFunction>(make_shared<OneDAckleySegmentFunction>(),
//                                                  make_shared<DoubleSegmentFunction>(0.4)));

    m_instance->m_segmentSize = m_instance->m_segmentFunctionList.size();
    return true;
}

std::shared_ptr<BicyclePlan> BicyclePlan::getInstance() {
    return m_instance;
}

int BicyclePlan::getSegmentSize() {
    return m_segmentSize;
}

float BicyclePlan::evaluate(shared_ptr<Encoding> offspring) {
    assert(m_segmentFunctionList.size() == offspring->m_n);
    float currentFatigue = m_initFatigue;
    float totalFunctionValue = 0.0f;
    for (int i = 0; i < m_segmentFunctionList.size(); ++i) {
        float functionValue = m_segmentFunctionList[i]->evaluate((*offspring)[i], currentFatigue);
        fatigueUpdate(currentFatigue, functionValue);
        totalFunctionValue += functionValue;
    }
    return totalFunctionValue;
}

void BicyclePlan::fatigueUpdate(float &fatigue, float functionValue) {
    static const float ra = 0.001f, rb = 2.38f;
    if (abs(functionValue) < 1e-7) {
        fatigue += rb;
    } else {
        float addedFatigue = Util::clamp(1 / functionValue, ra, rb);
        fatigue += addedFatigue;
    }
}

float DoubleSegmentFunction::evaluate(float power, float fatigue) {
    return fatigueMultiplier(fatigue) * m_alpha * power * power;
}

float DoubleSineSegmentFunction::evaluate(float power, float fatigue) {
    return fatigueMultiplier(fatigue) * m_alpha * sin(m_beta * power * power);
}

float OneDAckleySegmentFunction::evaluate(float power, float fatigue) {
    static const float PI = acos(-1);
    static const float e = exp(1);
    return fatigueMultiplier(fatigue) * -20. * exp(-0.2 * power) - exp(cos(2 * PI * power)) + 20.0 + e;
}

float CompositeSegmentFunction::evaluate(float power, float fatigue) {
    return m_compFunction_1->evaluate(m_compFunction_2->evaluate(power, 1.0f), fatigue);
}