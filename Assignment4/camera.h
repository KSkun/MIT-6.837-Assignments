//
// Created by kskun on 2021/10/4.
//

#ifndef ASSIGNMENT1_CAMERA_H
#define ASSIGNMENT1_CAMERA_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ray.h"

class Camera {
public:
    virtual Ray generateRay(Vec2f point) = 0;

    [[nodiscard]] virtual float getTMin() const = 0;

    virtual void glInit(int w, int h) = 0;

    virtual void glPlaceCamera() = 0;

    virtual void dollyCamera(float dist) = 0;

    virtual void truckCamera(float dx, float dy) = 0;

    virtual void rotateCamera(float rx, float ry) = 0;
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

    void glInit(int w, int h) override;

    void glPlaceCamera() override;

    void dollyCamera(float dist) override;

    void truckCamera(float dx, float dy) override;

    void rotateCamera(float rx, float ry) override;

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

        this->angle = angle;
        screenDist = 1;
        side = screenDist * tan(angle / 2) * 2;
    }

    Ray generateRay(Vec2f point) override;

    [[nodiscard]] float getTMin() const override;

    void glInit(int w, int h) override;

    void glPlaceCamera() override;

    void dollyCamera(float dist) override;

    void truckCamera(float dx, float dy) override;

    void rotateCamera(float rx, float ry) override;

protected:
    Vec3f center, direction, up, horizontal;
    float angle, side, screenDist;
    // screenDist: distance from camera center to virtual screen center
    // side: side length of virtual screen
};

#endif //ASSIGNMENT1_CAMERA_H
