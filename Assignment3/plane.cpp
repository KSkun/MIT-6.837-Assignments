//
// Created by kskun on 2021/10/11.
//

#include "plane.h"
#include "global.h"

bool Plane::intersect(const Ray &r, Hit &h, float tMin) {
    auto rdDotN = r.getDirection().Dot3(normal);
    auto roDotN = r.getOrigin().Dot3(normal);
    if (fabs(rdDotN) < EPSILON) {
        if (fabs(roDotN) < EPSILON && 0 > tMin + EPSILON) {
            h.set(0, material, normal, r);
            return true;
        }
        return false;
    }
    auto t = (d - roDotN) / rdDotN;
    if (t < tMin - EPSILON) return false;
    h.set(t, material, normal, r);
    return true;
}