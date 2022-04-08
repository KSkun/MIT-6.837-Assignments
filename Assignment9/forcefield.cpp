//
// Created by KSkun on 2022/4/7.
//

#include "forcefield.h"

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    auto acc = force;
    acc.Scale(1.0f / mass, 1.0f / mass, 1.0f / mass);
    return acc;
}

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return gravity;
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    auto dir = position;
    dir.Normalize();
    dir.Scale(-magnitude / mass, -magnitude / mass, -magnitude / mass);
    return dir;
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    auto dir = Vec3f(0, 1, 0);
    if (fabs(position.y()) < 1e-6) dir.Scale(0, 0, 0); // add epsilon to avoid oscillation
    else if (position.y() > 0) dir.Scale(0, -1, 0);
    dir.Scale(0, magnitude, 0);
    return dir;
}
