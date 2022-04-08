//
// Created by kskun on 2022/4/8.
//

#include "generator.h"

float randomNormalizedFloat(std::mt19937 &rand) {
    return (float) (rand() - rand.min()) / (rand.max() - rand.min());
}

float randomFloat(std::mt19937 &rand, float value, float randomness) {
    return value + (randomNormalizedFloat(rand) - 0.5f) * 2.0f * randomness;
}

Vec3f randomVec3f(std::mt19937 &rand, const Vec3f &value, float randomness) {
    auto r1 = (randomNormalizedFloat(rand) - 0.5f) * 2.0f,
            r2 = (randomNormalizedFloat(rand) - 0.5f) * 2.0f,
            r3 = (randomNormalizedFloat(rand) - 0.5f) * 2.0f;
    return value + Vec3f(r1, r2, r3) * randomness;
}

Particle *HoseGenerator::Generate(float current_time, float dt, int i) {
    auto c = randomVec3f(rand, color, colorRandomness);
    c.Clamp();
    auto dc = randomVec3f(rand, deadColor, colorRandomness);
    dc.Clamp();
    auto l = randomFloat(rand, lifespan, lifespanRandomness);
    auto m = randomFloat(rand, mass, massRandomness);
    auto p = randomVec3f(rand, position, positionRandomness);
    auto v = randomVec3f(rand, velocity, velocityRandomness);
    return new Particle(p, v, c, dc, m, l);
}

Particle *RingGenerator::Generate(float current_time, float dt, int i) {
    auto c = randomVec3f(rand, color, colorRandomness);
    c.Clamp();
    auto dc = randomVec3f(rand, deadColor, colorRandomness);
    dc.Clamp();
    auto l = randomFloat(rand, lifespan, lifespanRandomness);
    auto m = randomFloat(rand, mass, massRandomness);
    auto v = randomVec3f(rand, velocity, velocityRandomness);

    auto radius = current_time;
    auto numP = numNewParticles(current_time, dt);
    float angle = (float) i / numP * M_PI * 2.0f;
    auto p = randomVec3f(rand, {radius * cos(angle), 0, radius * sin(angle)}, positionRandomness);

    return new Particle(p, v, c, dc, m, l);
}
