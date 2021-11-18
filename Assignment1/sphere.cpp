//
// Created by kskun on 2021/10/4.
//

#include "sphere.h"

bool Sphere::intersect(const Ray &r, Hit &h, float tMin) {
    auto vecOriCen = center - r.getOrigin();
    auto disOriCen = vecOriCen.Dot3(vecOriCen);
    if (fabs(disOriCen) < EPSILON) { // ray origin on sphere, origin is intersection
        if (0 < tMin - EPSILON) return false;
        h.set(0, material, r);
        return true;
    }

    auto disOriVer = vecOriCen.Dot3(r.getDirection());
    //if (disOriVer < -EPSILON) return false; // the sphere is behind the origin

    auto disVer = vecOriCen.Dot3(vecOriCen) - disOriVer * disOriVer;
    if (disVer > radius * radius + EPSILON) return false; // the ray misses the sphere

    auto disHitVer = radius * radius - disVer;
    float t;
    //if (disOriCen > radius - EPSILON) { // ray origin outside the sphere
        t = disOriVer - disHitVer;
    //} else { // ray origin inside the sphere
    //    t = disOriVer + disHitVer;
    //}

    if (t < tMin - EPSILON) return false; // intersection behind the camera
    h.set(t, material, r);
    return true;
}