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

void Plane::paint() {
    // use big rectangle for plane
    // random vector v
    Vec3f v(1, 0, 0);
    // if normal parallels to v
    if (normal.y() == 0 && normal.z() == 0) {
        v.Set(0, 1, 0);
    }

    // plane space basis
    Vec3f b1, b2;
    Vec3f::Cross3(b1, v, normal);
    Vec3f::Cross3(b2, normal, b1);

    // (-BIG, -BIG) to (BIG, BIG) in plane space
    Vec3f p1 = BIG * b1 + BIG * b2,
            p2 = -BIG * b1 + BIG * b2,
            p3 = BIG * b1 - BIG * b2,
            p4 = -BIG * b1 - BIG * b2;

    material->glSetMaterial();
    glBegin(GL_QUADS);
    // set normal
    glNormal3f(normal.x(), normal.y(), normal.z());
    // set vertices
    glVertex3f(p1.x(), p1.y(), p1.z());
    glVertex3f(p2.x(), p2.y(), p2.z());
    glVertex3f(p3.x(), p3.y(), p3.z());
    glVertex3f(p4.x(), p4.y(), p4.z());
    glEnd();
}
