//
// Created by kskun on 2021/10/4.
//

#include <cmath>

#include "camera.h"

Ray OrthographicCamera::generateRay(Vec2f point) {
    point -= Vec2f(0.5, 0.5);
    point *= size;
    return {center + point.x() * horizontal + point.y() * up, direction};
}

float OrthographicCamera::getTMin() const {
    return -FP_INFINITE; // negative infinity
}