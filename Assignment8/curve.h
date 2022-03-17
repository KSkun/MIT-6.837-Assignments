//
// Created by KSkun on 2022/3/17.
//

#ifndef ASSIGNMENT8_CURVE_H
#define ASSIGNMENT8_CURVE_H


#include "spline.h"

class Curve : public Spline {
public:
    void Paint(ArgParser *args) override;

    virtual Vec2f evaluate(float t) = 0;
};

class BezierCurve : public Curve {
public:
    Vec2f evaluate(float t) override;
};

class BSplineCurve : public Curve {
public:
    Vec2f evaluate(float t) override;
};

#endif //ASSIGNMENT8_CURVE_H
