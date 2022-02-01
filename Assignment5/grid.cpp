//
// Created by kskun on 2021/12/14.
//

#include "grid.h"
#include "global.h"
#include "rayTree.h"

void Grid::paint() {
    auto bMin = bbox->getMin();
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                auto p1 = bMin + Vec3f(i * lx, j * ly, k * lz),
                        p2 = bMin + Vec3f((i + 1) * lx, j * ly, k * lz),
                        p3 = bMin + Vec3f((i + 1) * lx, (j + 1) * ly, k * lz),
                        p4 = bMin + Vec3f(i * lx, (j + 1) * ly, k * lz),
                        p5 = bMin + Vec3f(i * lx, j * ly, (k + 1) * lz),
                        p6 = bMin + Vec3f((i + 1) * lx, j * ly, (k + 1) * lz),
                        p7 = bMin + Vec3f((i + 1) * lx, (j + 1) * ly, (k + 1) * lz),
                        p8 = bMin + Vec3f(i * lx, (j + 1) * ly, (k + 1) * lz);

                glBegin(GL_QUADS);
                glNormal3f(0, 0, -1);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);
                glVertex3f(p5.x(), p5.y(), p5.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p7.x(), p7.y(), p7.z());
                glVertex3f(p8.x(), p8.y(), p8.z());

                glBegin(GL_QUADS);
                glNormal3f(0, -1, 0);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p5.x(), p5.y(), p5.z());
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glVertex3f(p8.x(), p8.y(), p8.z());
                glVertex3f(p7.x(), p7.y(), p7.z());

                glBegin(GL_QUADS);
                glNormal3f(-1, 0, 0);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glVertex3f(p8.x(), p8.y(), p8.z());
                glVertex3f(p5.x(), p5.y(), p5.z());
                glBegin(GL_QUADS);
                glNormal3f(1, 0, 0);
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p7.x(), p7.y(), p7.z());
            }
        }
    }
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tMin) const {
    mi.grid = this;

    const auto &Rd = r.getDirection(), &Ro = r.getOrigin(),
            &pMin = bbox->getMin(), &pMax = bbox->getMax();
    // intersect with bounding box
    // parallel no intersection
    for (int i = 0; i < 3; i++) {
        if (Rd[i] == 0 && (Ro[i] < pMin[i] || Ro[i] > pMax[i])) {
            mi.next = false;
            return;
        }
    }

    // calculate tNear & tFar
    float tNear, tFar;
    for (int i = 0; i < 3; i++) {
        float t1 = (pMin[i] - Ro[i]) / Rd[i],
                t2 = (pMax[i] - Ro[i]) / Rd[i];
        if (t1 > t2) std::swap(t1, t2);
        if (i == 0 || t1 > tNear) tNear = t1;
        if (i == 0 || t2 < tFar) tFar = t2;
    }
    // miss bounding box
    if (tNear > tFar || tFar < tMin) {
        mi.next = false;
        return;
    }

    // intersect with bounding box
    mi.next = true;

    mi.dTX = std::abs(lx / Rd.x());
    mi.dTY = std::abs(ly / Rd.y());
    mi.dTZ = std::abs(lz / Rd.z());

    mi.signX = Rd.x() < 0 ? -1 : (Rd.x() > 0 ? 1 : 0);
    mi.signY = Rd.y() < 0 ? -1 : (Rd.y() > 0 ? 1 : 0);
    mi.signZ = Rd.z() < 0 ? -1 : (Rd.z() > 0 ? 1 : 0);

    if (tNear > tMin) {
        // origin outside grid
        mi.tMin = tNear;
    } else {
        // origin inside grid
        mi.tMin = tMin;
    }

    // grid index
    auto p = r.pointAtParameter(mi.tMin),
            pRelative = p - pMin;
    mi.i = pRelative.x() / lx;
    mi.j = pRelative.y() / ly;
    mi.k = pRelative.z() / lz;

    // next grid index
    mi.tNextX = ((mi.i + mi.signX) * lx - pRelative.x()) / Rd.x() + mi.tMin;
    mi.tNextY = ((mi.j + mi.signY) * ly - pRelative.y()) / Rd.y() + mi.tMin;
    mi.tNextZ = ((mi.k + mi.signZ) * lz - pRelative.z()) / Rd.z() + mi.tMin;
}

void MarchingInfo::nextCell() {
    assert(next);
    tMin = std::min({tNextX, tNextY, tNextZ});

    if (tMin == tNextX) {
        tNextX += dTX * signX;
        i += signX;
    }
    if (tMin == tNextY) {
        tNextY += dTY * signY;
        j += signY;
    }
    if (tMin == tNextZ) {
        tNextZ += dTZ * signZ;
        k += signZ;
    }

    auto[nx, ny, nz] = grid->getSize();
    if ((i < 0 || i >= nx) && (j < 0 || j >= ny) && (k < 0 || k >= nz)) {
        next = false;
    }
}

void Grid::hitFace(const BoundingBox *bbox, const Vec3f &inter, const MarchingInfo &mi,
                   Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4, Vec3f &n) const {
    auto p = inter - bbox->getMin();
    if (fcmp(p.x() - lx * mi.i) == 0) {
        p1 = bbox->getMin() + Vec3f(lx * mi.i, ly * mi.j, lz * mi.k);
        p2 = bbox->getMin() + Vec3f(lx * mi.i, (ly + 1) * mi.j, lz * mi.k);
        p3 = bbox->getMin() + Vec3f(lx * mi.i, ly * mi.j, (lz + 1) * mi.k);
        p4 = bbox->getMin() + Vec3f(lx * mi.i, (ly + 1) * mi.j, (lz + 1) * mi.k);
        n.Set(1, 0, 0);
    } else if (fcmp(p.y() - ly * mi.j) == 0) {
        p1 = bbox->getMin() + Vec3f(lx * mi.i, ly * mi.j, lz * mi.k);
        p2 = bbox->getMin() + Vec3f((lx + 1) * mi.i, ly * mi.j, lz * mi.k);
        p3 = bbox->getMin() + Vec3f(lx * mi.i, ly * mi.j, (lz + 1) * mi.k);
        p4 = bbox->getMin() + Vec3f((lx + 1) * mi.i, ly * mi.j, (lz + 1) * mi.k);
        n.Set(0, 1, 0);
    } else if (fcmp(p.z() - lz * mi.k) == 0) {
        p1 = bbox->getMin() + Vec3f(lx * mi.i, ly * mi.j, lz * mi.k);
        p2 = bbox->getMin() + Vec3f((lx + 1) * mi.i, ly * mi.j, lz * mi.k);
        p3 = bbox->getMin() + Vec3f(lx * mi.i, (ly + 1) * mi.j, lz * mi.k);
        p4 = bbox->getMin() + Vec3f((lx + 1) * mi.i, (ly + 1) * mi.j, lz * mi.k);
        n.Set(0, 0, 1);
    } else {
        assert(false);
    }
}

bool Grid::intersect(const Ray &r, Hit &h, float tMin) {
    static Material *m = new PhongMaterial(
            {1, 1, 1}, {0, 0, 0}, 1,
            {0, 0, 0}, {0, 0, 0}, 1);
    MarchingInfo mi;
    initializeRayMarch(mi, r, tMin);

    auto bMin = bbox->getMin();
    while (mi.next && !getOpaqueness(mi.i, mi.j, mi.k)) {
        auto p = r.pointAtParameter(mi.tMin);
        Vec3f p1, p2, p3, p4, n;
        hitFace(bbox, p, mi, p1, p2, p3, p4, n);
        if (n.Dot3(r.getDirection()) > 0) n.Negate();
        RayTree::AddEnteredFace(p1, p2, p4, p3, n, m);

        mi.nextCell();
    }
    if (mi.next) {
        auto p = r.pointAtParameter(mi.tMin);
        Vec3f p1, p2, p3, p4, n;
        hitFace(bbox, p, mi, p1, p2, p3, p4, n);
        if (n.Dot3(r.getDirection()) > 0) n.Negate();
        RayTree::AddHitCellFace(p1, p2, p4, p3, n, m);

        h.set(mi.tMin, m, n, r);
        return true;
    }
    return false;
}