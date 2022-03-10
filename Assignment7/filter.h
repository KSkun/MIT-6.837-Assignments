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

protected:
    virtual float getWeight(float x, float y) = 0;

    virtual int getSupportRadius() = 0;
};

class BoxFilter : public Filter {
public:
    explicit BoxFilter(float radius) : radius(radius) {}

protected:
    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return floor(radius);
    }

    float radius;
};

class TentFilter : public Filter {
public:
    explicit TentFilter(float radius) : radius(radius) {}

protected:
    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return floor(radius);
    }

    float radius;
};

class GaussianFilter : public Filter {
public:
    explicit GaussianFilter(float sigma) : sigma(sigma) {}

protected:
    float getWeight(float x, float y) override;

    int getSupportRadius() override {
        return floor(2 * sigma);
    }

    float sigma;
};


#endif //ASSIGNMENT7_FILTER_H
