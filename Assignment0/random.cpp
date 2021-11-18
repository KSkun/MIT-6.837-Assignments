//
// Created by kskun on 2021/10/3.
//

#include "random.h"

Random::Random(int seed) {
    mt = new std::mt19937(seed);
}

Random::Random(): Random(time(nullptr)) {}

Random::~Random() {
    delete mt;
}

float Random::randFloatUniform() {
    return (float) ((*mt)() - std::mt19937::min()) /
        (std::mt19937::max() - std::mt19937::min());
}

int Random::randInt(int min, int max) {
    return ((*mt)() - std::mt19937::min()) % (max - min + 1) + min;
}