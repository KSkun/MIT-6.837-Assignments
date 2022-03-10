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
    Sampler(int numSamples) : numSamples(numSamples) {}

    virtual Vec2f getSamplePosition(int n) = 0;

protected:
    int numSamples;
};

class RandomSampler : public Sampler {
public:
    RandomSampler(int numSamples) : Sampler(numSamples) {
        // timestamp as random seed
        rand.seed(time(nullptr));
    }

    Vec2f getSamplePosition(int n) override;

protected:
    std::mt19937 rand;
};


#endif //ASSIGNMENT7_SAMPLER_H
