//
// Created by kskun on 2022/3/10.
//

#include "filter.h"
#include "global.h"

Vec3f Filter::getColor(int i, int j, Film *film) {
    auto r = getSupportRadius();
    Vec3f color = {0, 0, 0};
    float weightSum = 0;
    for (int offX = -r; offX <= r; offX++) {
        if (i + offX < 0 || i + offX >= film->getWidth()) continue;
        for (int offY = -r; offY <= r; offY++) {
            if (j + offY < 0 || j + offY >= film->getHeight()) continue;
            for (int k = 0; k < film->getNumSamples(); k++) {
                auto sample = film->getSample(i + offX, j + offY, k);
                auto pos = sample.getPosition();
                pos -= Vec2f(0.5f, 0.5f);
                pos += Vec2f(offX, offY);
                auto w = getWeight(pos.x(), pos.y());
                weightSum += w;
                color += w * sample.getColor();
            }
        }
    }
    color.Scale(1 / weightSum, 1 / weightSum, 1 / weightSum);
    return color;
}

float BoxFilter::getWeight(float x, float y) {
    auto dist = std::max(abs(x), abs(y));
    if (dist - EPSILON > radius) return 0;
    return 1;
}

float TentFilter::getWeight(float x, float y) {
    auto dist = sqrt(x * x + y * y);
    if (dist - EPSILON > radius) return 0;
    return (radius - dist) / radius;
}

float GaussianFilter::getWeight(float x, float y) {
    auto dist = sqrt(x * x + y * y);
//    if (dist - EPSILON > sigma * 2) return 0;
    return exp(-dist * dist / (2 * sigma * sigma));
}
