//
// Created by kskun on 2021/10/4.
//

#define _USE_MATH_DEFINES

#include <math.h>

#include "sphere.h"
#include "global.h"

bool Sphere::intersect(const Ray &r, Hit &h, float tMin) {
    auto vecOriCen = center - r.getOrigin();
    auto disOriCen = vecOriCen.Length();
    if (fcmp(disOriCen) == 0) { // ray origin on sphere, origin is intersection
        if (fcmp(tMin) >= 0) return false;
        auto n = vecOriCen;
        n.Negate();
        n.Normalize();
        h.set(0, material, n, r);
        return true;
    }

    auto disOriVer = vecOriCen.Dot3(r.getDirection());
    if (fcmp(disOriVer - tMin) < 0) return false; // the sphere is behind the origin

    auto disVerSq = vecOriCen.Dot3(vecOriCen) - disOriVer * disOriVer;
    if (fcmp(disVerSq - radius * radius) > 0) return false; // the ray misses the sphere

    auto disHitVer = sqrt(radius * radius - disVerSq);
    float t;
    if (fcmp(disOriCen - radius) > 0 && fcmp(disOriVer - disHitVer - tMin) >= 0) { // ray origin outside the sphere
        t = disOriVer - disHitVer;
    } else { // ray origin inside the sphere
        t = disOriVer + disHitVer;
    }

    if (fcmp(t - tMin) < 0) return false; // intersection behind the camera
    auto n = r.pointAtParameter(t) - center;
    n.Normalize();
    h.set(t, material, n, r);
    return true;
}

Vec3f Sphere::getPoint(float phi, float theta) {
    return center + Vec3f(radius * cos(phi) * cos(theta),
                          radius * cos(phi) * sin(theta), radius * sin(phi));
}

void Sphere::paint() {
    float deltaPhi = M_PI / phiSteps,
            deltaTheta = 2 * M_PI / thetaSteps;

    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    // phi from -90 to 90
    for (int iPhi = 1; iPhi <= phiSteps; iPhi++) {
        // theta from 0 to 360
        for (int iTheta = 1; iTheta <= thetaSteps; iTheta++) {
            float phi = -M_PI / 2 + iPhi * deltaPhi,
                    lastPhi = -M_PI / 2 + (iPhi - 1) * deltaPhi;
            float theta = iTheta * deltaTheta,
                    lastTheta = (iTheta - 1) * deltaTheta;

            // compute appropriate coordinates & normals
            Vec3f p1 = getPoint(lastPhi, lastTheta),
                    p2 = getPoint(lastPhi, theta),
                    p3 = getPoint(phi, lastTheta),
                    p4 = getPoint(phi, theta);
            Vec3f n1, n2, n3, n4;
            if (!gouraud) { // facet normal
                Vec3f::Cross3(n1, p2 - p1, p3 - p1);
                n1.Normalize();
                Vec3f::Cross3(n2, p4 - p2, p3 - p2);
                n2.Normalize();
            } else {
                n1 = p1 - center;
                n1.Normalize();
                n2 = p2 - center;
                n2.Normalize();
                n3 = p3 - center;
                n3.Normalize();
                n4 = p4 - center;
                n4.Normalize();
            }

            // send gl vertex commands
            if (!gouraud) {
                if ((p1 - p2).Length() > EPSILON) {
                    glNormal3f(n1.x(), n1.y(), n1.z());
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                }
                if ((p3 - p4).Length() > EPSILON) {
                    glNormal3f(n2.x(), n2.y(), n2.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                }
            } else {
                if ((p1 - p2).Length() > EPSILON) {
                    glNormal3f(n1.x(), n1.y(), n1.z());
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glNormal3f(n2.x(), n2.y(), n2.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glNormal3f(n3.x(), n3.y(), n3.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                }
                if ((p3 - p4).Length() > EPSILON) {
                    glNormal3f(n2.x(), n2.y(), n2.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glNormal3f(n3.x(), n3.y(), n3.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glNormal3f(n4.x(), n4.y(), n4.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                }
            }
        }
    }
    glEnd();
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m) {
    auto gridBBox = g->getBoundingBox();
    auto gridLo = gridBBox->getMin(), gridHi = gridBBox->getMax();
    auto[gridNX, gridNY, gridNZ] = g->getSize();
    auto gridDX = (gridHi.x() - gridLo.x()) / gridNX,
            gridDY = (gridHi.y() - gridLo.y()) / gridNY,
            gridDZ = (gridHi.z() - gridLo.z()) / gridNZ;
    auto gridDiag = sqrt((gridHi.x() - gridLo.x()) * (gridHi.x() - gridLo.x()) +
                         (gridHi.y() - gridLo.y()) * (gridHi.y() - gridLo.y()) +
                         (gridHi.z() - gridLo.z()) * (gridHi.z() - gridLo.z()));
    for (int i = 0; i < gridNX; i++) {
        for (int j = 0; j < gridNY; j++) {
            for (int k = 0; k < gridNZ; k++) {
                auto voxCen = gridLo + Vec3f(gridDX * (i + 0.5f),
                                             gridDY * (j + 0.5f), gridDZ * (k + 0.5f));
                if ((voxCen - center).Length() <= radius + gridDiag / 2.0f) {
                    g->setOpaqueness(i, j, k, true);
                }
            }
        }
    }
}
