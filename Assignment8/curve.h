//
// Created by KSkun on 2022/3/17.
//

#ifndef ASSIGNMENT8_CURVE_H
#define ASSIGNMENT8_CURVE_H


#include "spline.h"

class Curve : public Spline {
public:
    explicit Curve(int numVertices) : Spline(numVertices) {}

    void Paint(ArgParser *args) override;

    virtual Vec2f evaluate(float t) = 0;
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(int numVertices) : Curve(numVertices) {}

    Vec2f evaluate(float t) override;

    void OutputBezier(FILE *file) override {}

    void OutputBSpline(FILE *file) override {}
};

class BSplineCurve : public Curve {
public:
    explicit BSplineCurve(int numVertices) : Curve(numVertices) {}

    Vec2f evaluate(float t) override;

    void OutputBezier(FILE *file) override {}

    void OutputBSpline(FILE *file) override {}
};

#endif //ASSIGNMENT8_CURVE_H
