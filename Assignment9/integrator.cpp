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

// 4-order runge kutta method
// ref: https://blog.csdn.net/u013007900/article/details/45922331/
void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    auto m = particle->getMass();
    auto pn = particle->getPosition(), vn = particle->getVelocity();

    auto kv1 = dt * forcefield->getAcceleration(pn, m, t);
    auto kp1 = dt * vn;
    auto kv2 = dt * forcefield->getAcceleration(pn + kp1 * 0.5f, m, t + dt * 0.5f);
    auto kp2 = dt * (vn + kv1 * 0.5f);
    auto kv3 = dt * forcefield->getAcceleration(pn + kp2 * 0.5f, m, t + dt * 0.5f);
    auto kp3 = dt * (vn + kv2 * 0.5f);
    auto kv4 = dt * forcefield->getAcceleration(pn + kp3, m, t + dt);
    auto kp4 = dt * (vn + kv3);

    particle->setPosition(pn + 1 / 6.0f * (kp1 + kp2 * 2.0f + kp3 * 2.0f + kp4));
    particle->setVelocity(vn + 1 / 6.0f * (kv1 + kv2 * 2.0f + kv3 * 2.0f + kv4));
    particle->increaseAge(dt);
}