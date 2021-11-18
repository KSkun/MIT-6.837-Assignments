//
// Created by kskun on 2021/10/3.
//

#ifndef ASSIGNMENT0_RANDOM_H
#define ASSIGNMENT0_RANDOM_H

#include <random>
#include <ctime>

class Random {
public:
    explicit Random(int seed);

    Random();

    ~Random();

    float randFloatUniform();

    int randInt(int min, int max);
private:
    std::mt19937 *mt;
};


#endif //ASSIGNMENT0_RANDOM_H
