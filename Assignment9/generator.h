//
// Created by kskun on 2022/4/8.
//

#ifndef ASSIGNMENT9_GENERATOR_H
#define ASSIGNMENT9_GENERATOR_H

#include <ctime>

#include <random>

#include "vectors.h"
#include "particle.h"

class Generator {
public:
    Generator() {
        timeSeed = time(nullptr);
        rand.seed(timeSeed);
    }

    // initialization
    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {
        this->color = color;
        this->deadColor = dead_color;
        this->colorRandomness = color_randomness;
    }

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {
        this->lifespan = lifespan;
        this->lifespanRandomness = lifespan_randomness;
        this->desiredNumParticles = desired_num_particles;
    }

    void SetMass(float mass, float mass_randomness) {
        this->mass = mass;
        this->massRandomness = mass_randomness;
    }

    // on each timestep, create some particles
    int numNewParticles(float current_time, float dt) const {
        return dt * desiredNumParticles / lifespan;
    }

    virtual Particle *Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const {}

    void Restart() {
        rand.seed(timeSeed);
    }

protected:
    std::mt19937 rand;
    time_t timeSeed;

    Vec3f color, deadColor;
    float colorRandomness;
    float lifespan, lifespanRandomness;
    int desiredNumParticles;
    float mass, massRandomness;
};


#endif //ASSIGNMENT9_GENERATOR_H
