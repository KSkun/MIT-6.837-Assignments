//
// Created by kskun on 2022/3/10.
//

#include "sampler.h"

Vec2f RandomSampler::getSamplePosition(int n) {
    assert(n >= 0 && n < numSamples);
    return {(float) rand() / rand.max(), (float) rand() / rand.max()};
}