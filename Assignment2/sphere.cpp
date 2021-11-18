//
// Created by kskun on 2021/10/4.
//

#include "sphere.h"
#include "global.h"

bool Sphere::intersect(const Ray &r, Hit &h, float tMin) {
    auto vecOriCen = center - r.getOrigin();
    auto disOriCen = vecOriCen.Length();
    if (fabs(disOriCen) < EPSILON) { // ray origin on sphere, origin is intersection
        if (0 < tMin - EPSILON) return false;
        auto n = vecOriCen;
        n.Negate();
        n.Normalize();
        h.set(0, material, n, r);
        return true;
    }

    auto disOriVer = vecOriCen.Dot3(r.getDirection());
    if (disOriVer < tMin - EPSILON) return false; // the sphere is behind the origin

    auto disVerSq = vecOriCen.Dot3(vecOriCen) - disOriVer * disOriVer;
    if (disVerSq > radius * radius + EPSILON) return false; // the ray misses the sphere

    auto disHitVer = sqrt(radius * radius - disVerSq);
    float t;
    if (disOriCen > radius - EPSILON &&
        disOriVer - disHitVer > tMin - EPSILON) { // ray origin outside the sphere
        t = disOriVer - disHitVer;
    } else { // ray origin inside the sphere
        t = disOriVer + disHitVer;
    }

    if (t < tMin - EPSILON) return false; // intersection behind the camera
    auto n = r.pointAtParameter(t) - center;
    n.Normalize();
    h.set(t, material, n, r);
    return true;
}