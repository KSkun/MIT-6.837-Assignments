//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_TRIANGLE_H
#define ASSIGNMENT2_TRIANGLE_H

#include <array>
#include <algorithm>

#include "object3d.h"

class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : a(a), b(b), c(c), Object3D(m) {
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();

        // calculate bounding box
        auto arrX = {a.x(), b.x(), c.x()},
                arrY = {a.y(), b.y(), c.y()},
                arrZ = {a.z(), b.z(), c.z()};
        auto minmaxX = std::minmax(arrX),
                minmaxY = std::minmax(arrY),
                minmaxZ = std::minmax(arrZ);
        bbox = new BoundingBox(Vec3f(minmaxX.first, minmaxY.first, minmaxZ.first),
                               Vec3f(minmaxX.second, minmaxY.second, minmaxZ.second));
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

protected:
    Vec3f a, b, c, normal;
};


#endif //ASSIGNMENT2_TRIANGLE_H
