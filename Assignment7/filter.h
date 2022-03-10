//
// Created by kskun on 2022/3/10.
//

#ifndef ASSIGNMENT7_FILTER_H
#define ASSIGNMENT7_FILTER_H


#include "vectors.h"
#include "film.h"

class Filter {
public:
    Vec3f getColor(int i, int j, Film *film);

    virtual ~Filter() = default;

    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {
public:
    explicit BoxFilter(float radius) : radius(radius) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return ceil(radius - 0.5f);
    }

    float radius;
};

class TentFilter : public Filter {
public:
    explicit TentFilter(float radius) : radius(radius) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return ceil(radius - 0.5f);
    }

    float radius;
};

class GaussianFilter : public Filter {
public:
    explicit GaussianFilter(float sigma) : sigma(sigma) {}

    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return ceil(2 * sigma - 0.5f);
    }

    float sigma;
};


#endif //ASSIGNMENT7_FILTER_H
