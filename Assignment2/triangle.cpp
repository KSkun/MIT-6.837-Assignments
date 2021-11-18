//
// Created by kskun on 2021/10/11.
//

#include "triangle.h"
#include "matrix.h"
#include "global.h"

bool Triangle::intersect(const Ray &r, Hit &h, float tMin) {
    auto detA = det3x3(
            a.x() - b.x(), a.x() - c.x(), r.getDirection().x(),
            a.y() - b.y(), a.y() - c.y(), r.getDirection().y(),
            a.z() - b.z(), a.z() - c.z(), r.getDirection().z()
    );
    if (fabs(detA) < EPSILON) {
        return false;
    }

    auto detA1 = det3x3(
            a.x() - r.getOrigin().x(), a.x() - c.x(), r.getDirection().x(),
            a.y() - r.getOrigin().y(), a.y() - c.y(), r.getDirection().y(),
            a.z() - r.getOrigin().z(), a.z() - c.z(), r.getDirection().z()
    );
    auto detA2 = det3x3(
            a.x() - b.x(), a.x() - r.getOrigin().x(), r.getDirection().x(),
            a.y() - b.y(), a.y() - r.getOrigin().y(), r.getDirection().y(),
            a.z() - b.z(), a.z() - r.getOrigin().z(), r.getDirection().z()
    );
    auto detA3 = det3x3(
            a.x() - b.x(), a.x() - c.x(), a.x() - r.getOrigin().x(),
            a.y() - b.y(), a.y() - c.y(), a.y() - r.getOrigin().y(),
            a.z() - b.z(), a.z() - c.z(), a.z() - r.getOrigin().z()
    );

    auto beta = detA1 / detA;
    auto gamma = detA2 / detA;
    auto alpha = 1 - beta - gamma;
    if (alpha < -EPSILON || alpha > 1 + EPSILON) return false;
    if (beta < -EPSILON || beta > 1 + EPSILON) return false;
    if (gamma < -EPSILON || gamma > 1 + EPSILON) return false;
    auto t = detA3 / detA;
    if (t < tMin - EPSILON) return false;
    Vec3f normal;
    Vec3f::Cross3(normal, b - a, c - a);
    normal.Normalize();
//    auto nDotRd = normal.Dot3(r.getDirection());
//    if (nDotRd > -EPSILON) {
//        normal.Scale(-1, -1, -1);
//    }
    h.set(t, material, normal, r);
    return true;
}