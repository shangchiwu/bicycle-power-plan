//
// Created by Edge on 2021/5/18.
//

#ifndef BICYCLEPOWERPLAN_BICYCLEPLAN_H
#define BICYCLEPOWERPLAN_BICYCLEPLAN_H

#include <memory>
#include <vector>
#include <string>

class Encoding;

class SegmentFunction {
public:
    virtual float evaluate(float power, float fatigue) = 0;

    virtual float fatigueMultiplier(float fatigue);
};

class BicyclePlan {
public:

    static bool initialize(const std::string &filepath);

    static std::shared_ptr<BicyclePlan> getInstance();

    float evaluate(std::shared_ptr<Encoding> offspring);

    void fatigueUpdate(float &fatigue, float functionValue);

    int getSegmentSize();
private:
    static std::shared_ptr<BicyclePlan> m_instance;

    const float m_initFatigue = 1.0f;

    std::vector<std::shared_ptr<SegmentFunction>> m_segmentFunctionList;

    int m_segmentSize;

};

class DoubleSegmentFunction : public SegmentFunction {
public:
    explicit DoubleSegmentFunction(float alpha = 1.0f) : m_alpha(alpha) {}

    virtual float evaluate(float power, float fatigue) final;

private:
    float m_alpha;
};

class DoubleSineSegmentFunction : public SegmentFunction {
public:
    explicit DoubleSineSegmentFunction(float alpha = 1.0f, float beta = 1.0f) : m_alpha(alpha), m_beta(beta) {}

    virtual float evaluate(float power, float fatigue) final;

private:
    float m_alpha, m_beta;
};

class OneDAckleySegmentFunction : public SegmentFunction {
public:
    virtual float evaluate(float power, float fatigue) final;

};

class CompositeSegmentFunction : public SegmentFunction {
public:
    CompositeSegmentFunction(std::shared_ptr<SegmentFunction> compFunction_1,
                             std::shared_ptr<SegmentFunction> compFunction_2) : m_compFunction_1(compFunction_1),
                                                                                m_compFunction_2(compFunction_2) {}

    virtual float evaluate(float power, float fatigue) final;

private:
    std::shared_ptr<SegmentFunction> m_compFunction_1, m_compFunction_2;
};

#endif //BICYCLEPOWERPLAN_BICYCLEPLAN_H
