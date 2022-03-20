//
// Created by KSkun on 2022/3/17.
//

#include <windows.h>
#include <GL/gl.h>

#include "curve.h"
#include "matrix.h"

void Curve::Paint(ArgParser *args) {
    // draw control polygon
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 1);
    for (const auto &p: vertices) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();

    // draw curve line
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 0);
    auto points = tessellate(args);
    for (const auto &p : points) {
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();

    // draw control points
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    for (const auto &p: vertices) {
        glVertex2f(p.x(), p.y());
    }
    glEnd();
}

//Vec2f BezierCurve::evaluate(float t) {
//    assert(getNumVertices() == 4);
//    float tm = 1 - t;
//    auto p = tm * tm * tm * vertices[0] + 3 * t * tm * tm * vertices[1] +
//             3 * t * t * tm * vertices[2] + t * t * t * vertices[3];
//    return {p.x(), p.y()};
//}

std::vector<Vec3f> BezierCurve::tessellate(ArgParser *args) {
    std::vector<Vec3f> results;
    for (int i = 0; i < vertices.size(); i += 3) {
        if (i == vertices.size() - 1) break;
        float step = 1.0f / args->curve_tessellation, t = 0;
        for (int j = 0; j <= args->curve_tessellation; j++) {
            auto p = evaluate(i, min((int) vertices.size() - i, 4), t);
            t += step;
            results.push_back(p);
        }
    }
    return results;
}

Vec3f BezierCurve::evaluate(int index, int num, float t) {
    std::vector<Vec3f> v1, v2;
    v1.reserve(vertices.size());
    for (int i = index; i < index + num; i++) {
        v1.push_back(vertices[i]);
    }
    while (v1.size() > 1) {
        v2.clear();
        v2.reserve(v1.size() - 1);
        for (int j = 0; j < v1.size() - 1; j++) {
            auto p1 = v1[j], p2 = v1[j + 1];
            p1.Scale(1 - t, 1 - t, 1 - t);
            p2.Scale(t, t, t);
            p1 += p2;
            v2.push_back(p1);
        }
        v1 = v2;
    }
    return v1[0];
}

void BezierCurve::OutputBezier(FILE *file) {
    fprintf(file, "bezier\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (const auto &p: vertices) {
        fprintf(file, "%f %f %f\n", p.x(), p.y(), p.z());
    }
}

void BezierCurve::OutputBSpline(FILE *file) {
    assert(getNumVertices() == 4);
    float vertexArr[] = {
            vertices[0].x(), vertices[1].x(), vertices[2].x(), vertices[3].x(),
            vertices[0].y(), vertices[1].y(), vertices[2].y(), vertices[3].y(),
            vertices[0].z(), vertices[1].z(), vertices[2].z(), vertices[3].z(),
            0, 0, 0, 0
    };
    Matrix vertexMat(vertexArr);
    vertexMat *= matConvert;

    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (int i = 0; i < 4; i++) {
        fprintf(file, "%f %f %f\n",
                vertexMat.Get(i, 0), vertexMat.Get(i, 1), vertexMat.Get(i, 2));
    }
}

std::vector<Vec3f> BSplineCurve::tessellate(ArgParser *args) {
    assert(getNumVertices() >= 4);
    std::vector<Vec3f> results;
    for (int i = 3; i < vertices.size(); i++) {
        float step = 1.0f / args->curve_tessellation, t = 0;
        for (int j = 0; j <= args->curve_tessellation; j++) {
            auto p = evaluate(i, t);
            t += step;
            results.push_back(p);
        }
    }
    return results;
}

Vec3f BSplineCurve::evaluate(int index, float t) {
    assert(index >= 3 && index < getNumVertices());
    float b0 = (1 - t) * (1 - t) * (1 - t) / 6,
            b1 = (3 * t * t * t - 6 * t * t + 4) / 6,
            b2 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6,
            b3 = t * t * t / 6;
    auto p = b0 * vertices[index - 3] + b1 * vertices[index - 2] +
             b2 * vertices[index - 1] + b3 * vertices[index];
    return p;
}

void BSplineCurve::OutputBSpline(FILE *file) {
    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (const auto &p: vertices) {
        fprintf(file, "%f %f %f\n", p.x(), p.y(), p.z());
    }
}

void BSplineCurve::OutputBezier(FILE *file) {
    assert(getNumVertices() == 4);
    float vertexArr[] = {
            vertices[0].x(), vertices[1].x(), vertices[2].x(), vertices[3].x(),
            vertices[0].y(), vertices[1].y(), vertices[2].y(), vertices[3].y(),
            vertices[0].z(), vertices[1].z(), vertices[2].z(), vertices[3].z(),
            0, 0, 0, 0
    };
    Matrix vertexMat(vertexArr);
    vertexMat *= matConvert;

    fprintf(file, "bezier\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (int i = 0; i < 4; i++) {
        fprintf(file, "%f %f %f\n",
                vertexMat.Get(i, 0), vertexMat.Get(i, 1), vertexMat.Get(i, 2));
    }
}