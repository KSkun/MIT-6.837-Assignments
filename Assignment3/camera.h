//
// Created by kskun on 2021/10/4.
//

#ifndef ASSIGNMENT1_CAMERA_H
#define ASSIGNMENT1_CAMERA_H

#include "ray.h"

class Camera {
public:
    virtual Ray generateRay(Vec2f point) = 0;

    [[nodiscard]] virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size) {
        this->center = center;

        this->direction = direction;
        this->direction.Normalize();

        Vec3f::Cross3(this->horizontal, direction, up);
        this->horizontal.Normalize();

        Vec3f::Cross3(this->up, this->horizontal, direction);
        this->up.Normalize();

        this->size = size;
    }

    Ray generateRay(Vec2f point) override;

    [[nodiscard]] float getTMin() const override;

protected:
    Vec3f center, up, horizontal, direction;
    float size;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle) {
        this->center = center;

        this->direction = direction;
        this->direction.Normalize();

        Vec3f::Cross3(this->horizontal, direction, up);
        this->horizontal.Normalize();

        Vec3f::Cross3(this->up, this->horizontal, direction);
        this->up.Normalize();

        screenDist = 1;
        side = screenDist * tan(angle / 2) * 2;
    }

    Ray generateRay(Vec2f point) override;

    [[nodiscard]] float getTMin() const override;

protected:
    Vec3f center, direction, up, horizontal, loPos, hiPos;
    float side, screenDist;
};

#endif //ASSIGNMENT1_CAMERA_H
