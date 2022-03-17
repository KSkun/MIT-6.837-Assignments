//
// Created by KSkun on 2022/3/17.
//

#include <windows.h>
#include <GL/gl.h>

#include "curve.h"

void Curve::Paint(ArgParser *args) {
    // draw control polygon
    glBegin(GL_LINE_STRIP);
    glLineWidth(1);
    glColor3f(0, 0, 1);
    for (const auto &p: vertices) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();

    // draw curve line
    glBegin(GL_LINE_STRIP);
    glLineWidth(2);
    glColor3f(0, 1, 0);
    float step = 1.0f / args->curve_tessellation, t = step / 2.0f;
    for (int i = 0; i < args->curve_tessellation; i++) {
        auto p = evaluate(t);
        t += step;
        glVertex2f(p.x(), p.y());
    }
    glEnd();

    // draw control points
    glBegin(GL_POINTS);
    glPointSize(3);
    glColor3f(1, 0, 0);
    for (const auto &p: vertices) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
}

Vec2f BezierCurve::evaluate(float t) {
    assert(getNumVertices() == 4);
    float tm = 1 - t;
    auto p = tm * tm * tm * vertices[0] + 3 * t * tm * tm * vertices[1] +
             3 * t * t * tm * vertices[2] + t * t * t * vertices[3];
    return {p.x(), p.y()};
}

Vec2f BSplineCurve::evaluate(float t) {
    assert(getNumVertices() == 4);
    float b0 = (1 - t) * (1 - t) * (1 - t) / 6,
            b1 = (3 * t * t * t - 6 * t * t + 4) / 6,
            b2 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6,
            b3 = t * t * t / 6;
    auto p = b0 * vertices[0] + b1 * vertices[1] + b2 * vertices[2] + b3 * vertices[3];
    return {p.x(), p.y()};
}