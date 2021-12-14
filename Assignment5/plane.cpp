//
// Created by kskun on 2021/10/11.
//

#include "plane.h"
#include "global.h"

bool Plane::intersect(const Ray &r, Hit &h, float tMin) {
    auto rdDotN = r.getDirection().Dot3(normal);
    auto roDotN = r.getOrigin().Dot3(normal);
    if (rdDotN == 0) { // Rd dot N == 0, Rd vertical to N
        if (fcmp(roDotN - d) == 0 && fcmp(tMin) <= 0) { // Ro dot N == 0 && 0 >= tMin, origin on plane
            h.set(0, material, normal, r);
            return true;
        }
        return false;
    }
    auto t = (d - roDotN) / rdDotN;
    if (fcmp(t - tMin) < 0) return false; // t < tMin, intersection behind camera
    auto realNormal = normal;
    if (rdDotN > 0) realNormal.Negate();
    h.set(t, material, realNormal, r);
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
    Vec3f oProjected = normal * d;

    // (-BIG, -BIG) to (BIG, BIG) in plane space
    Vec3f p1 = oProjected + BIG * b1 + BIG * b2,
            p2 = oProjected - BIG * b1 + BIG * b2,
            p3 = oProjected - BIG * b1 - BIG * b2,
            p4 = oProjected + BIG * b1 - BIG * b2;

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
