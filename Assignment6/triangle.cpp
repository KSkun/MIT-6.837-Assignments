//
// Created by kskun on 2021/10/11.
//

#include "triangle.h"
#include "matrix.h"
#include "global.h"
#include "grid.h"
#include "raytracing_stats.h"
#include "transform.h"

bool Triangle::intersect(const Ray &r, Hit &h, float tMin) {
    RayTracingStats::IncrementNumIntersections();
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
//    auto nDotRd = normal.Dot3(r.getDirection());
//    if (nDotRd > -EPSILON) {
//        normal.Scale(-1, -1, -1);
//    }
    h.set(t, material, normal, r);
    return true;
}

void Triangle::paint() {
    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    // set normal
    glNormal3f(normal.x(), normal.y(), normal.z());
    // set vertices
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m) {
    auto[nx, ny, nz] = g->getSize();
    auto[lx, ly, lz] = g->getVoxelSize();
    auto ta = Vec3f(a), tb = Vec3f(b), tc = Vec3f(c);
    auto pMin = g->getMin();
    Object3D *objIn = this;
    if (m != nullptr) {
        m->Transform(ta);
        m->Transform(tb);
        m->Transform(tc);
        objIn = new Transform(*m, this);
    }
    auto min = Vec3f(std::min({ta.x(), tb.x(), tc.x()}),
                     std::min({ta.y(), tb.y(), tc.y()}),
                     std::min({ta.z(), tb.z(), tc.z()})),
            max = Vec3f(std::max({ta.x(), tb.x(), tc.x()}),
                        std::max({ta.y(), tb.y(), tc.y()}),
                        std::max({ta.z(), tb.z(), tc.z()}));
    auto epsVec = Vec3f(EPSILON, EPSILON, EPSILON);
    min -= epsVec;
    max += epsVec;
    auto iMin = (int) floor((min.x() - pMin.x()) / lx),
            iMax = (int) ceil((max.x() - pMin.x()) / lx),
            jMin = (int) floor((min.y() - pMin.y()) / ly),
            jMax = (int) ceil((max.y() - pMin.y()) / ly),
            kMin = (int) floor((min.z() - pMin.z()) / lz),
            kMax = (int) ceil((max.z() - pMin.z()) / lz);
    for (int i = std::max(iMin, 0); i <= std::min(iMax, nx - 1); i++) {
        for (int j = std::max(jMin, 0); j <= std::min(jMax, ny - 1); j++) {
            for (int k = std::max(kMin, 0); k <= std::min(kMax, nz - 1); k++) {
                g->insertObject(i, j, k, objIn);
            }
        }
    }
}