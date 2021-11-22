//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_TRIANGLE_H
#define ASSIGNMENT2_TRIANGLE_H


#include "object3d.h"

class Triangle : public Object3D {
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m) : a(a), b(b), c(c), Object3D(m) {
        Vec3f::Cross3(normal, b - a, c - a);
        normal.Normalize();
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

protected:
    Vec3f a, b, c, normal;
};


#endif //ASSIGNMENT2_TRIANGLE_H
