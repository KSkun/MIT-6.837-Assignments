//
// Created by kskun on 2022/4/8.
//

#include "integrator.h"

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    auto pn = particle->getPosition(), vn = particle->getVelocity();
    auto a = forcefield->getAcceleration(pn, particle->getMass(), t);

    particle->setPosition(pn + vn * dt);
    particle->setVelocity(vn + a * dt);
    particle->increaseAge(dt);
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    auto pn = particle->getPosition(), vn = particle->getVelocity();
    auto a = forcefield->getAcceleration(pn, particle->getMass(), t);
    auto pm = pn + vn * (dt / 2.0f), vm = vn + a * (dt / 2.0f);
    auto am = forcefield->getAcceleration(pn, particle->getMass(), t + (dt / 2.0f));

    particle->setPosition(pn + vm * dt);
    particle->setVelocity(vn + am * dt);
    particle->increaseAge(dt);
}