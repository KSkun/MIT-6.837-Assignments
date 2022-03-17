//
// Created by KSkun on 2022/3/17.
//

#ifndef ASSIGNMENT8_SPLINE_H
#define ASSIGNMENT8_SPLINE_H

#include <vector>

#include "arg_parser.h"
#include "triangle_mesh.h"

class Spline {
public:
    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) = 0;

    virtual void OutputBSpline(FILE *file) = 0;

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() {
        return vertices.size();
    }

    virtual Vec3f getVertex(int i) {
        return vertices[i];
    }

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) {
        vertices[selectedPoint] = {x, y, 0};
    }

    virtual void addControlPoint(int selectedPoint, float x, float y) {
        vertices.insert(vertices.begin() + selectedPoint, {x, y, 0});
    }

    virtual void deleteControlPoint(int selectedPoint) {
        vertices.erase(vertices.begin() + selectedPoint);
    }

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh *OutputTriangles(ArgParser *args) {}

protected:
    std::vector<Vec3f> vertices;
};


#endif //ASSIGNMENT8_SPLINE_H
