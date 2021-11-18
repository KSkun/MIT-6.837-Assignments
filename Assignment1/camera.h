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

protected:
    constexpr static const float EPSILON = 1e-8;
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

private:
    Vec3f center, up, horizontal, direction;
    float size;
};

#endif //ASSIGNMENT1_CAMERA_H
