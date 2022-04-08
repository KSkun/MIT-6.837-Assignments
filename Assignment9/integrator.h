//
// Created by kskun on 2022/4/8.
//

#ifndef ASSIGNMENT9_INTEGRATOR_H
#define ASSIGNMENT9_INTEGRATOR_H


#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;

    virtual Vec3f getColor() = 0;
};

class EulerIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;

    Vec3f getColor() override {
        return {1, 0, 0};
    }
};

class MidpointIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;


    Vec3f getColor() override {
        return {0, 1, 0};
    }
};

class RungeKuttaIntegrator : public Integrator {
public:
    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;


    Vec3f getColor() override {
        return {0, 0, 1};
    }
};


#endif //ASSIGNMENT9_INTEGRATOR_H
