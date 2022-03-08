//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_PLANE_H
#define ASSIGNMENT2_PLANE_H

#include "object3d.h"

class Plane : public Object3D {
public:
    Plane(Vec3f &normal, float d, Material *m) : normal(normal), d(d), Object3D(m) {}

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

protected:
    constexpr static const float BIG = 1e5; // used in GL draw

    Vec3f normal;
    float d;
};


#endif //ASSIGNMENT2_PLANE_H
