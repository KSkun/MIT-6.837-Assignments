//
// Created by KSkun on 2022/3/17.
//

#ifndef ASSIGNMENT8_CURVE_H
#define ASSIGNMENT8_CURVE_H


#include "spline.h"
#include "matrix.h"

class Curve : public Spline {
public:
    explicit Curve(int numVertices) : Spline(numVertices) {}

    void Paint(ArgParser *args) override;

    virtual void drawCurveLine(ArgParser *args) = 0;

protected:
    Matrix matConvert;
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(int numVertices) : Curve(numVertices) {
        float arrBezier[] = {
                -1, 3, -3, 1,
                3, -6, 3, 0,
                -3, 3, 0, 0,
                1, 0, 0, 0
        };
        float arrBSpline[] = {
                -1, 3, -3, 1,
                3, -6, 0, 4,
                -3, 3, 3, 1,
                1, 0, 0, 0
        };
        Matrix matBezier(arrBezier);
        Matrix matBSpline(arrBSpline);
        matBSpline *= 1.0f / 6;
        matBSpline.Inverse();
        matConvert = matBezier * matBSpline;
    }

    Vec2f evaluate(int index, int num, float t);

    void drawCurveLine(ArgParser *args) override;

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;
};

class BSplineCurve : public Curve {
public:
    explicit BSplineCurve(int numVertices) : Curve(numVertices) {
        float arrBezier[] = {
                -1, 3, -3, 1,
                3, -6, 3, 0,
                -3, 3, 0, 0,
                1, 0, 0, 0
        };
        float arrBSpline[] = {
                -1, 3, -3, 1,
                3, -6, 0, 4,
                -3, 3, 3, 1,
                1, 0, 0, 0
        };
        Matrix matBezier(arrBezier);
        Matrix matBSpline(arrBSpline);
        matBSpline *= 1.0f / 6;
        matBezier.Inverse();
        matConvert = matBSpline * matBezier;
    }

    Vec2f evaluate(int index, float t);

    void drawCurveLine(ArgParser *args) override;

    void OutputBezier(FILE *file) override;

    void OutputBSpline(FILE *file) override;
};

#endif //ASSIGNMENT8_CURVE_H
