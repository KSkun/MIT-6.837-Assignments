//
// Created by kskun on 2022/4/8.
//

#ifndef ASSIGNMENT9_GENERATOR_H
#define ASSIGNMENT9_GENERATOR_H

#include <windows.h>
#include <GL/gl.h>

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
    void SetColors(const Vec3f &color, const Vec3f &dead_color, float color_randomness) {
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
    virtual int numNewParticles(float current_time, float dt) const {
        return ceil(dt * desiredNumParticles / lifespan);
    }

    virtual Particle *Generate(float current_time, float dt, int i) = 0;

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

class HoseGenerator : public Generator {
public:
    HoseGenerator(const Vec3f &position, float position_randomness,
                  const Vec3f &velocity, float velocity_randomness) {
        this->position = position;
        this->positionRandomness = position_randomness;
        this->velocity = velocity;
        this->velocityRandomness = velocity_randomness;
    }

    Particle *Generate(float current_time, float dt, int i) override;

protected:
    Vec3f position;
    float positionRandomness;
    Vec3f velocity;
    float velocityRandomness;
};

class RingGenerator : public Generator {
public:
    RingGenerator(float position_randomness, const Vec3f &velocity, float velocity_randomness) {
        this->positionRandomness = position_randomness;
        this->velocity = velocity;
        this->velocityRandomness = velocity_randomness;
    }

    int numNewParticles(float current_time, float dt) const override {
        return ceil(dt * desiredNumParticles / lifespan * current_time);
    }

    Particle *Generate(float current_time, float dt, int i) override;

    void Paint() const override {
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glNormal3f(0, 0, -1);
        glVertex3f(-100.0f, -1.0f, -100.0f);
        glVertex3f(-100.0f, -1.0f, 100.0f);
        glVertex3f(100.0f, -1.0f, 100.0f);
        glVertex3f(100.0f, -1.0f, -100.0f);
        glEnd();
    }

protected:
    float positionRandomness;
    Vec3f velocity;
    float velocityRandomness;
};

#endif //ASSIGNMENT9_GENERATOR_H
