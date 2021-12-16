//
// Created by kskun on 2021/10/4.
//

#ifndef ASSIGNMENT1_SPHERE_H
#define ASSIGNMENT1_SPHERE_H

#include "object3d.h"
#include "grid.h"

class Sphere : public Object3D {
public:
    Sphere(const Vec3f &center, float radius, Material *material) :
            center(center), radius(radius), Object3D(material) {
        // calculate bounding box
        auto vecR = Vec3f(radius, radius, radius);
        bbox = new BoundingBox(center - vecR, center + vecR);
    }

    Sphere() : Sphere(Vec3f(), 0, nullptr) {}

    [[nodiscard]] Vec3f getCenter() const { return center; }

    [[nodiscard]] float getRadius() const { return radius; }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

protected:
    Vec3f getPoint(float phi, float theta);

    Vec3f center;
    float radius;
};

#endif //ASSIGNMENT1_SPHERE_H
