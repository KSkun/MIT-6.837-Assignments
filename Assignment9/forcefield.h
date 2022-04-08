//
// Created by KSkun on 2022/4/7.
//

#ifndef ASSIGNMENT9_FORCEFIELD_H
#define ASSIGNMENT9_FORCEFIELD_H


#include "vectors.h"

class ForceField {
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class ConstantForceField : public ForceField {
public:
    explicit ConstantForceField(const Vec3f &force) : force(force) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f force;
};

class GravityForceField : public ForceField {
public:
    explicit GravityForceField(const Vec3f &gravity) : gravity(gravity) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f gravity;
};

class RadialForceField : public ForceField {
public:
    explicit RadialForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

class VerticalForceField : public ForceField {
public:
    explicit VerticalForceField(float magnitude) : magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float magnitude;
};

#endif //ASSIGNMENT9_FORCEFIELD_H
