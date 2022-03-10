//
// Created by kskun on 2022/3/10.
//

#ifndef ASSIGNMENT7_SAMPLER_H
#define ASSIGNMENT7_SAMPLER_H

#include <random>
#include <ctime>

#include "vectors.h"

class Sampler {
public:
    explicit Sampler(int numSamples) : numSamples(numSamples) {}

    virtual ~Sampler() = default;

    virtual Vec2f getSamplePosition(int n) = 0;

protected:
    int numSamples;
};

class RandomSampler : public Sampler {
public:
    explicit RandomSampler(int numSamples) : Sampler(numSamples) {
        // timestamp as random seed
        rand.seed(time(nullptr));
    }

    Vec2f getSamplePosition(int n) override;

protected:
    std::mt19937 rand;
};

class UniformSampler : public Sampler {
public:
    explicit UniformSampler(int numSamples) : Sampler(numSamples) {
        nPerSide = round(sqrt(numSamples));
        assert(nPerSide * nPerSide == numSamples);
        gap = 1.0f / nPerSide;
    }

    Vec2f getSamplePosition(int n) override;

protected:
    int nPerSide;
    float gap;
};

class JitteredSampler : public Sampler {
public:
    explicit JitteredSampler(int numSamples) : Sampler(numSamples) {
        nPerSide = round(sqrt(numSamples));
        assert(nPerSide * nPerSide == numSamples);
        gap = 1.0f / nPerSide;
        // timestamp as random seed
        rand.seed(time(nullptr));
    }

    Vec2f getSamplePosition(int n) override;

protected:
    int nPerSide;
    float gap;
    std::mt19937 rand;
};

#endif //ASSIGNMENT7_SAMPLER_H
