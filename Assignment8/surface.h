//
// Created by kskun on 2022/3/20.
//

#ifndef ASSIGNMENT8_SURFACE_H
#define ASSIGNMENT8_SURFACE_H


#include "spline.h"
#include "curve.h"

class Surface : public Spline {
public:
    explicit Surface(int numVertices) : Spline(numVertices) {}
};

class SurfaceOfRevolution : public Surface {
public:
    explicit SurfaceOfRevolution(Curve *c) : c(c), Surface(0) {}

    void Paint(ArgParser *args) override {
        c->Paint(args);
    }

    // FOR CONTROL POINT PICKING
    int getNumVertices() override {
        return c->getNumVertices();
    }

    Vec3f getVertex(int i) override {
        return c->getVertex(i);
    }

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y) override {
        c->moveControlPoint(selectedPoint, x, y);
    }

    void addControlPoint(int selectedPoint, float x, float y) override {
        c->addControlPoint(selectedPoint, x, y);
    }

    void deleteControlPoint(int selectedPoint) override {
        c->deleteControlPoint(selectedPoint);
    }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file) override {
        fprintf(file, "surface_of_revolution\n");
        c->OutputBezier(file);
    }

    void OutputBSpline(FILE *file) override {
        fprintf(file, "surface_of_revolution\n");
        c->OutputBSpline(file);
    }

    TriangleMesh *OutputTriangles(ArgParser *args) override;

protected:
    Curve *c;
};

class BezierPatch : public Surface {
public:
    BezierPatch() : Surface(16) {}

    void Paint(ArgParser *args) override;

    TriangleMesh *OutputTriangles(ArgParser *args) override;
};

#endif //ASSIGNMENT8_SURFACE_H
