//
// Created by kskun on 2022/3/20.
//

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