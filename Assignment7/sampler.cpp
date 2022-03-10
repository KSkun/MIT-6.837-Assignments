//
// Created by kskun on 2022/3/10.
//

#include "sampler.h"

Vec2f RandomSampler::getSamplePosition(int n) {
    assert(n >= 0 && n < numSamples);
    return {(float) rand() / rand.max(), (float) rand() / rand.max()};
}

Vec2f UniformSampler::getSamplePosition(int n) {
    auto x = n / nPerSide, y = n % nPerSide;
    return {(x + 0.5f) * gap, (y + 0.5f) * gap};
}

Vec2f JitteredSampler::getSamplePosition(int n) {
    auto x = n / nPerSide, y = n % nPerSide;
    Vec2f offset = {(float) rand() / rand.max() - 0.5f, (float) rand() / rand.max() - 0.5f};
    offset.Scale(gap, gap);
    Vec2f center = {(x + 0.5f) * gap, (y + 0.5f) * gap};
    center += offset;
    return center;
}