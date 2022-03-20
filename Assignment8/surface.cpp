//
// Created by kskun on 2022/3/20.
//

#include <windows.h>
#include <GL/gl.h>

#include "surface.h"

TriangleMesh *SurfaceOfRevolution::OutputTriangles(ArgParser *args) {
    float angle = 2.0f * M_PI / args->revolution_tessellation;
    Matrix matRotate = Matrix::MakeYRotation(angle);
    auto points = c->tessellate(args);
    auto *mesh = new TriangleNet(points.size() - 1, args->revolution_tessellation);
    for (auto i = 0; i <= args->revolution_tessellation; i++) {
        for (int j = 0; j < points.size(); j++) {
            mesh->SetVertex(j, i, points[j]);
            matRotate.Transform(points[j]);
        }
    }
    return mesh;
}

void BezierPatch::Paint(ArgParser *args) {
    // draw control polygon
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 1);
    for (const auto &p: vertices) {
        glVertex2f(p.x(), p.y());
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

TriangleMesh *BezierPatch::OutputTriangles(ArgParser *args) {
    std::vector<Vec3f> group[4];
    for (int i = 0; i < 4; i++) {
        std::copy(vertices.begin() + i * 4, vertices.begin() + (i + 1) * 4,
                  std::back_inserter(group[i]));
    }
    BezierCurve *curve[4] = {
            new BezierCurve(group[0]), new BezierCurve(group[1]),
            new BezierCurve(group[2]), new BezierCurve(group[3])
    };

    auto *mesh = new TriangleNet(args->patch_tessellation, args->curve_tessellation);
    float s, t = 0.0f,
            stepS = 1.0f / args->patch_tessellation,
            stepT = 1.0f / args->curve_tessellation;
    std::vector<Vec3f> tempVertices;
    tempVertices.reserve(4);
    for (auto i = 0; i <= args->curve_tessellation; i++) {
        tempVertices.clear();
        for (auto &c: curve) {
            tempVertices.push_back(c->evaluate(0, 4, t));
        }
        BezierCurve tempCurve(tempVertices);
        s = 0.0f;
        for (auto j = 0; j <= args->patch_tessellation; j++) {
            mesh->SetVertex(j, i, tempCurve.evaluate(0, 4, s));
            s += stepS;
        }
        t += stepT;
    }
    for (auto &c: curve) {
        delete c;
    }
    return mesh;
}